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

#define LOCTEXT_NAMESPACE "FEktishafEditorModule"

void FEktishafEditorModule::StartupModule()
{
#if WITH_EDITOR	
	FEktishafEditorStyle::Initialize();
	FEktishafEditorStyle::ReloadTextures();
	FEktishafEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(FEktishafEditorCommands::Get().OpenPluginWindow, FExecuteAction::CreateRaw(this, &FEktishafEditorModule::PluginButtonClicked), FCanExecuteAction());
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

void FEktishafEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FEktishafEditorCommands::Get().OpenPluginWindow, PluginCommands);
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
	return MenuBuilder.MakeWidget();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEktishafEditorModule, EktishafEditor)