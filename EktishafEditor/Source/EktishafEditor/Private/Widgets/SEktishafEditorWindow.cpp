// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#include "../../Public/Widgets/SEktishafEditorWindow.h"
#include <SlateOptMacros.h>
#include <Delegates/DelegateSignatureImpl.inl>
#include <HAL/FileManagerGeneric.h>
#include <Misc/FileHelper.h>
#include <Styling/SlateBrush.h>
#include <Widgets/Input/SMultiLineEditableTextBox.h>
#include <WindowManager.h>
#include <Widgets/SOverlay.h>
#include <Engine/Engine.h>
#include <Misc/MessageDialog.h>
#include <Widgets/SWindow.h>
#include <../../../../../Plugins/Ektishaf/Source/Ektishaf//Public/EktishafSubsystem.h>

#define LOCTEXT_NAMESPACE "LocalizedText"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEktishafEditorWindow::Construct(const FArguments& InArgs)
{
	LoadResources();
	
	ChildSlot
	[	
		SNew(SOverlay)

		 + SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage).Image(BackgroundBrush)
		]

		+SOverlay::Slot()
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		[
			SAssignNew(VerticalBoxList, SVerticalBox)
		]
	];
	
	DrawWidget();
}

void SEktishafEditorWindow::LoadResources()
{
	HeaderBackgroundBrush = new FSlateBrush();
	HeaderBackgroundBrush->TintColor = FLinearColor(0.025f, 0.025f, 0.025f, 1.f);
	HeaderBackgroundBrush->SetImageSize(FVector2D(64.f, 64.f));

	BackgroundBrush = new FSlateBrush();
#if ENGINE_MAJOR_VERSION >= 5
	BackgroundBrush->TintColor = FLinearColor(0.01f, 0.01f, 0.01f, 1.f);
#else
	BackgroundBrush->TintColor = FLinearColor(0.05f, 0.05f, 0.05f, 1.f);
#endif

	HeaderLargeTextStyle = GetTextStyle("EmbossedText", "Normal", 14);
	HeaderSmallTextStyle = GetTextStyle("EmbossedText", "Normal", 11);
	ButtonTextStyle = GetTextStyle("EmbossedText", "Normal", 10);
}

