// Copyright Epic Games, Inc. All Rights Reserved.

#include "SlatePlugin.h"
#include "SlatePluginStyle.h"
#include "SlatePluginCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "SlateWidgets/SSlateWebsite.h"

static const FName SlatePluginTabName("SlatePlugin");

#define LOCTEXT_NAMESPACE "FSlatePluginModule"

void FSlatePluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSlatePluginStyle::Initialize();
	FSlatePluginStyle::ReloadTextures();

	FSlatePluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSlatePluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSlatePluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSlatePluginModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SlatePluginTabName, FOnSpawnTab::CreateRaw(this, &FSlatePluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSlatePluginTabTitle", "SlatePlugin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FSlatePluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSlatePluginStyle::Shutdown();

	FSlatePluginCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SlatePluginTabName);
}

TSharedRef<SDockTab> FSlatePluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	//FText WidgetText = FText::Format(
	//	LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
	//	FText::FromString(TEXT("FSlatePluginModule::OnSpawnPluginTab")),
	//	FText::FromString(TEXT("SlatePlugin.cpp"))
	//	);

	//return SNew(SDockTab)
	//	.TabRole(ETabRole::NomadTab)
	//	[
	//		// Put your tab content here!
	//		SNew(SBox)
	//		.HAlign(HAlign_Center)
	//		.VAlign(VAlign_Center)
	//		[
	//			SNew(STextBlock)
	//			.Text(WidgetText)
	//		]
	//	];
	return
		SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SNew(SSlateWebsite)

		];
}

void FSlatePluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SlatePluginTabName);
}

void FSlatePluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSlatePluginCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSlatePluginCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSlatePluginModule, SlatePlugin)