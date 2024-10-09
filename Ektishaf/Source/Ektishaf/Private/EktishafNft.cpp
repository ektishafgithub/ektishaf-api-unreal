// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#include "EktishafNft.h"
#include "EktishafSubsystem.h"
#include "Blueprint/AsyncTaskDownloadImage.h"

void UEktishafNft::DownloadImage()
{
	if (UEktishafSubsystem* EktishafSubsystem = GEngine->GetEngineSubsystem<UEktishafSubsystem>())
	{
		EktishafSubsystem->GetRequest(FEktishafOnResponseFast::CreateLambda([this](bool success, const TArray<uint8>, const FString content, TSharedPtr<FJsonObject> JsonObject)
		{
			if(success)
			{ 
				ImageUrl = JsonObject->GetStringField(TEXT("image"));
				UAsyncTaskDownloadImage* DownloadTask = NewObject<UAsyncTaskDownloadImage>();
				DownloadTask->OnSuccess.AddDynamic(this, &UEktishafNft::OnDownloadImage);
				DownloadTask->Start(ImageUrl);
			}
		}), MetadataUrl);
	}
}

void UEktishafNft::OnDownloadImage(UTexture2DDynamic* DownloadedTexture)
{
	Texture = DownloadedTexture;
}