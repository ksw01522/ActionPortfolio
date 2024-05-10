// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Camera/CameraActor.h"
#include "DialogueDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Dialogue", Config = "Game", defaultconfig)
class DIALOGUERUNTIME_API UDialogueDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UDialogueDeveloperSettings();

private:
	virtual FName GetContainerName() const override
	{
		return "Project";
	}

	virtual FName GetCategoryName() const override
	{
		return "Dialogue";
	}

	virtual FName GetSectionName() const override
	{
		return "Dialogue";
	}

private:
	UPROPERTY(config, EditDefaultsOnly, Category = "Camera")
	TSubclassOf<class ASceneCapture2D> SceneCaptureClass;

public:
	TSubclassOf<ASceneCapture2D> GetDialogueSceneCaptureClass() const { return SceneCaptureClass; }

private:
	UPROPERTY(config, EditDefaultsOnly, Category = "TextStyle", meta = (RequiredAssetDataTags = "RowStructure=/Script/UMG.RichTextStyleRow"))
	TSoftObjectPtr<class UDataTable> DialogueTextStyleSet;

public:
	UDataTable* GetDialogueTextStyleSet() const;
};
