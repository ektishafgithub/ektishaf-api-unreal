// Copyright (C) 2024 Ektishaf.  All Rights Reserved. <https://www.ektishaf.com>

#include "EktishafEditorCommands.h"

#define LOCTEXT_NAMESPACE "FEktishafEditorModule"

void FEktishafEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "ABI Converter", "Converts ABI to human readable ABI", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
