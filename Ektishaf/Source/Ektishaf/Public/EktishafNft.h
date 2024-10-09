// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2DDynamic.h"

#include "EktishafNft.generated.h"

UCLASS(Blueprintable, BlueprintType)
class EKTISHAF_API UEktishafNft : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite)
	int Id;

	UPROPERTY(BlueprintReadWrite)
	int Amount;

	UPROPERTY(BlueprintReadWrite)
	FString MetadataUrl;

	UPROPERTY(BlueprintReadWrite)
	FString ImageUrl;

	UPROPERTY(BlueprintReadOnly)
	UTexture2DDynamic* Texture;

	UFUNCTION(BlueprintCallable)
	void DownloadImage();

	UFUNCTION()
	void OnDownloadImage(UTexture2DDynamic* DownloadedTexture);
};