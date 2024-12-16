// Copyright (C) 2024 Ektishaf. All Rights Reserved. <https://www.ektishaf.com>

#pragma once

#include "CoreMinimal.h"
#include "EktishafDataTypes.generated.h"

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
	Write,
	Accounts,
	Send
};

USTRUCT(BlueprintType)
struct EKTISHAF_API FEktishafNetwork
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Ektishaf|Blockchain|Network")
	FString NetworkName;

	UPROPERTY(EditAnywhere, Category = "Ektishaf|Blockchain|Network")
	FString Rpc;

	UPROPERTY(EditAnywhere, Category = "Ektishaf|Blockchain|Network")
	FString ChainId;

	UPROPERTY(EditAnywhere, Category = "Ektishaf|Blockchain|Network")
	FString CurrencySymbol;

	UPROPERTY(EditAnywhere, Category = "Ektishaf|Blockchain|Network")
	FString BlockExplorer;
};

USTRUCT(BlueprintType)
struct EKTISHAF_API FEktishafAccount
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Ektishaf|Blockchain|Account")
	FString Address;

	UPROPERTY(EditAnywhere, Category = "Ektishaf|Blockchain|Account")
	FString Ticket;
};

USTRUCT(BlueprintType)
struct EKTISHAF_API FEktishafNft
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Ektishaf|Blockchain|NFT")
	int Id;

	UPROPERTY(EditAnywhere, Category = "Ektishaf|Blockchain|NFT")
	int Amount;

	UPROPERTY(EditAnywhere, Category = "Ektishaf|Blockchain|NFT")
	FString Uri;
};