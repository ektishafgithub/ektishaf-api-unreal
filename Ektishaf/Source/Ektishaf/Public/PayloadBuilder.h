// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#pragma once

#include "CoreMinimal.h"

class EKTISHAF_API UPayloadBuilder : public UObject
{
public:
	static TSharedPtr<FJsonObject> GetBuilder();
	static FString Build(TSharedPtr<FJsonObject> builder);
	static TSharedPtr<FJsonValueBoolean> BoolToJsonValue(bool _value);
	static TSharedPtr<FJsonValueNumber> IntToJsonValue(int _value);
	static TSharedPtr<FJsonValueNumber> LongToJsonValue(long _value);
	static TSharedPtr<FJsonValueNumber> FloatToJsonValue(float _value);
	static TSharedPtr<FJsonValueNumber> DoubleToJsonValue(double _value);
	static TSharedPtr<FJsonValueString> FStringToJsonValue(FString _value);
	static void AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, bool value);
	static void AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, int value);
	static void AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, long value);
	static void AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, float value);
	static void AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, double value);
	static void AddArrayItem(TArray<TSharedPtr<FJsonValue>>& _array, FString value);
	static void AddNestedArray(TArray<TSharedPtr<FJsonValue>>& _array, TArray<TSharedPtr<FJsonValue>> _nestedArray);
	static void AddNestedObject(TArray<TSharedPtr<FJsonValue>>& _array, TSharedPtr<FJsonObject> _nestedObject);
	static FString BuildPayload(TMap<FString, TSharedPtr<FJsonValue>> _map);
};