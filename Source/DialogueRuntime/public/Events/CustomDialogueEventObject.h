// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogueStructs.h"
#include "DialogueManager.h"
#include "CustomDialogueEventObject.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class DIALOGUERUNTIME_API UCustomDialogueEventObject : public UObject
{
	GENERATED_BODY()
	

	friend UDialogueManager;

protected:
	virtual void BeginDestroy() override;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DialogueEvent")
	void OnCalledCustomEvent();

	void OnCalledCustomEvent_Implementation() {};

};
