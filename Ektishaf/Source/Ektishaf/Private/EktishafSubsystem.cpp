// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#include "EktishafSubsystem.h"
#include "PayloadBuilder.h"

void UEktishafSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("EktishafSubsystem initialized successfully."));
}

void UEktishafSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UEktishafSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UEktishafSubsystem::SendRequest(FString _url, FString _verb, FString _payload, FString _ticket, TFunctionRef<void(bool, const TArray<uint8>, const FString, FEktishafOnResponseFast, TSharedPtr<FJsonObject>)> _function, FEktishafOnResponseFast _callback)
{
	FHttpModule* httpModule = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = httpModule->CreateRequest();
#endif

	if (!_ticket.IsEmpty()) Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *_ticket));
	if (!_payload.IsEmpty()) Request->SetContentAsString(_payload);

	Request->SetHeader("Content-Type", "application/json");
	Request->SetURL(_url);
	Request->SetVerb(_verb);
	Request->ProcessRequest();
	Request->OnProcessRequestComplete().BindWeakLambda(this, [this, _url, _function, _callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const TArray<uint8> bytes = Response->GetContent();
			const FString content = Response->GetContentAsString();

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			bool success = bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode());
			bool hasJson = FJsonSerializer::Deserialize(Reader, JsonObject);

			_function(success, bytes, content, _callback, JsonObject);
		});
}

void UEktishafSubsystem::SendRequest(FString _url, FString _verb, FString _payload, FString _ticket, TFunctionRef<void(bool, const TArray<uint8>, const FString, FEktishafOnResponse, TSharedPtr<FJsonObject>)> _function, FEktishafOnResponse _callback)
{
	FHttpModule* httpModule = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = httpModule->CreateRequest();
#endif

	if (!_ticket.IsEmpty()) Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *_ticket));
	if (!_payload.IsEmpty()) Request->SetContentAsString(_payload);

	Request->SetHeader("Content-Type", "application/json");
	Request->SetURL(_url);
	Request->SetVerb(_verb);
	Request->ProcessRequest();
	Request->OnProcessRequestComplete().BindWeakLambda(this, [this, _url, _function, _callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const TArray<uint8> bytes = Response->GetContent();
			const FString content = Response->GetContentAsString();

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);

			bool success = bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode());
			bool hasJson = FJsonSerializer::Deserialize(Reader, JsonObject);

			_function(success, bytes, content, _callback, JsonObject);
		});
}

void UEktishafSubsystem::Host(FEktishafOnResponseFast Callback)
{
	SendRequest(HostUrl, "GET", "", "", [this](bool success, const TArray<uint8> bytes, const FString content, FEktishafOnResponseFast callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Host: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]() 
			{
				callback.ExecuteIfBound(success, content);
			});
		}, Callback);
}

void UEktishafSubsystem::Register(FString password, FEktishafOnResponseFast Callback)
{
	const FString payload = UPayloadBuilder::BuildPayload({ {"password", UPayloadBuilder::FStringToJsonValue(password)} });
	UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Register - Url: %s, Request: %s"), *RegisterUrl, *payload);
	SendRequest(RegisterUrl, "POST", payload, "", [this](bool success, const TArray<uint8> bytes, const FString content, FEktishafOnResponseFast callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Register: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					callback.ExecuteIfBound(success, content);
				});
		}, Callback);
}

void UEktishafSubsystem::Login(FString ticket, FString password, FEktishafOnResponseFast Callback)
{
	const FString payload = UPayloadBuilder::BuildPayload({ {"password", UPayloadBuilder::FStringToJsonValue(password)} });
	UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Login - Url: %s, Request: %s"), *LoginUrl, *payload);
	SendRequest(LoginUrl, "POST", payload, ticket, [this](bool success, const TArray<uint8> bytes, const FString content, FEktishafOnResponseFast callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Login: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					callback.ExecuteIfBound(success, content);
				});
		}, Callback);
}

