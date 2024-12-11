// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#pragma once

#include "Subsystems/EngineSubsystem.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "EktishafNft.h"

#include "EktishafSubsystem.generated.h"

#define L(x) UE_LOG(LogTemp, Warning, TEXT("%s"), *x)

DECLARE_DELEGATE_FourParams(FEktishafOnResponseFast, bool, const TArray<uint8>, const FString, TSharedPtr<FJsonObject>);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FEktishafOnResponse, bool, success, const FString, content);
DECLARE_DYNAMIC_DELEGATE_OneParam(FEktishafOnGetNfts, const TArray<UEktishafNft*>&, nfts);
DECLARE_DELEGATE_OneParam(FEktishafOnGetNftsFast, const TArray<TArray<FString>>&);

UCLASS(MinimalAPI)
class UEktishafSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

protected:
	FString ConnectedAddress;
	FString CurrentTicket;

	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();
	virtual bool ShouldCreateSubsystem(UObject* Outer) const;

	FString ExtractFunctionABI(FString FuncSig);
	FString ExtractFunctionName(FString FuncSig);

public:
	FString GetWalletAddress();
	FString GetCurrentTicket();
	void SetWalletAddress(FString Address);
	void SetCurrentTicket(FString Ticket);

public:
	class UBlockchainSettings* Config;

	EKTISHAF_API void SendRequest(const FEktishafOnResponseFast& Callback, const FString Url, const FString Payload = "", const FString Ticket = "", const FString Verb = "GET");
	EKTISHAF_API void GetRequest(const FEktishafOnResponseFast& Callback, const FString Url);
	EKTISHAF_API void PostRequest(const FEktishafOnResponseFast& Callback, const FString Url, const FString Payload = "", const FString Ticket = "");

	EKTISHAF_API void Host(const FEktishafOnResponseFast& Callback);
	EKTISHAF_API void Register(const FString Password, const FEktishafOnResponseFast& Callback);
	EKTISHAF_API void Login(const FString Ticket, const FString Password, const FEktishafOnResponseFast& Callback);
	EKTISHAF_API void External(const FString PrivateKey, const FString Password, const FEktishafOnResponseFast& Callback);
	EKTISHAF_API void Reveal(const FString Ticket, const FString Password, const FEktishafOnResponseFast& Callback);
	EKTISHAF_API void Sign(const FString Ticket, const FString Message, const FEktishafOnResponseFast& Callback);
	EKTISHAF_API void Verify(const FString Address, const FString Message, const FString Signature, const FEktishafOnResponseFast& Callback);
	EKTISHAF_API void Balance(const FString Rpc, const FString Ticket, const FEktishafOnResponseFast& Callback);
	EKTISHAF_API void ABI(const FString Abi, const bool Minimal, const FEktishafOnResponseFast& Callback);
	EKTISHAF_API void Read(const FString Rpc, const FString Ticket, const FString Contract, const FString Abi, const FString Function, const TArray<TSharedPtr<FJsonValue>> Args, const FEktishafOnResponseFast& Callback);
	EKTISHAF_API void Read(const FString Rpc, const FString Ticket, const FString Contract, const FString FuncSig, const TArray<TSharedPtr<FJsonValue>> Args, const FEktishafOnResponseFast& Callback);
	EKTISHAF_API void Write(const FString Rpc, const FString Ticket, const FString Contract, const FString Abi, const FString Function, const TArray<TSharedPtr<FJsonValue>> Args, const FEktishafOnResponseFast& Callback);
	EKTISHAF_API void Write(const FString Rpc, const FString Ticket, const FString Contract, const FString FuncSig, const TArray<TSharedPtr<FJsonValue>> Args, const FEktishafOnResponseFast& Callback);
	EKTISHAF_API void GetNfts(const FEktishafOnGetNftsFast& Callback);

	UFUNCTION(BlueprintCallable, DisplayName = "Host", Category = "Ektishaf")
	EKTISHAF_API void K2_Host(const FEktishafOnResponse& Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Register", Category = "Ektishaf")
	EKTISHAF_API void K2_Register(const FString Password, const FEktishafOnResponse& Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Login", Category = "Ektishaf")
	EKTISHAF_API void K2_Login(const FString Ticket, FString Password, const FEktishafOnResponse& Callback);

	UFUNCTION(BlueprintCallable, DisplayName="External", Category = "Ektishaf")
	EKTISHAF_API void K2_External(const FString PrivateKey, FString Password, const FEktishafOnResponse& Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Reveal", Category = "Ektishaf")
	EKTISHAF_API void K2_Reveal(const FString Ticket, FString Password, const FEktishafOnResponse& Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Sign", Category = "Ektishaf")
	EKTISHAF_API void K2_Sign(const FString Ticket, FString message, const FEktishafOnResponse& Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Verify", Category = "Ektishaf")
	EKTISHAF_API void K2_Verify(const FString Address, FString Message, FString Signature, const FEktishafOnResponse& Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Balance", Category = "Ektishaf")
	EKTISHAF_API void K2_Balance(const FString Rpc, FString Ticket, const FEktishafOnResponse& Callback);

	UFUNCTION(BlueprintCallable, DisplayName="ABI", Category = "Ektishaf")
	EKTISHAF_API void K2_ABI(const FString Abi, bool Minimal, const FEktishafOnResponse& Callback);

	UFUNCTION(BlueprintCallable, DisplayName="ConnectedAddress", Category = "Ektishaf")
	EKTISHAF_API FString K2_ConnectedAddress();

	UFUNCTION(BlueprintCallable, DisplayName="Ticket", Category = "Ektishaf")
	EKTISHAF_API FString K2_Ticket();

	UFUNCTION(BlueprintCallable, DisplayName="GetNfts", Category = "Ektishaf")
	EKTISHAF_API void K2_GetNfts(const FEktishafOnGetNfts& Callback);

	UFUNCTION(BlueprintCallable, DisplayName = "MintBatch", Category = "Ektishaf")
	EKTISHAF_API void K2_MintBatch(const FString To, TArray<int> Ids, TArray<int> Amounts, TArray<FString> Uris, const FEktishafOnResponse& Callback);
};