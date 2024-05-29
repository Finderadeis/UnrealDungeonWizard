// Copyright Epic Games, Inc. All Rights Reserved.

#include "DWEditorMode.h"
#include "DWEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "DWEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "DWBaseTool.h"

// step 2: register a ToolBuilder in FDWEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "DWEditorMode"

const FEditorModeID UDWEditorMode::EM_DWEditorModeId = TEXT("EM_DWEditorMode");

FString UDWEditorMode::BaseToolName = TEXT("DW_BaseTool");


UDWEditorMode::UDWEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UDWEditorMode::EM_DWEditorModeId,
		LOCTEXT("ModeName", "Dungeon Wizard"),
		FSlateIcon(),
		true);
}


UDWEditorMode::~UDWEditorMode()
{
}


void UDWEditorMode::ActorSelectionChangeNotify()
{
}

void UDWEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FDWEditorModeCommands& SampleToolCommands = FDWEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.BaseTool, BaseToolName, NewObject<UDWBaseToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, BaseToolName);
}

void UDWEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FDWEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UDWEditorMode::GetModeCommands() const
{
	return FDWEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
