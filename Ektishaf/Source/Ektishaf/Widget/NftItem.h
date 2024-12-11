// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NftItem.generated.h"

UCLASS()
class EKTISHAF_API UNftItem : public UUserWidget
{
	GENERATED_BODY()

private:
	class UEktishafUI* Parent;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* IdText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AmountText;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image;

	UPROPERTY(EditAnywhere)
	bool bIsDownloaded = false;

	UPROPERTY(EditAnywhere)
	FString Uri;

	UPROPERTY(EditAnywhere)
	FString ImageUrl;

	UFUNCTION(BlueprintCallable)
	void Init(class UEktishafUI* Ref, int Id, int Amount, FString MetadataUri);

	UFUNCTION(BlueprintCallable)
	void GetImage();

	UFUNCTION()
	void OnDownloadImage(class UTexture2DDynamic* DownloadedTexture);
};

USTRUCT(BlueprintType)
struct FNftItemStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int Id;

	UPROPERTY(BlueprintReadWrite)
	int Amount;

	UPROPERTY(BlueprintReadWrite)
	FString Uri;
};