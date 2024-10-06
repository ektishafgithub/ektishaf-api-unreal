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
#include "EktishafSubsystem.h"

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
		EktishafSubsystem->ABI(ABIEditableTextBox->GetText().ToString(), true, FEktishafOnResponseFast::CreateLambda([this](bool success, const TArray<uint8>, const FString content, TSharedPtr<FJsonObject> JsonObject) 
		{
				
				if(success)
				{
					FString SavePathDir = FPaths::ProjectPluginsDir() + "Ektishaf/Source/Ektishaf/Public/Contracts";
					FString SavePath = SavePathDir + "/" + FileNameEditableTextBox->GetText().ToString() + ".h";

					IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
					if (!PlatformFile.DirectoryExists(*SavePathDir))
					{
						PlatformFile.CreateDirectory(*SavePathDir);
					}
					GenerateABI(content, ABIEditableTextBox->GetText().ToString(), ContractAddressEditableTextBox->GetText().ToString(), SavePath);

					const FText Title = Localize("UserInfo", "User Information");
					const FText Message = Localize("UserInfoMessage", FString::Printf(TEXT("Contract interface saved at path: %s"), *SavePath));
					EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, Title);
					if (UserSelection == EAppReturnType::Ok)
					{
						WindowManager::Get().GetWindow()->BringToFront();
					}
				}
				else
				{
					const FText Title = Localize("UserInfo", "User Information");
					const FText Message = Localize("UserInfoMessage", FString::Printf(TEXT("Something went wrong: %s"), *content));
					EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, Title);
					if (UserSelection == EAppReturnType::Ok)
					{
						WindowManager::Get().GetWindow()->BringToFront();
					}
				}
		}));
	}
	return FReply::Handled();
}

void SEktishafEditorWindow::GenerateABI(FString HBI, FString ABI, FString Address, FString FileName)
{
	TArray<TSharedPtr<FJsonValue>> ValueArray;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(HBI);
	if(FJsonSerializer::Deserialize(Reader, ValueArray))
	{ 
		TMap<FString, FString> dictionary;
		for (int32 i = 1; i < ValueArray.Num(); i++)
		{
			FString raw = ValueArray[i]->AsString();

			int32 argStartIndex = raw.Find("(", ESearchCase::IgnoreCase, ESearchDir::FromStart, 0);
			int32 argEndIndex = raw.Find(")", ESearchCase::IgnoreCase, ESearchDir::FromStart, argStartIndex + 1);
			FString rawArgs = raw.Mid(argStartIndex, (argEndIndex + 1) - argStartIndex);
			FString lineWithoutReturn = raw;
			lineWithoutReturn.RemoveAt(argEndIndex + 1, raw.Len() - (argEndIndex + 1));

			FString args = rawArgs.TrimChar('(').TrimChar(')');
			int numOfArgs = 0;
			FString a = "";
			if (!args.IsEmpty())
			{
				TArray<FString> splits;
				args.ParseIntoArray(splits, TEXT(","), true);
				numOfArgs = splits.Num();

				for (int32 j = 0; j < numOfArgs; j++)
				{
					FString split = splits[j];
					TArray<FString> argSplits;
					split.ParseIntoArray(argSplits, TEXT(" "), false);
					if (argSplits.Num() > 1)
					{
						split = argSplits[0];
					}
					a += split.Replace(TEXT("[]"), TEXT("Array"), ESearchCase::IgnoreCase);
					if (j < numOfArgs - 1) a += "_";
				}
			}

			FString lineWithoutArgs = raw.Replace(*rawArgs, TEXT(""), ESearchCase::IgnoreCase);
			lineWithoutArgs = lineWithoutArgs.TrimChar(',');
			lineWithoutArgs = lineWithoutArgs.TrimQuotes();

			TArray<FString> elements;
			lineWithoutArgs.ParseIntoArray(elements, TEXT(" "), true);
			FString functionName = elements[1];
			FString dump = FString::Printf(TEXT("%s_%d_%s"), *functionName, numOfArgs, *a);
			dictionary.Add(dump, raw);

			FString code = FString::Printf(TEXT("namespace %s\n"), *FPaths::GetBaseFilename(FileName));
			code = code.Append("{\n");
			for (const auto& entry : dictionary)
			{
				code.Append(FString::Printf(TEXT("\t\tconst FString %s = \"%s\"; \n"), *entry.Key, *entry.Value));
			}
			code.Append(FString::Printf(TEXT("\n\t\tconst FString Address = \"%s\";\n"), *Address));
			code.Append(FString::Printf(TEXT("\t\tconst FString ABI = \"%s\";\n"), *ABI.Replace(TEXT("\""), TEXT("\\\""), ESearchCase::IgnoreCase)));
			code.Append(FString::Printf(TEXT("\t\tconst FString HBI = \"%s\";\n"), *HBI.Replace(TEXT("\""), TEXT("\\\""), ESearchCase::IgnoreCase)));
			code = code.Append("}");
			FFileHelper::SaveStringToFile(code, *FileName);
		}
	}
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