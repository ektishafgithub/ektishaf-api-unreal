// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#pragma once

#include "CoreMinimal.h"
#include "PropertyEditorModule.h"

const FName PROPERTY_EDITOR("PropertyEditor");

class WindowManager
{

public:

	static WindowManager& Get()
	{
		static WindowManager INSTANCE;
		return INSTANCE;
	}

private:
	WindowManager() {}

public:
	WindowManager(WindowManager const&) = delete;
	void operator=(WindowManager const&) = delete;

private:

	TSharedPtr<class SWindow> Window;

public:

	TSharedPtr<class SEktishafEditorWindow> RootWidget;
	TSharedPtr<class SWindow> GetWindow();
	FPropertyEditorModule& GetPropertyModule();
	void CloseWindow();

private:

	TSet<FName> RegisteredClassNames;
	void RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate);

public:

	void RegisterObjectCustomizations();
	void UnregisterObjectCustomizations();
};