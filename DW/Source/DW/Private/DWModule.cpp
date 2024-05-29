// Copyright Epic Games, Inc. All Rights Reserved.

#include "DWModule.h"
#include "DWEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "DWModule"

void FDWModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FDWEditorModeCommands::Register();
}

void FDWModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FDWEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDWModule, DWEditorMode)