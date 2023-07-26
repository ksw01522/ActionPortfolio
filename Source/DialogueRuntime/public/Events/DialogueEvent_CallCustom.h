// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Events/DialogueEvent.h"
#include "DialogueEvent_CallCustom.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUERUNTIME_API UDialogueEvent_CallCustom : public UDialogueEvent
{
	GENERATED_BODY()
	
	UDialogueEvent_CallCustom();

private:
	UPROPERTY(EditAnywhere, Category = "Event")
	int EventID;

protected:
	virtual void OnCalledEvent_Implementation();

};
