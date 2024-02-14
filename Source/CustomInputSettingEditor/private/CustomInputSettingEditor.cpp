// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomInputSettingEditor.h"
#include "Modules/ModuleManager.h"
#include "CIDSDetailCustomization.h"
#include "CustomInputDeveloperSettings.h"

DEFINE_LOG_CATEGORY(CustomInputSettingEditor);

IMPLEMENT_MODULE( FCustomInputSettingEditor, CustomInputSettingEditor);

void FCustomInputSettingEditor::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomClassLayout(UCustomInputDeveloperSettings::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FCIDSDetailCustomization::MakeInstance));
}


void FCustomInputSettingEditor::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomClassLayout(UCustomInputDeveloperSettings::StaticClass()->GetFName());
}