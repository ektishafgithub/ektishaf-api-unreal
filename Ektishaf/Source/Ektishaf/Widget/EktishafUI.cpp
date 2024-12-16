// Copyright (C) 2024 Ektishaf. All Rights Reserved. <https://www.ektishaf.com>

#include "EktishafUI.h"
#include "Engine/Engine.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/GridPanel.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "EktishafSubsystem.h"
#include "NftItem.h"
#include "../Settings/BlockchainSettings.h"
#include "PayloadBuilder.h"
#include "Contracts/EktishafNftCollection.h"

bool UEktishafUI::Initialize()
{
	if (!Super::Initialize()) return false;

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
	if (AccountWalletButton)
	{
		AccountWalletButton->OnClicked.AddDynamic(this, &ThisClass::OnAccountWalletButtonClicked);
	}
	if (AccountSendButton)
	{
		AccountSendButton->OnClicked.AddDynamic(this, &ThisClass::OnAccountSendButtonClicked);
	}
	if (AccountToggleButton)
	{
		AccountToggleButton->OnClicked.AddDynamic(this, &ThisClass::OnAccountToggleButtonClicked);
	}
	if (AccountContinueButton)
	{
		AccountContinueButton->OnClicked.AddDynamic(this, &ThisClass::OnAccountContinueButtonClicked);
	}
	if (AccountNftButton)
	{
		AccountNftButton->OnClicked.AddDynamic(this, &ThisClass::OnAccountNftButtonClicked);
	}
	if (NftToggleButton)
	{
		NftToggleButton->OnClicked.AddDynamic(this, &ThisClass::OnNftToggleButtonClicked);
	}
	if (NftContinueButton)
	{
		NftContinueButton->OnClicked.AddDynamic(this, &ThisClass::OnNftContinueButtonClicked);
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
			if (LoginComboBoxString && AccountToComboBoxString && NftToComboBoxString)
			{
				for (const FEktishafAccount& Entry : Subsystem->Config->Accounts)
				{
					LoginComboBoxString->AddOption(Entry.Address.ToUpper());
					AccountToComboBoxString->AddOption(Entry.Address.ToUpper());
					NftToComboBoxString->AddOption(Entry.Address.ToUpper());
				}
				LoginComboBoxString->SetSelectedIndex(0);
				AccountToComboBoxString->SetSelectedIndex(0);
				NftToComboBoxString->SetSelectedIndex(0);
			}
		}
	}
	if (PanelWidgetSwitcher)
	{
		PanelWidgetSwitcher->SetActiveWidgetIndex(0);
	}
	if (TabWidgetSwitcher)
	{
		TabWidgetSwitcher->SetActiveWidgetIndex(0);
	}
	return true;
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
	PanelWidgetSwitcher->SetActiveWidgetIndex(1);
	Back->SetVisibility(ESlateVisibility::Visible);
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
					FString address = jsonObject->GetStringField(TEXT("address"));
					FString ticket = jsonObject->GetStringField(TEXT("ticket"));
					Subsystem->SetAccount(address, ticket);
					
					AddressText->SetText(FText::FromString(FString::Printf(TEXT("%s...%s"), *address.Mid(0, 5), *address.Mid(address.Len() - 5, 5))));
					BalanceText->SetText(FText::FromString(FString::Printf(TEXT("0 %s"), *Subsystem->CurrentNetwork.CurrencySymbol)));
					PanelWidgetSwitcher->SetActiveWidgetIndex(4);
					TabWidgetSwitcher->SetActiveWidgetIndex(0);
					
					Balance();
				}
				HideLoading();
			}));
		}
	}
}

void UEktishafUI::OnLoginButtonClicked()
{
	if (PanelWidgetSwitcher)
	{
		PanelWidgetSwitcher->SetActiveWidgetIndex(3);
		Back->SetVisibility(ESlateVisibility::Visible);
	}
}

