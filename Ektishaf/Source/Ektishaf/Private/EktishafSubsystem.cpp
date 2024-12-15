// Copyright (C) 2024 Ektishaf. All Rights Reserved. <https://www.ektishaf.com>

#include "EktishafSubsystem.h"
#include "PayloadBuilder.h"
#include "Contracts/EktishafNftCollection.h"
#include "../Widget/NftItem.h"

void UEktishafSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Config = GetMutableDefault<UBlockchainSettings>();
	CurrentNetwork = Config->Networks[0];
	Log(FString::Printf(TEXT("EktishafSubsystem initialized successfully.")));
	Host(nullptr);
}

void UEktishafSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UEktishafSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UEktishafSubsystem::SendRequest(const FEktishafOnResponseFast& Callback, const FString Url, const FString Payload, const FString Ticket, const FString Verb)
{
	FHttpModule* httpModule = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = httpModule->CreateRequest();
#endif

	if (!Ticket.IsEmpty()) Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *Ticket));
	if (!Payload.IsEmpty()) Request->SetContentAsString(Payload);

	Request->SetHeader("Content-Type", "application/json");
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->ProcessRequest();
	Log(FString::Printf(TEXT("UEktishafSubsystem - %s/Request - Url: %s, Payload: %s"), *Verb, *Url, *Payload));
	Request->OnProcessRequestComplete().BindWeakLambda(this, [&, Callback, Url, Verb, Payload](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			const TArray<uint8> bytes = Response->GetContent();
			const FString content = Response->GetContentAsString();

			bool success = bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode());
			Log(FString::Printf(TEXT("UEktishafSubsystem - %s/Response - Url: %s, Content: %s"), *Verb, *Url, *content));

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(content);
			FJsonSerializer::Deserialize(Reader, JsonObject);

			Callback.ExecuteIfBound(success, bytes, content, JsonObject);
		});
}

EKTISHAF_API void UEktishafSubsystem::GetRequest(const FEktishafOnResponseFast& Callback, const FString Url)
{
	SendRequest(Callback, Url);
}

EKTISHAF_API void UEktishafSubsystem::PostRequest(const FEktishafOnResponseFast& Callback, const FString Url, const FString Payload, const FString Ticket)
{
	SendRequest(Callback, Url, Payload, Ticket, "POST");
}

void UEktishafSubsystem::Host(const FEktishafOnResponseFast& Callback)
{
	GetRequest(Callback, Config->Op(EServOp::None));
}

void UEktishafSubsystem::Register(const FString Password, const FEktishafOnResponseFast& Callback)
{
	const FString Payload = UPayloadBuilder::BuildPayload({ {"password", UPayloadBuilder::FStringToJsonValue(Password)} });
	PostRequest(Callback, Config->Op(EServOp::Register), Payload);
}

void UEktishafSubsystem::Login(const FString Ticket, const FString Password, const FEktishafOnResponseFast& Callback)
{
	const FString Payload = UPayloadBuilder::BuildPayload({ {"password", UPayloadBuilder::FStringToJsonValue(Password)} });
	PostRequest(Callback, Config->Op(EServOp::Login), Payload, Ticket);
}

void UEktishafSubsystem::External(const FString PrivateKey, const FString Password, const FEktishafOnResponseFast& Callback)
{
	const FString Payload = UPayloadBuilder::BuildPayload({ {"privateKey", UPayloadBuilder::FStringToJsonValue(PrivateKey)}, {"password", UPayloadBuilder::FStringToJsonValue(Password)}});
	PostRequest(Callback, Config->Op(EServOp::External), Payload);
}

void UEktishafSubsystem::Reveal(const FString Ticket, const FString Password, const FEktishafOnResponseFast& Callback)
{
	const FString Payload = UPayloadBuilder::BuildPayload({ {"password", UPayloadBuilder::FStringToJsonValue(Password)} });
	PostRequest(Callback, Config->Op(EServOp::Reveal), Payload, Ticket);
}

