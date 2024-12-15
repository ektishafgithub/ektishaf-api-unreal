// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EktishafEditorStyle.h"

class FEktishafEditorCommands : public TCommands<FEktishafEditorCommands>
{
public:

	FEktishafEditorCommands()
		: TCommands<FEktishafEditorCommands>(TEXT("EktishafEditor"), NSLOCTEXT("Contexts", "EktishafEditor", "EktishafEditor Plugin"), NAME_None, FEktishafEditorStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
	TSharedPtr< FUICommandInfo > GenerateNewAccounts;
};