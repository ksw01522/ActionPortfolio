// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueDeveloperSettings.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/DataTable.h"

UDialogueDeveloperSettings::UDialogueDeveloperSettings()
{
	SceneCaptureClass = ASceneCapture2D::StaticClass();
}

UDataTable* UDialogueDeveloperSettings::GetDialogueTextStyleSet() const
{
	return DialogueTextStyleSet.LoadSynchronous();
}