void UEktishafSubsystem::Sign(const FString Ticket, const FString Message, const FEktishafOnResponseFast& Callback)
{
	const FString Payload = UPayloadBuilder::BuildPayload({ {"message", UPayloadBuilder::FStringToJsonValue(Message)} });
	PostRequest(Callback, Config->Op(EServOp::Sign), Payload, Ticket);
}

void UEktishafSubsystem::Verify(const FString Address, const FString Message, const FString Signature, const FEktishafOnResponseFast& Callback)
{
	const FString Payload = UPayloadBuilder::BuildPayload({
	{"address", UPayloadBuilder::FStringToJsonValue(Address)},
	{"message", UPayloadBuilder::FStringToJsonValue(Message)},
	{"signature", UPayloadBuilder::FStringToJsonValue(Signature)} });
	PostRequest(Callback, Config->Op(EServOp::Verify), Payload);
}

void UEktishafSubsystem::Balance(const FString Rpc, const FString Address, const FEktishafOnResponseFast& Callback)
{
	const FString Payload = UPayloadBuilder::BuildPayload({ {"rpc", UPayloadBuilder::FStringToJsonValue(Rpc)}, {"address", UPayloadBuilder::FStringToJsonValue(Address)} });
	PostRequest(Callback, Config->Op(EServOp::Balance), Payload);
}

void UEktishafSubsystem::ABI(const FString Abi, const bool Minimal, const FEktishafOnResponseFast& Callback)
{
	const FString Payload = UPayloadBuilder::BuildPayload({ {"abi", UPayloadBuilder::FStringToJsonValue(Abi)}, {"minimal", UPayloadBuilder::BoolToJsonValue(Minimal)} });
	PostRequest(Callback, Config->Op(EServOp::ABI), Payload);
}

void UEktishafSubsystem::Read(const FString Rpc, const FString Ticket, const FString Contract, const FString Abi, const FString Function, const TArray<TSharedPtr<FJsonValue>> Args, const FEktishafOnResponseFast& Callback)
{
	TSharedPtr<FJsonObject> Builder = UPayloadBuilder::GetBuilder();
	Builder->SetStringField("rpc", Rpc);
	Builder->SetStringField("contract", Contract);
	Builder->SetStringField("abi", Abi);
	Builder->SetStringField("function", Function);
	Builder->SetArrayField("params", Args);
	const FString Payload = UPayloadBuilder::Build(Builder);
	PostRequest(Callback, Config->Op(EServOp::Read), Payload, Ticket);
}

EKTISHAF_API void UEktishafSubsystem::Read(const FString Rpc, const FString Ticket, const FString Contract, const FString FuncSig, const TArray<TSharedPtr<FJsonValue>> Args, const FEktishafOnResponseFast& Callback)
{
	TSharedPtr<FJsonObject> Builder = UPayloadBuilder::GetBuilder();
	Builder->SetStringField("rpc", Rpc);
	Builder->SetStringField("contract", Contract);
	Builder->SetStringField("abi", ExtractFunctionABI(FuncSig));
	Builder->SetStringField("function", ExtractFunctionName(FuncSig));
	Builder->SetArrayField("params", Args);
	const FString Payload = UPayloadBuilder::Build(Builder);
	PostRequest(Callback, Config->Op(EServOp::Read), Payload, Ticket);
}

void UEktishafSubsystem::Write(const FString Rpc, const FString Ticket, const FString Contract, const FString Abi, const FString Function, const TArray<TSharedPtr<FJsonValue>> Args, const FEktishafOnResponseFast& Callback)
{
	TSharedPtr<FJsonObject> Builder = UPayloadBuilder::GetBuilder();
	Builder->SetStringField("rpc", Rpc);
	Builder->SetStringField("contract", Contract);
	Builder->SetStringField("abi", Abi);
	Builder->SetStringField("function", Function);
	Builder->SetArrayField("params", Args);
	const FString Payload = UPayloadBuilder::Build(Builder);
	PostRequest(Callback, Config->Op(EServOp::Write), Payload, Ticket);
}