void UEktishafUI::OnLoginSubmitButtonClicked()
{
	if (GEngine)
	{
		if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
		{
			FEktishafAccount Account = Subsystem->Config->GetAccount(LoginComboBoxString->GetSelectedOption());
			ShowLoading();
			Subsystem->Login(Account.Ticket, LoginPasswordEditableTextBox->GetText().ToString(), FEktishafOnResponseFast::CreateWeakLambda(this, [this, Subsystem](bool success, const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
			{
				if (success && jsonObject.IsValid())
				{
					FString address = jsonObject->GetStringField(TEXT("address"));
					FString ticket = jsonObject->GetStringField(TEXT("ticket"));
					Subsystem->SetAccount(address, ticket);
					
					AddressText->SetText(FText::FromString(FString::Printf(TEXT("%s...%s"), *address.Mid(0, 5), *address.Mid(address.Len() - 5, 5))));
					BalanceText->SetText(FText::FromString(FString::Printf(TEXT("0 %s"), *Subsystem->CurrentNetwork.CurrencySymbol)));
					PanelWidgetSwitcher->SetActiveWidgetIndex(4);
					TabWidgetSwitcher->SetActiveWidgetIndex(0);
					
					Balance();
				}
				HideLoading();
			}));
		}
	}
}

void UEktishafUI::OnImportButtonClicked()
{
	if (PanelWidgetSwitcher)
	{
		PanelWidgetSwitcher->SetActiveWidgetIndex(2);
		Back->SetVisibility(ESlateVisibility::Visible);
	}
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
					FString address = jsonObject->GetStringField(TEXT("address"));
					FString ticket = jsonObject->GetStringField(TEXT("ticket"));
					Subsystem->SetAccount(address, ticket);

					AddressText->SetText(FText::FromString(FString::Printf(TEXT("%s...%s"), *address.Mid(0, 5), *address.Mid(address.Len() - 5, 5))));
					BalanceText->SetText(FText::FromString(FString::Printf(TEXT("0 %s"), *Subsystem->CurrentNetwork.CurrencySymbol)));
					PanelWidgetSwitcher->SetActiveWidgetIndex(4);
					TabWidgetSwitcher->SetActiveWidgetIndex(0);

					Balance();
				}
				HideLoading();
			}));
		}
	}
}

void UEktishafUI::OnAccountWalletButtonClicked()
{
	TabWidgetSwitcher->SetActiveWidgetIndex(0);
	Balance();
}

void UEktishafUI::OnAccountSendButtonClicked()
{
	TabWidgetSwitcher->SetActiveWidgetIndex(2);
}