void SEktishafEditorWindow::DrawWidget()
{
	VerticalBoxList->AddSlot()
	.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
	.AutoHeight()
	[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(Localize("ABI", "Contract ABI"))
			]

			+ SVerticalBox::Slot()
			.Padding(FMargin(15.f, 15.0f, 15.f, 0.f))
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.AutoHeight()
			[
				SAssignNew(ABIEditableTextBox, SMultiLineEditableTextBox)
			]

			+ SVerticalBox::Slot()
			.Padding(FMargin(15.f, 50.f, 15.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				
					+ SHorizontalBox::Slot()
					.Padding(FMargin(0.f, 0.f, 15.f, 0.f))
					.HAlign(HAlign_Left)
					.FillWidth(100.f)
					[
						SNew(STextBlock)
							.Text(Localize("ContractAddress", "Contract Address: "))
							.MinDesiredWidth(100.f)
					]

					+ SHorizontalBox::Slot()
					.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
					.HAlign(HAlign_Fill)
					.FillWidth(256.f)
					[
						SAssignNew(ContractAddressEditableTextBox, SEditableTextBox)
						.MinDesiredWidth(256.f)
							.Text(Localize("ContractAddressText", "0x"))
					]
				
			]

			+ SVerticalBox::Slot()
			.Padding(FMargin(15.f, 25.f, 15.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				
					+ SHorizontalBox::Slot()
					.Padding(FMargin(0.f, 0.f, 15.f, 0.f))
					.HAlign(HAlign_Left)
					.FillWidth(100.f)
					[
						SNew(STextBlock)
							.Text(Localize("SaveFilename", "Save Filename: "))
							.MinDesiredWidth(100.f)
					]

					+ SHorizontalBox::Slot()
					.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
					.HAlign(HAlign_Fill)
					.FillWidth(256.f)
					[
						SAssignNew(FileNameEditableTextBox, SEditableTextBox)
						.MinDesiredWidth(256.f)
						.Text(Localize("SaveFilenameText", "{ContractInterface}"))
					]
				
			]

			+ SVerticalBox::Slot()
			.Padding(FMargin(15.f, 25.f, 15.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.Padding(FMargin(0.f, 0.f, 15.f, 0.f))
				.HAlign(HAlign_Left)
				[
					SNew(SButton)
					.DesiredSizeScale(FVector2D(1.5f, 1.f))
					.OnClicked(this, &SEktishafEditorWindow::OnGenerateButtonClicked)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
						.Text(Localize("Generate", "Generate"))
						.Justification(ETextJustify::Center)
					]
				]

				+ SHorizontalBox::Slot()
				.Padding(FMargin(0.f, 0.f, 15.f, 0.f))
				.HAlign(HAlign_Left)
				[
					SNew(SButton)
						.DesiredSizeScale(FVector2D(1.5f, 1.f))

						.OnClicked(this, &SEktishafEditorWindow::OnCancelButtonClicked)
						[
							SNew(STextBlock)
								.Font(ButtonTextStyle)
								.Text(Localize("Cancel", "Cancel"))
								.Justification(ETextJustify::Center)
						]
				]
			]
	];
}

FReply SEktishafEditorWindow::OnGenerateButtonClicked()
{
	EktishafSubsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>();
	if (EktishafSubsystem)
	{
		EktishafSubsystem->ABI(ABIEditableTextBox->GetText().ToString(), true, FEktishafOnResponseFast::CreateLambda([this](bool success, const TArray<uint8>, const FString content, TSharedPtr<FJsonObject>) 
		{
				if(success)
				{
					FString SavePathDir = FPaths::ProjectDir() + "EktishafGenerated/Contracts";
					FString SavePath = SavePathDir + "/" + FileNameEditableTextBox->GetText().ToString() + "_" + ContractAddressEditableTextBox->GetText().ToString() + ".json";
					IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
					if (!PlatformFile.DirectoryExists(*SavePathDir))
					{
						PlatformFile.CreateDirectory(*SavePath);
					}
					FFileHelper::SaveStringToFile(content, *SavePath);

					const FText Title = Localize("UserInfo", "User Information");
					const FText Message = Localize("UserInfoMessage", FString::Printf(TEXT("Contract interface saved at path: %s"), *SavePath));
					EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, &Title);
					if (UserSelection == EAppReturnType::Ok)
					{
						WindowManager::Get().GetWindow()->BringToFront();
					}
				}
				else
				{
					const FText Title = Localize("UserInfo", "User Information");
					const FText Message = Localize("UserInfoMessage", FString::Printf(TEXT("Something went wrong: %s"), *content));
					EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, &Title);
					if (UserSelection == EAppReturnType::Ok)
					{
						WindowManager::Get().GetWindow()->BringToFront();
					}
				}
		}));
	}
	return FReply::Handled();
}

void SEktishafEditorWindow::OnABI(bool success, const FString content)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *content);
}

FReply SEktishafEditorWindow::OnCancelButtonClicked()
{
	WindowManager::Get().CloseWindow();	
	return FReply::Handled();
}

void SEktishafEditorWindow::UnloadResources()
{
	HeaderBackgroundBrush = nullptr;
	BackgroundBrush = nullptr;
	VerticalBoxList = nullptr;
}

FText SEktishafEditorWindow::Localize(FString Key, FString Text)
{
	return FText::Format(LOCTEXT("{0}", "{1}"), FText::FromString(Key), FText::FromString(Text));
}

FSlateFontInfo SEktishafEditorWindow::GetTextStyle(FName PropertyName, FName FaceName, int32 Size)
{
	FSlateFontInfo FontInfo = FCoreStyle::Get().GetFontStyle(PropertyName);
	FontInfo.Size = Size;
	FontInfo.TypefaceFontName = FaceName;
	return FontInfo;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
#undef LOCTEXT_NAMESPACE