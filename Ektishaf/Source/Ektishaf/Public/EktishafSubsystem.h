// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#pragma once

#include "Subsystems/EngineSubsystem.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "EktishafSubsystem.generated.h"

DECLARE_DELEGATE_FourParams(FEktishafOnResponseFast, bool, const TArray<uint8>, const FString, TSharedPtr<FJsonObject>);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FEktishafOnResponse, bool, success, const FString, content);

const FString HostUrl = "https://api.ektishaf.com/v1";
const FString RegisterUrl = HostUrl + "/register";
const FString LoginUrl = HostUrl + "/login";
const FString ExternalUrl = HostUrl + "/external";
const FString RevealUrl = HostUrl + "/reveal";
const FString BalanceUrl = HostUrl + "/balance";
const FString ABIUrl = HostUrl + "/abi";
const FString ReadUrl = HostUrl + "/read";
const FString WriteUrl = HostUrl + "/write";
const FString SignUrl = HostUrl + "/sign";
const FString VerifyUrl = HostUrl + "/verify";

static FString ConnectedAddress;
static FString CurrentTicket;

UCLASS(MinimalAPI)
class UEktishafSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();
	virtual bool ShouldCreateSubsystem(UObject* Outer) const;

public:

	EKTISHAF_API void SendRequest(const FEktishafOnResponseFast& Callback, const FString Url = HostUrl, const FString Payload = "", const FString Ticket = "", const FString Verb = "GET");
	EKTISHAF_API void GetRequest(const FEktishafOnResponseFast& Callback, const FString Url = HostUrl);
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
	EKTISHAF_API void Write(const FString Rpc, const FString Ticket, const FString Contract, const FString Abi, const FString Function, const TArray<TSharedPtr<FJsonValue>> Args, const FEktishafOnResponseFast& Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Host", Category = "Ektishaf")
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
};