void UEktishafSubsystem::External(FString privateKey, FString password, FEktishafOnResponseFast Callback)
{
	const FString payload = UPayloadBuilder::BuildPayload({ {"privateKey", UPayloadBuilder::FStringToJsonValue(privateKey)}, {"password", UPayloadBuilder::FStringToJsonValue(password)}});
	UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - External - Url: %s, Request: %s"), *ExternalUrl, *payload);
	SendRequest(ExternalUrl, "POST", payload, "", [this](bool success, const TArray<uint8> bytes, const FString content, FEktishafOnResponseFast callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - External: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					callback.ExecuteIfBound(success, content);
				});
		}, Callback);
}

void UEktishafSubsystem::Reveal(FString ticket, FString password, FEktishafOnResponseFast Callback)
{
	const FString payload = UPayloadBuilder::BuildPayload({ {"password", UPayloadBuilder::FStringToJsonValue(password)} });
	UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Reveal - Url: %s, Request: %s"), *RevealUrl, *payload);
	SendRequest(RevealUrl, "POST", payload, ticket, [this](bool success, const TArray<uint8> bytes, const FString content, FEktishafOnResponseFast callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Reveal: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					callback.ExecuteIfBound(success, content);
				});
		}, Callback);
}

void UEktishafSubsystem::Sign(FString ticket, FString message, FEktishafOnResponseFast Callback)
{
	const FString payload = UPayloadBuilder::BuildPayload({ {"message", UPayloadBuilder::FStringToJsonValue(message)} });
	UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Sign - Url: %s, Request: %s"), *SignUrl, *payload);
	SendRequest(SignUrl, "POST", payload, ticket, [this](bool success, const TArray<uint8> bytes, const FString content, FEktishafOnResponseFast callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Sign: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					callback.ExecuteIfBound(success, content);
				});
		}, Callback);
}

void UEktishafSubsystem::Verify(FString address, FString message, FString signature, FEktishafOnResponseFast Callback)
{
	const FString payload = UPayloadBuilder::BuildPayload({ 
	{"address", UPayloadBuilder::FStringToJsonValue(address)}, 
	{"message", UPayloadBuilder::FStringToJsonValue(message)}, 
	{"signature", UPayloadBuilder::FStringToJsonValue(signature)} });
	UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Verify - Url: %s, Request: %s"), *VerifyUrl, *payload);
	SendRequest(VerifyUrl, "POST", payload, "", [this](bool success, const TArray<uint8> bytes, const FString content, FEktishafOnResponseFast callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Verify: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					callback.ExecuteIfBound(success, content);
				});
		}, Callback);
}

void UEktishafSubsystem::Balance(FString rpc, FString ticket, FEktishafOnResponseFast Callback)
{
	const FString payload = UPayloadBuilder::BuildPayload({ {"rpc", UPayloadBuilder::FStringToJsonValue(rpc)}, {"ticket", UPayloadBuilder::FStringToJsonValue(ticket)} });
	UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Balance - Url: %s, Request: %s"), *BalanceUrl, *payload);
	SendRequest(BalanceUrl, "POST", payload, ticket, [this](bool success, const TArray<uint8> bytes, const FString content, FEktishafOnResponseFast callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Balance: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					callback.ExecuteIfBound(success, content);
				});
		}, Callback);
}

void UEktishafSubsystem::ABI(FString abi, bool minimal, FEktishafOnResponseFast Callback)
{
	const FString payload = UPayloadBuilder::BuildPayload({ {"abi", UPayloadBuilder::FStringToJsonValue(abi)}, {"minimal", UPayloadBuilder::BoolToJsonValue(minimal)} });
	UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - ABI - Url: %s, Request: %s"), *ABIUrl, *payload);
	SendRequest(ABIUrl, "POST", payload, "", [this](bool success, const TArray<uint8> bytes, const FString content, FEktishafOnResponseFast callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - ABI: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					callback.ExecuteIfBound(success, content);
				});
		}, Callback);
}

void UEktishafSubsystem::Read(FString rpc, FString ticket, FString contract, FString abi, FString function, TArray<TSharedPtr<FJsonValue>> args, FEktishafOnResponseFast Callback)
{
	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("rpc", rpc);
	builder->SetStringField("contract", contract);
	builder->SetStringField("abi", abi);
	builder->SetStringField("function", function);
	builder->SetArrayField("params", args);
	const FString payload = UPayloadBuilder::Build(builder);
	UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Read - Url: %s, Request: %s"), *ReadUrl, *payload);
	SendRequest(ReadUrl, "POST", payload, ticket, [this](bool success, const TArray<uint8> bytes, const FString content, FEktishafOnResponseFast callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Read: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					callback.ExecuteIfBound(success, content);
				});
		}, Callback);
}

