// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SlatePluginStyle.h"

class FSlatePluginCommands : public TCommands<FSlatePluginCommands>
{
public:

	FSlatePluginCommands()
		: TCommands<FSlatePluginCommands>(TEXT("SlatePlugin"), NSLOCTEXT("Contexts", "SlatePlugin", "SlatePlugin Plugin"), NAME_None, FSlatePluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};