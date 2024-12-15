// Copyright (C) 2024 Ektishaf. All Rights Reserved. <https://www.ektishaf.com>

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Types/EktishafDataTypes.h"
#include "BlockchainSettings.generated.h"

const FString ApiService = "https://api.ektishaf.com";
const FString ApiVersion = "v1";

UCLASS(config=Game, defaultConfig, meta=(DisplayName = "Ektishaf"))
class EKTISHAF_API UBlockchainSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(config, EditAnywhere, Category = Blockchain, meta=(ToolTip = "A list of EVM compatible networks to be used for blockchain communication."))
	TArray<FEktishafNetwork> Networks;

	UPROPERTY(config, EditAnywhere, Category = Blockchain, meta=(ToolTip = "A list of accounts to be used for development purposes. To generate, please click Ektishaf->Generate New Accounts."))
	TArray<FEktishafAccount> Accounts;

	UPROPERTY(config, EditAnywhere, Category = Blockchain, meta=(ClampMin="1", ClampMax="5", UIMin="1", UIMax="5", ToolTip = "How many accounts to create when a request is made from Ektishaf->Generate New Accounts."))
	int MaxAccountsPerRequest;

	UPROPERTY(config, EditAnywhere, Category = Blockchain, meta=(ToolTip = "When a request is made to create new accounts, this password will be used for all of them. Make sure to modify (if needed) and remember this before Generate New Accounts."))
	FString GenerateAccountsWithPassword;

	UPROPERTY(config, EditAnywhere, Category = Blockchain, meta = (ToolTip = "An IPFS url to access NFT's assets path on blockchain. i.e. Pinata gateway url. (Only for contract owner)"))
	FString AssetGateway;

	UPROPERTY(config, EditAnywhere, Category = Blockchain, meta = (ToolTip = "The metadata hash obtained when all metadatas are uploaded for the NFTs. (Only for contract owner)"))
	FString MetadataHash;

	UPROPERTY(config, EditAnywhere, Category = Blockchain, meta = (ToolTip = "Shows or hides logs."))
	bool ShowLogs;

	UBlockchainSettings();

	static const UBlockchainSettings* GetBlockchainSettings();

private:
	UFUNCTION(BlueprintCallable, Category = Blockchain)
	FString GetUrl(const FString& api = "");

public:
	UFUNCTION(BlueprintCallable, Category = Blockchain)
	FEktishafNetwork GetNetwork(const FString& ChainId);

	UFUNCTION(BlueprintCallable, Category = Blockchain)
	FEktishafAccount GetAccount(const FString& Address);

	UFUNCTION(BlueprintCallable, Category = Blockchain)
	TArray<FString> GetMetadataUris(const TArray<int> Ids);

	UFUNCTION(BlueprintCallable, Category = Blockchain)
	FString Op(EServOp servOp = EServOp::None);
};