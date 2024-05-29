// Copyright Epic Games, Inc. All Rights Reserved.

#include "DWEditorModeToolkit.h"
#include "DWEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "DWEditorModeToolkit"

FDWEditorModeToolkit::FDWEditorModeToolkit()
{
}

void FDWEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FDWEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FDWEditorModeToolkit::GetToolkitFName() const
{
	return FName("DWEditorMode");
} 

FText FDWEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "DWEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
