// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#include "EktishafEditor.h"
#include "EktishafEditorStyle.h"
#include "EktishafEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "WindowManager.h"
#include "EktishafSubsystem.h"
#include "../../../../Ektishaf/Source/Ektishaf/Settings/BlockchainSettings.h"

#define LOCTEXT_NAMESPACE "FEktishafEditorModule"

void FEktishafEditorModule::StartupModule()
{
#if WITH_EDITOR	
	FEktishafEditorStyle::Initialize();
	FEktishafEditorStyle::ReloadTextures();
	FEktishafEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(FEktishafEditorCommands::Get().OpenPluginWindow, FExecuteAction::CreateRaw(this, &FEktishafEditorModule::PluginButtonClicked), FCanExecuteAction());
	PluginCommands->MapAction(FEktishafEditorCommands::Get().GenerateNewAccounts, FExecuteAction::CreateRaw(this, &FEktishafEditorModule::GenerateNewAccountsClicked), FCanExecuteAction());
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FEktishafEditorModule::RegisterMenus));
	WindowManager::Get().RegisterObjectCustomizations();
#endif
}

void FEktishafEditorModule::ShutdownModule()
{
#if WITH_EDITOR
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FEktishafEditorStyle::Shutdown();
	FEktishafEditorCommands::Unregister();
	WindowManager::Get().UnregisterObjectCustomizations();
#endif
}

TSharedRef<SDockTab> FEktishafEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FEktishafEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("EktishafEditor.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FEktishafEditorModule::PluginButtonClicked()
{
	TSharedPtr<SWindow> EktishafWindow = WindowManager::Get().GetWindow();
	if (EktishafWindow.IsValid())
	{
		EktishafWindow->ShowWindow();
	}
}

void FEktishafEditorModule::GenerateNewAccountsClicked()
{
	if (GEngine)
	{
		if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
		{
			if (Subsystem->Config->Accounts.Num() > 10)
			{
				UE_LOG(LogTemp, Warning, TEXT("Creating more than 10 accounts is not allowed."));
				return;
			}
			if (Subsystem->Config->GenerateAccountsWithPassword.IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("Please specify an account password in Project Settings->Game->Ektishaf->Accounts->GenerateAccountsWithPassword to be used for new accounts."));
				return;
			}
			Subsystem->Accounts(Subsystem->Config->MaxAccountsPerRequest, Subsystem->Config->GenerateAccountsWithPassword, FEktishafOnResponseFast::CreateLambda([this, Subsystem](bool success, const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
			{
				if (success && jsonObject.IsValid())
				{
					TArray<TSharedPtr<FJsonValue>> ValueArray = jsonObject->GetArrayField(TEXT("accounts"));
					if(ValueArray.Num() > 0)
					{
						for (int i = 0; i < ValueArray.Num(); i++)
						{
							TSharedPtr<FJsonObject> jObject = ValueArray[i]->AsObject();
							FString address = jObject->GetStringField(TEXT("address"));
							FString ticket = jObject->GetStringField(TEXT("ticket"));

							FEktishafAccount Account;
							Account.WalletAddress = address.ToLower();
							Account.Ticket = ticket;
							Subsystem->Config->Accounts.Add(Account);
							UE_LOG(LogTemp, Warning, TEXT("Generated Account %d: %s"), i + 1, *address);
						}
						Subsystem->Config->SaveConfig();
						GConfig->Flush(false, GGameIni);
						const FText Title = LOCTEXT("AccountsDialogInfoTitleKey", "New Accounts");
						const FText Message = LOCTEXT("AccountsDialogInfoMessageKey", "New accounts are created successfully.");
						EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, Title);
					}
				}
			}));
			
		}
	}
}

void FEktishafEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FEktishafEditorCommands::Get().OpenPluginWindow, PluginCommands);
			Section.AddMenuEntryWithCommandList(FEktishafEditorCommands::Get().GenerateNewAccounts, PluginCommands);
		}
	}
	{
		#if ENGINE_MAJOR_VERSION >= 5
		FName ToolBarName = "LevelEditor.LevelEditorToolBar.PlayToolBar";
		FName ExtensionPoint = "Play";
#else
		FName ToolBarName = "LevelEditor.LevelEditorToolBar";
		FName ExtensionPoint = "Settings";
#endif
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu(ToolBarName);
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection(ExtensionPoint);
			{
				FToolMenuEntry& EntryComboButton = Section.AddEntry(FToolMenuEntry::InitComboButton("Ektishaf", FUIAction(),
					FNewToolMenuChoice(FOnGetContent::CreateRaw(this, &FEktishafEditorModule::PopulateComboButton, PluginCommands)),
					LOCTEXT("Ektishaf", "Ektishaf"), 
					LOCTEXT("Ektishaf", "Ektishaf"),
					FEktishafEditorCommands::Get().OpenPluginWindow->GetIcon(), false));
				EntryComboButton.SetCommandList(PluginCommands);
			}
		}
	}
}

TSharedRef<SWidget> FEktishafEditorModule::PopulateComboButton(TSharedPtr<FUICommandList> Commands)
{
	FMenuBuilder MenuBuilder(true, Commands);
	MenuBuilder.AddMenuEntry(FEktishafEditorCommands::Get().OpenPluginWindow);
	MenuBuilder.AddMenuEntry(FEktishafEditorCommands::Get().GenerateNewAccounts);
	return MenuBuilder.MakeWidget();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEktishafEditorModule, EktishafEditor)