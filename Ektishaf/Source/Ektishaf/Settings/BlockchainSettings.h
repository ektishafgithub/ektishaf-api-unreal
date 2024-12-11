// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "BlockchainSettings.generated.h"

const FString ApiService = "https://api.ektishaf.com";
const FString ApiVersion = "v1";

UENUM(BlueprintType)
enum EServOp
{
	None,
	Register,
	Login,
	External,
	Reveal,
	Sign,
	Verify,
	Balance,
	ABI,
	Read,
	Write
};

UCLASS(config=Game, defaultConfig, meta=(DisplayName = "Ektishaf"))
class EKTISHAF_API UBlockchainSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(config, EditAnywhere, Category = Blockchain, meta=(ToolTip = "A list of rpc urls that can be used selectively."))
	TMap<FString, FString> RpcList;

	UPROPERTY(config, EditAnywhere, Category = Blockchain, meta = (ToolTip = "A test wallet address provided to see how the demo functionality works."))
	FString TestWalletAddress;

	UPROPERTY(config, EditAnywhere, Category = Blockchain, meta = (ToolTip = "A test ticket provided to allow authorization for demo blockchain operation."))
	FString TestTicket;

	UPROPERTY(config, EditAnywhere, Category = Blockchain, meta = (ToolTip = "An ipfs url to access NFT's assets on blockchain. i.e. Pinata gateway url. (This property is only for Studio who owns the contract)"))
	FString DataUrl;

	UPROPERTY(config, EditAnywhere, Category = Blockchain, meta = (ToolTip = "The metadata hash obtained when all metadatas are uploaded for the NFTs. (This property is only for Studio who owns the contract)"))
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
	FString GetRpc(const FString& RpcKey);

	UFUNCTION(BlueprintCallable, Category = Blockchain)
	TArray<FString> GetMetadataUris(const TArray<int> Ids);

	UFUNCTION(BlueprintCallable, Category = Blockchain)
	FString Op(EServOp servOp = EServOp::None);
};
