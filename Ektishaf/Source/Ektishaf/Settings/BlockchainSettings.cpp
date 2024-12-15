// Copyright (C) 2024 Ektishaf. All Rights Reserved. <https://www.ektishaf.com>

#include "BlockchainSettings.h"
#include "Misc/ConfigCacheIni.h"
#include "PropertyEditorModule.h"
#include "PayloadBuilder.h"

UBlockchainSettings::UBlockchainSettings()
{
	// Sample Network
	FEktishafNetwork Sepolia;
	Sepolia.NetworkName = "Sepolia test network";
	Sepolia.Rpc = "https://eth-sepolia.g.alchemy.com/v2/YBy3ka0SJ5YW7aGgnz7oj-U_QUJ_pND4";
	Sepolia.ChainId = "11155111";
	Sepolia.CurrencySymbol = "SepoliaETH";
	Sepolia.BlockExplorer = "https://sepolia.etherscan.io";
	Networks.Add(Sepolia);

	// Sample Account
	FEktishafAccount SampleAccount;
	SampleAccount.WalletAddress = FString(TEXT("0xf0deb67ec9064794211e14938c639728bda2481a"));
	SampleAccount.Ticket = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwcm9qZWN0IjoiZ29yIiwicGFzc3dvcmQiOiJFa3Rpc2hhZiBBaHdheiIsImVuY3J5cHRpb24iOiJ7XCJhZGRyZXNzXCI6XCJmMGRlYjY3ZWM5MDY0Nzk0MjExZTE0OTM4YzYzOTcyOGJkYTI0ODFhXCIsXCJpZFwiOlwiZmZkYWVjOTUtOThjMi00YTdiLWI1YzItYWQ0YzZkYjNkZWY5XCIsXCJ2ZXJzaW9uXCI6MyxcIkNyeXB0b1wiOntcImNpcGhlclwiOlwiYWVzLTEyOC1jdHJcIixcImNpcGhlcnBhcmFtc1wiOntcIml2XCI6XCIyMWIyODY2ZjdlMjJkMTc2MjRhMTM4YWY2NDhmM2EzN1wifSxcImNpcGhlcnRleHRcIjpcImQ1NzFjOWZjNGRkN2Q5NGE2MGRkMjIwYzY0ZWRjZDlmZGQyNGYxMmU3ODVkZDEzM2Q0ZWI4OTc1OWM0OTFlNmRcIixcImtkZlwiOlwic2NyeXB0XCIsXCJrZGZwYXJhbXNcIjp7XCJzYWx0XCI6XCI3NmVjNzcxMDc2NTc0OTkzOWY3ZDliOTM0MjM3YTFlY2RhYjNiMzYzNDI3ZDk5OGY3OGFmMTdiNTMxYzVmODIwXCIsXCJuXCI6MTMxMDcyLFwiZGtsZW5cIjozMixcInBcIjoxLFwiclwiOjh9LFwibWFjXCI6XCI2ZGYyNTMwODQ4ZWY1MjI3MjNmYzU5MWJkZjVmYzU1ZmI3MTg0ODFiZjA2MzRkYWQ1Y2M4MWE5ZTlmMDZiN2FmXCJ9LFwieC1ldGhlcnNcIjp7XCJjbGllbnRcIjpcImV0aGVycy82LjEzLjJcIixcImdldGhGaWxlbmFtZVwiOlwiVVRDLS0yMDI0LTA4LTIzVDExLTQzLTAwLjBaLS1mMGRlYjY3ZWM5MDY0Nzk0MjExZTE0OTM4YzYzOTcyOGJkYTI0ODFhXCIsXCJwYXRoXCI6XCJtLzQ0Jy82MCcvMCcvMC8wXCIsXCJsb2NhbGVcIjpcImVuXCIsXCJtbmVtb25pY0NvdW50ZXJcIjpcImVkOGI4OTllNWVhMzU0ZDc5MGRmMDAxOGRlMzc3M2NmXCIsXCJtbmVtb25pY0NpcGhlcnRleHRcIjpcImU1NDZjNTBlYzVhZTcxNzlmNDMyNDFkYzFiZjJhM2I4XCIsXCJ2ZXJzaW9uXCI6XCIwLjFcIn19IiwiaWF0IjoxNzI0ODE5NzM5LCJleHAiOjE3MjQ5MDYxMzl9.2QPVZ6t_AHWKDdPTzj25Fj41dMzF764CagpFWKFGPuA";
	Accounts.Add(SampleAccount);

	MaxAccountsPerRequest = 5;

	AssetGateway = "https://azure-elaborate-quelea-290.mypinata.cloud/ipfs";
	MetadataHash = "QmRE6YQTpQxu725mXr5usqVGfcwGZFJrxDGjnZgCr7Ruso";
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
		if (Account.WalletAddress.ToUpper().Equals(Address.ToUpper()))
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

TArray<FString> UBlockchainSettings::GetMetadataUris(const TArray<int> Ids)
{
	TArray<FString> Uris;
	for (int i = 0; i < Ids.Num(); i++)
	{
		Uris.Add(FString::Printf(TEXT("%s/%s/%d.json"), *AssetGateway, *MetadataHash, Ids[i]));
	}
	return Uris;
}

FString UBlockchainSettings::Op(EServOp servOp)
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