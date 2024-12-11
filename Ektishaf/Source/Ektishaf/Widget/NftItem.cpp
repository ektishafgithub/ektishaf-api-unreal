// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#include "NftItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "EktishafSubsystem.h"
#include "Blueprint/AsyncTaskDownloadImage.h"
#include "Engine/Texture2DDynamic.h"
#include "EktishafUI.h"

void UNftItem::Init(UEktishafUI* Ref, int Id, int Amount, FString MetadataUri)
{
	Parent = Ref;
	IdText->SetText(FText::FromString(FString::FromInt(Id)));
	AmountText->SetText(FText::FromString(FString::FromInt(Amount)));
	Uri = MetadataUri;
}

void UNftItem::GetImage()
{
	if(GEngine)
	{
		if (UEktishafSubsystem* Subsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
		{
			Subsystem->GetRequest(FEktishafOnResponseFast::CreateLambda([this](bool success, const TArray<uint8>, const FString content, TSharedPtr<FJsonObject> JsonObject)
			{
				if (success)
				{
					Parent->Log(FString::Printf(TEXT("Metadata Downloaded (%s)"), *Uri));
					ImageUrl = JsonObject->GetStringField(TEXT("image"));
					UAsyncTaskDownloadImage* DownloadTask = NewObject<UAsyncTaskDownloadImage>();
					DownloadTask->OnSuccess.AddDynamic(this, &ThisClass::OnDownloadImage);
					DownloadTask->Start(ImageUrl);
				}
			}), Uri);
		}
	}
}

void UNftItem::OnDownloadImage(UTexture2DDynamic* DownloadedTexture)
{
	Image->SetBrushFromTextureDynamic(DownloadedTexture);
	bIsDownloaded = true;
	Parent->PendingDownloads--;
	if (Parent->PendingDownloads == 0)
	{
		Parent->HideLoading();
	}
	Parent->Log(FString::Printf(TEXT("Image Downloaded (%s)"), *ImageUrl));
}