EKTISHAF_API void UEktishafSubsystem::Write(const FString Rpc, const FString Ticket, const FString Contract, const FString FuncSig, const TArray<TSharedPtr<FJsonValue>> Args, const FEktishafOnResponseFast& Callback)
{
	TSharedPtr<FJsonObject> Builder = UPayloadBuilder::GetBuilder();
	Builder->SetStringField("rpc", Rpc);
	Builder->SetStringField("contract", Contract);
	Builder->SetStringField("abi", ExtractFunctionABI(FuncSig));
	Builder->SetStringField("function", ExtractFunctionName(FuncSig));
	Builder->SetArrayField("params", Args);
	const FString Payload = UPayloadBuilder::Build(Builder);
	PostRequest(Callback, Config->Op(EServOp::Write), Payload, Ticket);
}

void UEktishafSubsystem::GetNfts(const FEktishafOnGetNftsFast& Callback)
{
	TArray<TSharedPtr<FJsonValue>> Args;
	Read(CurrentNetwork.Rpc, CurrentAccount.Ticket, EktishafNftCollection::Address, EktishafNftCollection::getNfts_0_, Args, FEktishafOnResponseFast::CreateLambda([this, Callback](bool Success, const TArray<uint8> Bytes, const FString Content, TSharedPtr<FJsonObject> JsonObject)
	{
		TArray<TSharedPtr<FJsonValue>> ValueArray = JsonObject->GetArrayField(TEXT("data"));
		TArray<TArray<FString>> nfts;
		for (auto& entry : ValueArray)
		{
			TArray<TSharedPtr<FJsonValue>> ItemArray = entry->AsArray();
			TArray<FString> StringArray;
			StringArray.Add(ItemArray[0]->AsString());
			StringArray.Add(ItemArray[1]->AsString());
			StringArray.Add(ItemArray[2]->AsString());
			nfts.Add(StringArray);
		}
		Callback.ExecuteIfBound(nfts);
	}));
}

void UEktishafSubsystem::Accounts(int Registers, const FString Password, const FEktishafOnResponseFast& Callback)
{
	const FString Payload = UPayloadBuilder::BuildPayload({ {"registers", UPayloadBuilder::IntToJsonValue(Registers)}, {"password", UPayloadBuilder::FStringToJsonValue(Password)} });
	PostRequest(Callback, Config->Op(EServOp::Accounts), Payload);
}

void UEktishafSubsystem::Send(const FString Rpc, const FString To, const FString Amount, const FString Ticket, const FEktishafOnResponseFast& Callback)
{
	const FString Payload = UPayloadBuilder::BuildPayload({
	{"rpc", UPayloadBuilder::FStringToJsonValue(Rpc)},
	{"to", UPayloadBuilder::FStringToJsonValue(To)},
	{"amount", UPayloadBuilder::FStringToJsonValue(Amount)} });
	PostRequest(Callback, Config->Op(EServOp::Send), Payload, Ticket);
}

void UEktishafSubsystem::K2_Host(const FEktishafOnResponse& Callback)
{

	Host(FEktishafOnResponseFast::CreateLambda([this, Callback](bool success, const TArray<uint8>, const FString content, TSharedPtr<FJsonObject> JsonObject)
	{ 
		Log(FString::Printf(TEXT("UEktishafSubsystem - K2_Host: %s"), *content));
		AsyncTask(ENamedThreads::GameThread, [=]() 
		{
			Callback.ExecuteIfBound(success, content);
		});
	}));
}

void UEktishafSubsystem::K2_Register(const FString Password, const FEktishafOnResponse& Callback)
{
	Register(Password, FEktishafOnResponseFast::CreateWeakLambda(this, [this, Callback](bool success, const TArray<uint8>, const FString content, TSharedPtr<FJsonObject> JsonObject)
		{
			Log(FString::Printf(TEXT("UEktishafSubsystem - K2_Register: %s"), *content));

			if (success && JsonObject.IsValid())
			{
				CurrentAccount.WalletAddress = JsonObject->GetStringField(TEXT("address"));
				CurrentAccount.Ticket = JsonObject->GetStringField(TEXT("ticket"));
			}

			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				Callback.ExecuteIfBound(success, content);
			});
		}));
}

