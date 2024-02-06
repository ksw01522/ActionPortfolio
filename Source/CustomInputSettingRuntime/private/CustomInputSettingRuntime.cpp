// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomInputSettingRuntime.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(CustomInputSettingRuntime);

IMPLEMENT_MODULE( FCustomInputSettingRuntime, CustomInputSettingRuntime);

void FCustomInputSettingRuntime::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FCustomInputSettingRuntime::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}