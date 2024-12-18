// Copyright (C) 2024 Ektishaf. All Rights Reserved. <https://www.ektishaf.com>

#include "Settings/BlockchainSettings.h"
#include "Misc/ConfigCacheIni.h"
#include "PropertyEditorModule.h"
#include "PayloadBuilder.h"

UBlockchainSettings::UBlockchainSettings()
{
	MaxAccountsPerRequest = 2;
	ShowLogs = true;
}

const UBlockchainSettings* UBlockchainSettings::GetBlockchainSettings()
{
	return GetDefault<UBlockchainSettings>();
}

FEktishafNetwork UBlockchainSettings::GetNetwork(const FString& ChainId)
{
	if (Networks.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to obtain networks from Project Settings->Game->Ektishaf->Blockchain->Networks"));
		return FEktishafNetwork();
	}

	for(const FEktishafNetwork& Network : Networks)
	{
		if (Network.ChainId.ToUpper().Equals(ChainId.ToUpper()))
		{
			return Network;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("No network found with the chain id: %s"), *ChainId);
	return FEktishafNetwork();
}

FEktishafAccount UBlockchainSettings::GetAccount(const FString& Address)
{
	if (Accounts.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to obtain accounts from Project Settings->Game->Ektishaf->Accounts->Accounts"));
		return FEktishafAccount();
	}

	for (const FEktishafAccount& Account : Accounts)
	{
		if (Account.Address.ToUpper().Equals(Address.ToUpper()))
		{
			return Account;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("No account found with the address: %s"), *Address);
	return FEktishafAccount();
}

FString UBlockchainSettings::GetUrl(const FString& api)
{
	return FString::Printf(TEXT("%s/%s/%s"), *ApiService, *ApiVersion, *api);
}

TArray<FString> UBlockchainSettings::GetMetadataUris(const TArray<int>& Ids)
{
	TArray<FString> Uris;
	for (int i = 0; i < Ids.Num(); ++i)
	{
		Uris.Add(FString::Printf(TEXT("%s/%s/%d.json"), *AssetGateway, *MetadataHash, Ids[i]));
	}
	return Uris;
}

FString UBlockchainSettings::Op(const EServOp& servOp)
{
	switch (servOp)
	{
		case EServOp::None:     return GetUrl();
		case EServOp::Register: return GetUrl("register");
		case EServOp::Login:    return GetUrl("login");
		case EServOp::External: return GetUrl("external");
		case EServOp::Reveal:   return GetUrl("reveal");
		case EServOp::Sign:     return GetUrl("sign");
		case EServOp::Verify:   return GetUrl("verify");
		case EServOp::Balance:  return GetUrl("balance");
		case EServOp::ABI:      return GetUrl("abi");
		case EServOp::Read:     return GetUrl("read");
		case EServOp::Write:    return GetUrl("write");
		case EServOp::Accounts: return GetUrl("accounts");
		case EServOp::Send:     return GetUrl("send");
		default:                return GetUrl();
	}
}