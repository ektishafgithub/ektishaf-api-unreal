// Copyright (C) 2024 Ektishaf. All Rights Reserved. <https://www.ektishaf.com>

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EktishafUI.generated.h"

UCLASS()
class EKTISHAF_API UEktishafUI : public UUserWidget
{
	GENERATED_BODY()
	
	bool Initialize();

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UNftItem> NftTemplate;

	UPROPERTY(EditAnywhere)
	class UNftItem* SelectedItem;

	UPROPERTY(BlueprintReadOnly)
	int ColumnIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	int RowIndex = 0;

	UPROPERTY(EditAnywhere)
	int TotalColumns;

	UPROPERTY(EditAnywhere)
	int TotalRows;

	UPROPERTY(BlueprintReadOnly)
	int PendingDownloads = 0;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UNftItem*> Nfts;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AddressText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BalanceText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* RegisterButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* RegisterSubmitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* RegisterPasswordEditableTextBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* LoginButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* LoginSubmitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UComboBoxString* LoginComboBoxString;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* LoginPasswordEditableTextBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ImportButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ImportSubmitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* ImportPrivateKeyEditableTextBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* ImportPasswordEditableTextBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* AccountWalletButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* AccountSendButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* AccountToComboBoxStringBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UComboBoxString* AccountToComboBoxString;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* AccountSendToAccountEditableTextBoxBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* AccountSendToAccountEditableTextBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* AccountSendToAmountEditableTextBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* AccountToggleButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* AccountContinueButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* AccountNftButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* NftToComboBoxStringBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UComboBoxString* NftToComboBoxString;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* NftSendToAccountEditableTextBoxBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* NftSendToAccountEditableTextBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* NftSendToAmountEditableTextBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* NftToggleButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* NftContinueButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Back;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UGridPanel* GridPanel;

	/*UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* WalletPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* RegisterWalletPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* LoginWalletPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* ImportWalletPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* NftPanel;*/

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* LoadingPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UWidgetSwitcher* PanelWidgetSwitcher;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UWidgetSwitcher* TabWidgetSwitcher;

	UFUNCTION()
	void OnRegisterButtonClicked();

	UFUNCTION()
	void OnRegisterSubmitButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnLoginButtonClicked();

	UFUNCTION()
	void OnLoginSubmitButtonClicked();

	UFUNCTION()
	void OnImportButtonClicked();

	UFUNCTION()
	void OnImportSubmitButtonClicked();

	UFUNCTION()
	void OnAccountWalletButtonClicked();

	UFUNCTION()
	void OnAccountSendButtonClicked();

	UFUNCTION()
	void OnAccountToggleButtonClicked();

	UFUNCTION()
	void OnAccountContinueButtonClicked();

	UFUNCTION()
	void OnAccountNftButtonClicked();

	UFUNCTION()
	void OnNftToggleButtonClicked();

	UFUNCTION()
	void OnNftContinueButtonClicked();

	void Balance();
	void AddNft(int Id, int Amount, FString Uri);
	void AddNfts(TArray<TArray<FString>> _nfts);
	void DownloadNfts();
	void GrabNfts(TArray<TArray<FString>> _nfts);

public:
	void ShowLoading();
	void HideLoading();
	void ClearGrid();
	void Log(FString Message);
};
