// Copyright Epic Games, Inc. All Rights Reserved.

#include "SlatePluginCommands.h"

#define LOCTEXT_NAMESPACE "FSlatePluginModule"

void FSlatePluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "SlatePlugin", "Bring up SlatePlugin window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
