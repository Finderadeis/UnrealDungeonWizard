// Copyright Epic Games, Inc. All Rights Reserved.

#include "DWEditorModeCommands.h"
#include "DWEditorMode.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "DWEditorModeCommands"

FDWEditorModeCommands::FDWEditorModeCommands()
	: TCommands<FDWEditorModeCommands>("DWEditorMode",
		NSLOCTEXT("DWEditorMode", "DWEditorModeCommands", "DW Editor Mode"),
		NAME_None,
		FEditorStyle::GetStyleSetName())
{
}

void FDWEditorModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);

	UI_COMMAND(BaseTool, "Dungeon Wizard Base Tool", "Start generating dungeon", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(BaseTool);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FDWEditorModeCommands::GetCommands()
{
	return FDWEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
