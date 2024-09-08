// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#include "WindowManager.h"
#include <Widgets/SWindow.h>
#include <Widgets/SEktishafEditorWindow.h>

TSharedPtr<SWindow> WindowManager::GetWindow()
{
	if (!Window.IsValid()) 
	{
		Window = SNew(SWindow)
		.Title(FText::FromString("Ektishaf - ABI Converter"))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		.HasCloseButton(true)
		.ClientSize(FVector2D(900.f, 600.f))
		.SizingRule(ESizingRule::FixedSize)
		.AutoCenter(EAutoCenter::PreferredWorkArea)
		.ScreenPosition(FVector2D(0, 0))
		.LayoutBorder(FMargin(3.f))
		[
			SAssignNew(RootWidget, SEktishafEditorWindow)
		];
		Window->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& WindowRef) 
		{
			RootWidget->UnloadResources();
			RootWidget = nullptr;
			Window = nullptr;
		}));
		FSlateApplication::Get().AddWindow(Window.ToSharedRef(), false);
	}
	Window->BringToFront(true);
	return Window;
}

FPropertyEditorModule& WindowManager::GetPropertyModule()
{
	return FModuleManager::GetModuleChecked<FPropertyEditorModule>(PROPERTY_EDITOR);
}

void WindowManager::CloseWindow() 
{	
	if (Window.IsValid())
	{
		Window->RequestDestroyWindow();
	}
}

void WindowManager::RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate)
{
	check(ClassName != NAME_None);
	RegisteredClassNames.Add(ClassName);
	GetPropertyModule().RegisterCustomClassLayout(ClassName, DetailLayoutDelegate);
}

void WindowManager::RegisterObjectCustomizations() 
{
}

void WindowManager::UnregisterObjectCustomizations() 
{
	if (FModuleManager::Get().IsModuleLoaded(PROPERTY_EDITOR)) 
	{
		FPropertyEditorModule& PropertyModule = GetPropertyModule();
		for (auto registry = RegisteredClassNames.CreateConstIterator(); registry; ++registry) 
		{
			if (registry->IsValid())
			{
				PropertyModule.UnregisterCustomClassLayout(*registry);
			}
		}
		PropertyModule.NotifyCustomizationModuleChanged();
	}
}