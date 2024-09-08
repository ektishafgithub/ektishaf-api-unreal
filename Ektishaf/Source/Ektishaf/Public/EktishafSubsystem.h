// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#pragma once

#include "Subsystems/EngineSubsystem.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "EktishafSubsystem.generated.h"

DECLARE_DELEGATE_TwoParams(FEktishafOnResponseFast, bool, const FString);
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
static FString Ticket;

UCLASS(MinimalAPI)
class UEktishafSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();
	virtual bool ShouldCreateSubsystem(UObject* Outer) const;

	EKTISHAF_API void SendRequest(FString _url, FString _verb, FString _payload, FString ticket, TFunctionRef<void(bool, const TArray<uint8>, const FString, FEktishafOnResponseFast, TSharedPtr<FJsonObject>)> _function, FEktishafOnResponseFast _callback);
	EKTISHAF_API void SendRequest(FString _url, FString _verb, FString _payload, FString ticket, TFunctionRef<void(bool, const TArray<uint8>, const FString, FEktishafOnResponse, TSharedPtr<FJsonObject>)> _function, FEktishafOnResponse _callback);
	
EKTISHAF_API void Host(FEktishafOnResponseFast Callback);
	EKTISHAF_API void Register(FString password, FEktishafOnResponseFast Callback);
	EKTISHAF_API void Login(FString ticket, FString password, FEktishafOnResponseFast Callback);
	EKTISHAF_API void External(FString privateKey, FString password, FEktishafOnResponseFast Callback);
	EKTISHAF_API void Reveal(FString ticket, FString password, FEktishafOnResponseFast Callback);
	EKTISHAF_API void Sign(FString ticket, FString message, FEktishafOnResponseFast Callback);
	EKTISHAF_API void Verify(FString address, FString message, FString signature, FEktishafOnResponseFast Callback);
	EKTISHAF_API void Balance(FString rpc, FString ticket, FEktishafOnResponseFast Callback);
public:EKTISHAF_API void ABI(FString abi, bool minimal, FEktishafOnResponseFast Callback);
	EKTISHAF_API void Read(FString rpc, FString ticket, FString contract, FString abi, FString function, TArray<TSharedPtr<FJsonValue>> args, FEktishafOnResponseFast Callback);
	EKTISHAF_API void Write(FString rpc, FString ticket, FString contract, FString abi, FString function, TArray<TSharedPtr<FJsonValue>> args, FEktishafOnResponseFast Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Host", Category = "Ektishaf")
	EKTISHAF_API void K2_Host(FEktishafOnResponse Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Register", Category = "Ektishaf")
	EKTISHAF_API void K2_Register(FString password, FEktishafOnResponse Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Login", Category = "Ektishaf")
	EKTISHAF_API void K2_Login(FString ticket, FString password, FEktishafOnResponse Callback);

	UFUNCTION(BlueprintCallable, DisplayName="External", Category = "Ektishaf")
	EKTISHAF_API void K2_External(FString privateKey, FString password, FEktishafOnResponse Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Reveal", Category = "Ektishaf")
	EKTISHAF_API void K2_Reveal(FString ticket, FString password, FEktishafOnResponse Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Sign", Category = "Ektishaf")
	EKTISHAF_API void K2_Sign(FString ticket, FString message, FEktishafOnResponse Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Verify", Category = "Ektishaf")
	EKTISHAF_API void K2_Verify(FString address, FString message, FString signature, FEktishafOnResponse Callback);

	UFUNCTION(BlueprintCallable, DisplayName="Balance", Category = "Ektishaf")
	EKTISHAF_API void K2_Balance(FString rpc, FString ticket, FEktishafOnResponse Callback);

	UFUNCTION(BlueprintCallable, DisplayName="ABI", Category = "Ektishaf")
	EKTISHAF_API void K2_ABI(FString abi, bool minimal, FEktishafOnResponse Callback);

	UFUNCTION(BlueprintCallable, DisplayName="ConnectedAddress", Category = "Ektishaf")
	EKTISHAF_API FString K2_ConnectedAddress();

	UFUNCTION(BlueprintCallable, DisplayName="Ticket", Category = "Ektishaf")
	EKTISHAF_API FString K2_Ticket();
};