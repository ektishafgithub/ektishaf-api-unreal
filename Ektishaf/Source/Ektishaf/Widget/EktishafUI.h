// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

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

private:
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
	class UButton* NftButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Back;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UGridPanel* GridPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* WalletPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* RegisterWalletPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* LoginWalletPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* ImportWalletPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* NftPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* LoadingPanel;

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
	void OnNftButtonClicked();

	void Balance();
	void AddNft(int Id, int Amount, FString Uri);
	void AddNfts(TArray<TArray<FString>> _nfts);
	void DownloadNfts();
	void GrabNfts(TArray<TArray<FString>> _nfts);

public:
	void HideAllPanels();
	void SetPanel(class UCanvasPanel* panel);
	void ShowLoading();
	void HideLoading();
	void ClearGrid();
};
