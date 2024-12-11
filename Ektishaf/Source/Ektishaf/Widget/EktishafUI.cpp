// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#include "EktishafUI.h"
#include "Engine/Engine.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/GridPanel.h"
#include "Components/TextBlock.h"
#include "EktishafSubsystem.h"
#include "NftItem.h"
#include "../Settings/BlockchainSettings.h"

bool UEktishafUI::Initialize()
{
	if (!Super::Initialize()) return false;

	if (BalanceText)
	{
		BalanceText->SetVisibility(ESlateVisibility::Hidden);
	}
	if (RegisterButton)
	{
		RegisterButton->OnClicked.AddDynamic(this, &ThisClass::OnRegisterButtonClicked);
	}
	if (RegisterSubmitButton)
	{
		RegisterSubmitButton->OnClicked.AddDynamic(this, &ThisClass::OnRegisterSubmitButtonClicked);
	}
	if (LoginButton)
	{
		LoginButton->OnClicked.AddDynamic(this, &ThisClass::OnLoginButtonClicked);
	}
	if (LoginSubmitButton)
	{
		LoginSubmitButton->OnClicked.AddDynamic(this, &ThisClass::OnLoginSubmitButtonClicked);
	}
	if (ImportButton)
	{
		ImportButton->OnClicked.AddDynamic(this, &ThisClass::OnImportButtonClicked);
	}
	if (ImportSubmitButton)
	{
		ImportSubmitButton->OnClicked.AddDynamic(this, &ThisClass::OnImportSubmitButtonClicked);
	}
	if (NftButton)
	{
		NftButton->OnClicked.AddDynamic(this, &ThisClass::OnNftButtonClicked);
	}
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButtonClicked);
		Back->SetVisibility(ESlateVisibility::Hidden);
	}
	if (GEngine)
	{
		if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
		{
			if (LoginComboBoxString) 
			{
				LoginComboBoxString->AddOption(Subsystem->Config->TestWalletAddress.ToUpper());
				LoginComboBoxString->SetSelectedIndex(0);
			}
		}
	}
	Log(FString::Printf(TEXT("EktishafUI Initialized Successfully.")));
	return true;
}

