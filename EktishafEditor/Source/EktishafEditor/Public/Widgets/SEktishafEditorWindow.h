// Copyright (C) 2024 Ektishaf. All Rights Reserved. <https://www.ektishaf.com>

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include <Widgets/DeclarativeSyntaxSupport.h>
#include <Brushes/SlateDynamicImageBrush.h>
#include <Widgets/SBoxPanel.h>
#include <Widgets/Input/SEditableTextBox.h>
#include <Widgets/Input/SMultiLineEditableTextBox.h>
#include <Widgets/Input/SButton.h>

class UEktishafSubsystem;

class EKTISHAFEDITOR_API SEktishafEditorWindow: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEktishafEditorWindow)
	{}
	SLATE_END_ARGS()

	UEktishafSubsystem* EktishafSubsystem;
	TSharedPtr<SVerticalBox> VerticalBoxList;
	TSharedPtr<SMultiLineEditableTextBox> ABIEditableTextBox;
	TSharedPtr<SEditableTextBox> ContractAddressEditableTextBox;
	TSharedPtr<SEditableTextBox> FileNameEditableTextBox;
	TSharedPtr<SButton> GenerateButton;
	TSharedPtr<SButton> CancelButton;

	FSlateBrush* HeaderBackgroundBrush;
	FSlateBrush* BackgroundBrush;
	FSlateFontInfo HeaderLargeTextStyle;
	FSlateFontInfo HeaderSmallTextStyle;
	FSlateFontInfo ButtonTextStyle;
	void Construct(const FArguments& InArgs);
	void LoadResources();
	void DrawWidget();
	FReply OnGenerateButtonClicked();
	void GenerateABI(FString HBI, FString ABI, FString Address, FString FileName);
	FReply OnCancelButtonClicked();
	void UnloadResources();
	FText Localize(FString Key, FString Text);
	FSlateFontInfo GetTextStyle(FName PropertyName, FName FaceName, int32 Size);
};
