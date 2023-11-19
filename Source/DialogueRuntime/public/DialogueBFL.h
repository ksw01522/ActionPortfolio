// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DialogueBFL.generated.h"

/**
 * 
 */

class UDialogueNode;
class UDialogueNode_Basic;
class UDialogueNode_Question;
class UDialogueNode_Answer;

class UDialogueManager;
class UDialoguerComponent;

UCLASS()
class DIALOGUERUNTIME_API UDialogueBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueBFL|Manager")
	static UDialogueManager* GetDialogueManager();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueBFL|Manager", meta = (WorldContext = "WorldContextObject"))
	static UDialogueManager* TestGetManager(UObject* WorldContextObject);
};