void UEktishafSubsystem::K2_Login(const FString Ticket, const FString Password, const FEktishafOnResponse& Callback)
{
	Login(Ticket, Password, FEktishafOnResponseFast::CreateLambda([this, Callback](bool success, const TArray<uint8>, const FString content, TSharedPtr<FJsonObject> JsonObject)
		{
			Log(FString::Printf(TEXT("UEktishafSubsystem - K2_Login: %s"), *content));

			if (success && JsonObject.IsValid())
			{
				CurrentAccount.WalletAddress = JsonObject->GetStringField(TEXT("address"));
				CurrentAccount.Ticket = JsonObject->GetStringField(TEXT("ticket"));
			}

			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, content);
				});
		}));
}

void UEktishafSubsystem::K2_External(const FString PrivateKey, const FString Password, const FEktishafOnResponse& Callback)
{
	External(PrivateKey, Password, FEktishafOnResponseFast::CreateLambda([this, Callback](bool success, const TArray<uint8>, const FString content, TSharedPtr<FJsonObject> JsonObject)
		{
			Log(FString::Printf(TEXT("UEktishafSubsystem - K2_External: %s"), *content));

			if (success && JsonObject.IsValid())
			{
				CurrentAccount.WalletAddress = JsonObject->GetStringField(TEXT("address"));
				CurrentAccount.Ticket = JsonObject->GetStringField(TEXT("ticket"));
			}

			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, content);
				});
		}));
}

void UEktishafSubsystem::K2_Reveal(const FString Ticket, const FString Password, const FEktishafOnResponse& Callback)
{
	Reveal(Ticket, Password, FEktishafOnResponseFast::CreateLambda([this, Callback](bool success, const TArray<uint8>, const FString content, TSharedPtr<FJsonObject> JsonObject)
		{
			Log(FString::Printf(TEXT("UEktishafSubsystem - K2_Reveal: %s"), *content));
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, content);
				});
		}));
}

void UEktishafSubsystem::K2_Sign(const FString Ticket, const FString Message, const FEktishafOnResponse& Callback)
{
	Sign(Ticket, Message, FEktishafOnResponseFast::CreateLambda([this, Callback](bool success, const TArray<uint8>, const FString content, TSharedPtr<FJsonObject> JsonObject)
		{
			Log(FString::Printf(TEXT("UEktishafSubsystem - K2_Sign: %s"), *content));
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, content);
				});
		}));
}

void UEktishafSubsystem::K2_Verify(const FString Address, const FString Message, const FString Signature, const FEktishafOnResponse& Callback)
{
	Verify(Address, Message, Signature, FEktishafOnResponseFast::CreateLambda([this, Callback](bool success, const TArray<uint8>, const FString content, TSharedPtr<FJsonObject> JsonObject)
		{
			Log(FString::Printf(TEXT("UEktishafSubsystem - K2_Verify: %s"), *content));
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, content);
				});
		}));
}

void UEktishafSubsystem::K2_Balance(const FString Rpc, const FString Address, const FEktishafOnResponse& Callback)
{
	Balance(Rpc, Address, FEktishafOnResponseFast::CreateLambda([this, Callback](bool success, const TArray<uint8>, const FString content, TSharedPtr<FJsonObject> JsonObject)
		{
			Log(FString::Printf(TEXT("UEktishafSubsystem - K2_Balance: %s"), *content));
			FString data = JsonObject->GetStringField(TEXT("data"));

			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, data);
				});
		}));
}