void UEktishafUI::OnAccountToggleButtonClicked()
{
	if (AccountToComboBoxStringBorder->GetVisibility() == ESlateVisibility::Visible)
	{
		AccountToComboBoxStringBorder->SetVisibility(ESlateVisibility::Hidden);
		AccountSendToAccountEditableTextBoxBorder->SetVisibility(ESlateVisibility::Visible);
	}
	else if (AccountSendToAccountEditableTextBoxBorder->GetVisibility() == ESlateVisibility::Visible)
	{
		AccountToComboBoxStringBorder->SetVisibility(ESlateVisibility::Visible);
		AccountSendToAccountEditableTextBoxBorder->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UEktishafUI::OnAccountContinueButtonClicked()
{
	if (GEngine)
	{
		if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
		{
			ShowLoading();
			FString To = AccountToComboBoxStringBorder->GetVisibility() == ESlateVisibility::Visible ? AccountToComboBoxString->GetSelectedOption() : AccountSendToAccountEditableTextBox->GetText().ToString();
			Subsystem->Send(Subsystem->CurrentNetwork.Rpc, To.ToLower(), AccountSendToAmountEditableTextBox->GetText().ToString(), Subsystem->CurrentAccount.Ticket, FEktishafOnResponseFast::CreateWeakLambda(this, [this, Subsystem](bool success, const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
			{
				if (success)
				{
					HideLoading();
					TabWidgetSwitcher->SetActiveWidgetIndex(0);
					Balance();
				}
			}));
		}
	}
}

void UEktishafUI::OnAccountNftButtonClicked()
{
	TabWidgetSwitcher->SetActiveWidgetIndex(1);
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

void UEktishafUI::OnNftToggleButtonClicked()
{
	if (NftToComboBoxStringBorder->GetVisibility() == ESlateVisibility::Visible)
	{
		NftToComboBoxStringBorder->SetVisibility(ESlateVisibility::Hidden);
		NftSendToAccountEditableTextBoxBorder->SetVisibility(ESlateVisibility::Visible);
	}
	else if (NftSendToAccountEditableTextBoxBorder->GetVisibility() == ESlateVisibility::Visible)
	{
		NftToComboBoxStringBorder->SetVisibility(ESlateVisibility::Visible);
		NftSendToAccountEditableTextBoxBorder->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UEktishafUI::OnNftContinueButtonClicked()
{
	if (GEngine)
	{
		if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
		{
			ShowLoading();
			FString To = NftToComboBoxStringBorder->GetVisibility() == ESlateVisibility::Visible ? NftToComboBoxString->GetSelectedOption() : NftSendToAccountEditableTextBox->GetText().ToString();
			TArray<TSharedPtr<FJsonValue>> Args;
			UPayloadBuilder::AddArrayItem(Args, Subsystem->CurrentAccount.Address.ToLower());
			UPayloadBuilder::AddArrayItem(Args, To.ToLower());
			UPayloadBuilder::AddArrayItem(Args, SelectedItem->Id);
			UPayloadBuilder::AddArrayItem(Args, FCString::Atoi(*NftSendToAmountEditableTextBox->GetText().ToString()));
			UPayloadBuilder::AddArrayItem(Args, FString("0x"));
			Subsystem->Write(Subsystem->CurrentNetwork.Rpc, Subsystem->CurrentAccount.Ticket, EktishafNftCollection::Address, EktishafNftCollection::safeTransferFrom_5_address_address_uint256_uint256_bytes, Args, FEktishafOnResponseFast::CreateWeakLambda(this, [this, Subsystem](bool success, const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
			{
				if (success)
				{
					HideLoading();
					TabWidgetSwitcher->SetActiveWidgetIndex(1);
					OnAccountNftButtonClicked();
				}
			}));
		}
	}
}

void UEktishafUI::OnBackButtonClicked()
{
	if (PanelWidgetSwitcher->GetActiveWidgetIndex() == 4 && TabWidgetSwitcher->GetActiveWidgetIndex() != 0)
	{
		TabWidgetSwitcher->SetActiveWidgetIndex(0);
		Balance();
	}
	else
	{
		Back->SetVisibility(ESlateVisibility::Hidden);
		PanelWidgetSwitcher->SetActiveWidgetIndex(0);

		if (GEngine)
		{
			if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
			{
				AddressText->SetText(FText::FromString(TEXT("0x")));
				BalanceText->SetText(FText::FromString(FString::Printf(TEXT("0 %s"), *Subsystem->CurrentNetwork.CurrencySymbol)));
			}
		}
		ClearGrid();
	}
}

void UEktishafUI::Balance()
{
	if (GEngine)
	{
		if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
		{
			Subsystem->Balance(Subsystem->CurrentNetwork.Rpc, Subsystem->CurrentAccount.Address, FEktishafOnResponseFast::CreateWeakLambda(this, [this, Subsystem](bool success, const TArray<uint8> bytes, const FString content, TSharedPtr<FJsonObject> jsonObject)
			{
				if (success && jsonObject.IsValid())
				{
					FString data = jsonObject->GetStringField(TEXT("data"));
					BalanceText->SetText(FText::FromString(FString::Printf(TEXT("%s %s"), *(data.Len() >= 5 ? data.Mid(0, 5) : data), *Subsystem->CurrentNetwork.CurrencySymbol)));
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