void UEktishafUI::HideAllPanels()
{
	WalletPanel->SetVisibility(ESlateVisibility::Hidden);
	RegisterWalletPanel->SetVisibility(ESlateVisibility::Hidden);
	LoginWalletPanel->SetVisibility(ESlateVisibility::Hidden);
	ImportWalletPanel->SetVisibility(ESlateVisibility::Hidden);
	NftPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UEktishafUI::SetPanel(UCanvasPanel* panel)
{
	HideAllPanels();
	panel->SetVisibility(ESlateVisibility::Visible);
	Back->SetVisibility(panel == WalletPanel ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UEktishafUI::ShowLoading()
{
	LoadingPanel->SetVisibility(ESlateVisibility::Visible);
}

void UEktishafUI::HideLoading()
{
	LoadingPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UEktishafUI::ClearGrid()
{
	if (GridPanel->GetChildrenCount() > 0)
	{
		for (int i = 0; i < GridPanel->GetChildrenCount(); i++)
		{
			GridPanel->RemoveChildAt(i);
		}
	}
	GridPanel->ClearChildren();
	Nfts.Empty();
	ColumnIndex = 0;
	RowIndex = 0;
}

void UEktishafUI::AddNft(int Id, int Amount, FString Uri)
{
	UNftItem* Widget = Cast<UNftItem>(CreateWidget(this, NftTemplate));
	Widget->Init(this, Id, Amount, Uri);
	GridPanel->AddChildToGrid(Widget, RowIndex, ColumnIndex);
	Nfts.Add(Widget);

	GridPanel->SetColumnFill(ColumnIndex, 0.0f);
	GridPanel->SetRowFill(RowIndex, 0.0f);

	ColumnIndex++;
	if (ColumnIndex % TotalColumns == 0)
	{
		ColumnIndex = 0;
		RowIndex++;
	}
}

void UEktishafUI::AddNfts(TArray<TArray<FString>> _nfts)
{
	if (_nfts.Num() > 0)
	{
		for (int i = 0; i < _nfts.Num(); i++)
		{
			TArray<FString> nft = _nfts[i];
			AddNft(FCString::Atoi(*nft[0]), FCString::Atoi(*nft[1]), nft[2]);
		}
	}
}

void UEktishafUI::DownloadNfts()
{
	if (Nfts.Num() > 0)
	{
		for (auto& nft : Nfts)
		{
			if (!nft->bIsDownloaded)
			{
				nft->GetImage();
			}
		}
	}
}

void UEktishafUI::GrabNfts(TArray<TArray<FString>> _nfts)
{
	PendingDownloads = _nfts.Num();
	ClearGrid();
	AddNfts(_nfts);
	DownloadNfts();
}

void UEktishafUI::OnRegisterButtonClicked()
{
	SetPanel(RegisterWalletPanel);
}

void UEktishafUI::OnRegisterSubmitButtonClicked()
{
	if (GEngine)
	{
		if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
		{
			ShowLoading();
			Subsystem->Register(RegisterPasswordEditableTextBox->GetText().ToString(), FEktishafOnResponseFast::CreateWeakLambda(this, [this, Subsystem](bool success, const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
			{
				if (success)
				{
					Subsystem->SetWalletAddress(jsonObject->GetStringField(TEXT("address")));
					Subsystem->SetCurrentTicket(jsonObject->GetStringField(TEXT("ticket")));
					SetPanel(NftPanel);
					Balance();
				}
				HideLoading();
			}));
		}
	}
}

void UEktishafUI::OnLoginButtonClicked()
{
	SetPanel(LoginWalletPanel);
}

void UEktishafUI::OnLoginSubmitButtonClicked()
{
	if (GEngine)
	{
		if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
		{
			ShowLoading();
			Subsystem->Login(Subsystem->Config->TestTicket, LoginPasswordEditableTextBox->GetText().ToString(), FEktishafOnResponseFast::CreateWeakLambda(this, [this, Subsystem](bool success, const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
			{
				if (success && jsonObject.IsValid())
				{
					Subsystem->SetWalletAddress(jsonObject->GetStringField(TEXT("address")));
					Subsystem->SetCurrentTicket(jsonObject->GetStringField(TEXT("ticket")));
					SetPanel(NftPanel);
					Balance();
				}
				HideLoading();
			}));
		}
	}
}

void UEktishafUI::OnImportButtonClicked()
{
	SetPanel(ImportWalletPanel);
}

void UEktishafUI::OnImportSubmitButtonClicked()
{
	if (GEngine)
	{
		if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
		{
			ShowLoading();
			Subsystem->External(ImportPrivateKeyEditableTextBox->GetText().ToString(), ImportPasswordEditableTextBox->GetText().ToString(), FEktishafOnResponseFast::CreateWeakLambda(this, [this, Subsystem](bool success, const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
			{
				if (success)
				{
					Subsystem->SetWalletAddress(jsonObject->GetStringField(TEXT("address")));
					Subsystem->SetCurrentTicket(jsonObject->GetStringField(TEXT("ticket")));
					SetPanel(NftPanel);
					Balance();
				}
				HideLoading();
			}));
		}
	}
}

void UEktishafUI::OnNftButtonClicked()
{
	if (GEngine)
	{
		if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
		{
			ShowLoading();
			Subsystem->GetNfts(FEktishafOnGetNftsFast::CreateWeakLambda(this, [this](TArray<TArray<FString>> nfts)
			{
				if(nfts.Num() > 0)
				{
					GrabNfts(nfts);
				}
				else
				{
					HideLoading();
				}
			}));
		}
	}
}

void UEktishafUI::OnBackButtonClicked()
{
	SetPanel(WalletPanel);
	BalanceText->SetText(FText::FromString("BAL: 0"));
	BalanceText->SetVisibility(ESlateVisibility::Hidden);
	ClearGrid();
}

void UEktishafUI::Balance()
{
	BalanceText->SetVisibility(ESlateVisibility::Visible);
	if (GEngine)
	{
		if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
		{
			Subsystem->Balance(Subsystem->Config->GetRpc("TestRpc"), Subsystem->GetCurrentTicket(), FEktishafOnResponseFast::CreateWeakLambda(this, [this](bool success, const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
			{
				if (success && jsonObject.IsValid())
				{
					FString data = jsonObject->GetStringField(TEXT("data"));
					BalanceText->SetText(FText::FromString(FString::Printf(TEXT("BAL: %s"), *(data.Len() >= 5 ? data.Mid(0, 5) : data))));
				}
			}));
		}
	}
}

void UEktishafUI::Log(FString Message)
{
	if (GEngine)
	{
		if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
		{
			Subsystem->Log(Message);
		}
	}
}