void UEktishafSubsystem::K2_ABI(const FString Abi, const bool Minimal, const FEktishafOnResponse& Callback)
{
	ABI(Abi, Minimal, FEktishafOnResponseFast::CreateLambda([this, Callback](bool success, const TArray<uint8>, const FString content, TSharedPtr<FJsonObject> JsonObject)
		{
			Log(FString::Printf(TEXT("UEktishafSubsystem - K2_ABI: %s"), *content));
			AsyncTask(ENamedThreads::GameThread, [=]()
				{
					Callback.ExecuteIfBound(success, content);
				});
		}));
}

FEktishafAccount UEktishafSubsystem::K2_GetCurrentAccount()
{
	return CurrentAccount;
}

void UEktishafSubsystem::K2_GetNfts(const FEktishafOnGetNfts& Callback)
{
	TArray<TSharedPtr<FJsonValue>> Args;
	Read(CurrentNetwork.Rpc, CurrentAccount.Ticket, EktishafNftCollection::Address, FString::Printf(TEXT("[\"%s\"]"), *EktishafNftCollection::getNfts_0_), "getNfts", Args, FEktishafOnResponseFast::CreateLambda([this, Callback](bool Success, const TArray<uint8> Bytes, const FString Content, TSharedPtr<FJsonObject> JsonObject)
	{
		TArray<TSharedPtr<FJsonValue>> ValueArray = JsonObject->GetArrayField(TEXT("data"));
		TArray<FEktishafNft> Nfts;
		for (int i = 0; i < ValueArray.Num(); i++)
		{
			TSharedPtr<FJsonValue> Value = ValueArray[i];
			TArray<TSharedPtr<FJsonValue>> ItemArray = Value->AsArray();

			FEktishafNft Nft;
			Nft.Id = ItemArray[0]->AsNumber();
			Nft.Amount = ItemArray[1]->AsNumber();
			Nft.Uri = ItemArray[2]->AsString();
			Nfts.Add(Nft);
		}
		Callback.ExecuteIfBound(Nfts);
	}));
}

void UEktishafSubsystem::K2_MintBatch(const FString To, TArray<int> Ids, TArray<int> Amounts, TArray<FString> Uris, const FEktishafOnResponse& Callback)
{
	TArray<TSharedPtr<FJsonValue>> Args;
	UPayloadBuilder::AddArrayItem(Args, To);
	UPayloadBuilder::AddNestedArray(Args, Ids);
	UPayloadBuilder::AddNestedArray(Args, Amounts);
	UPayloadBuilder::AddNestedArray(Args, Uris);

	Write(CurrentNetwork.Rpc, CurrentAccount.Ticket, EktishafNftCollection::Address, FString::Printf(TEXT("[\"%s\"]"), *EktishafNftCollection::mintBatch_4_address_uint256Array_uint256Array_stringArray), "mintBatch", Args, FEktishafOnResponseFast::CreateLambda([this, Callback](bool Success, const TArray<uint8> Bytes, const FString Content, TSharedPtr<FJsonObject> JsonObject)
	{
		Callback.ExecuteIfBound(Success, Content);
	}));
}

FString UEktishafSubsystem::ExtractFunctionABI(FString FuncSig)
{
	return FString::Printf(TEXT("[\"%s\"]"), *FuncSig.TrimChar('[').TrimChar(']').TrimQuotes());
}

FString UEktishafSubsystem::ExtractFunctionName(FString FuncSig)
{
	FString trimmed = FuncSig.TrimChar('[').TrimChar(']').TrimQuotes();
	TArray<FString> splits;
	trimmed.ParseIntoArray(splits, TEXT(" "), false);
	FString func = splits[1];
	int32 index = func.Find("(", ESearchCase::IgnoreCase, ESearchDir::FromStart, 0);
	return func.Mid(0, index);
}

FEktishafAccount UEktishafSubsystem::GetAccount()
{
	return CurrentAccount;
}

void UEktishafSubsystem::SetAccount(const FString Address, const FString Ticket)
{
	CurrentAccount.WalletAddress = Address;
	CurrentAccount.Ticket = Ticket;
}

void UEktishafSubsystem::Log(FString Message)
{
	if (GEngine)
	{
		if (Config && Config->ShowLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message)
		}
	}
}