void UEktishafSubsystem::Write(FString rpc, FString ticket, FString contract, FString abi, FString function, TArray<TSharedPtr<FJsonValue>> args, FEktishafOnResponseFast Callback)
{
	TSharedPtr<FJsonObject> builder = UPayloadBuilder::GetBuilder();
	builder->SetStringField("rpc", rpc);
	builder->SetStringField("contract", contract);
	builder->SetStringField("abi", abi);
	builder->SetStringField("function", function);
	builder->SetArrayField("params", args);
	const FString payload = UPayloadBuilder::Build(builder);
	UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Write - Url: %s, Request: %s"), *WriteUrl, *payload);
	SendRequest(WriteUrl, "POST", payload, ticket, [this](bool success, const TArray<uint8> bytes, const FString content, FEktishafOnResponseFast callback, TSharedPtr<FJsonObject> jsonObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - Write: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					callback.ExecuteIfBound(success, content);
				});
		}, Callback);
}

void UEktishafSubsystem::K2_Host(FEktishafOnResponse Callback)
{

	Host(FEktishafOnResponseFast::CreateLambda([Callback](bool success, const FString content)
	{ 
		UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - K2_Host: %s"), *content);
		AsyncTask(ENamedThreads::GameThread, [=]() 
		{
			Callback.ExecuteIfBound(success, content);
		});
	}));
}

void UEktishafSubsystem::K2_Register(FString password, FEktishafOnResponse Callback)
{
	Register(password, FEktishafOnResponseFast::CreateLambda([Callback](bool success, const FString content)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - K2_Register: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				Callback.ExecuteIfBound(success, content);
			});
		}));
}

void UEktishafSubsystem::K2_Login(FString ticket, FString password, FEktishafOnResponse Callback)
{
	Login(ticket, password, FEktishafOnResponseFast::CreateLambda([Callback](bool success, const FString content)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - K2_Login: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, content);
				});
		}));
}

void UEktishafSubsystem::K2_External(FString privateKey, FString password, FEktishafOnResponse Callback)
{
	External(privateKey, password, FEktishafOnResponseFast::CreateLambda([Callback](bool success, const FString content)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - K2_External: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, content);
				});
		}));
}

void UEktishafSubsystem::K2_Reveal(FString ticket, FString password, FEktishafOnResponse Callback)
{
	Reveal(ticket, password, FEktishafOnResponseFast::CreateLambda([Callback](bool success, const FString content)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - K2_Reveal: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, content);
				});
		}));
}

void UEktishafSubsystem::K2_Sign(FString ticket, FString message, FEktishafOnResponse Callback)
{
	Sign(ticket, message, FEktishafOnResponseFast::CreateLambda([Callback](bool success, const FString content)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - K2_Sign: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, content);
				});
		}));
}

void UEktishafSubsystem::K2_Verify(FString address, FString message, FString signature, FEktishafOnResponse Callback)
{
	Verify(address, message, signature, FEktishafOnResponseFast::CreateLambda([Callback](bool success, const FString content)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - K2_Verify: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, content);
				});
		}));
}

void UEktishafSubsystem::K2_Balance(FString rpc,FString ticket, FEktishafOnResponse Callback)
{
	Balance(rpc, ticket, FEktishafOnResponseFast::CreateLambda([Callback](bool success, const FString content)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - K2_Balance: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, content);
				});
		}));
}

void UEktishafSubsystem::K2_ABI(FString abi, bool minimal, FEktishafOnResponse Callback)
{
	ABI(abi, minimal, FEktishafOnResponseFast::CreateLambda([Callback](bool success, const FString content)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEktishafSubsystem - K2_ABI: %s"), *content);
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, content);
				});
		}));
}

FString UEktishafSubsystem::K2_ConnectedAddress()
{
	return ConnectedAddress;
}

FString UEktishafSubsystem::K2_Ticket()
{
	return Ticket;
}