// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogueStructs.h"
#include "DialogueManager.h"
#include "GameplayTagContainer.h"
#include "DialogueEvent.generated.h"

/**
 * 
 */
 class UDialoguerComponent;

 UENUM(BlueprintType)
 enum class EDialougeEventInstancingPolicy : uint8
 {
	NotInstanced,
	InstancedPerExecution
 };

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced, AutoExpandCategories = ("DialogueEvent"))
class DIALOGUERUNTIME_API UDialogueEvent : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogueEvent")
	FGameplayTag EventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogueEvent")
	EDialougeEventInstancingPolicy InstancingPolicy;

protected:
	FDialogueHandle CalledDialogueID;

public:
	void CallEvent(const FDialogueHandle& Handle);
	void CallEndEvent(bool bIsCancelled) { OnEndEvent(bIsCancelled); }

	UFUNCTION(BlueprintNativeEvent, Category = "DialogueEvent")
	bool CanEnterNextNode();

	UFUNCTION(BlueprintNativeEvent, Category = "DialogueEvent")
	void OnCalledEvent(const FDialogueHandle& Handle);

	UFUNCTION(BlueprintNativeEvent, Category = "DialogueEvent")
	void OnEndEvent(bool bIsCancelled);

protected:
	virtual bool CanEnterNextNode_Implementation() { return true; }

	virtual void OnCalledEvent_Implementation(FDialogueHandle& Handle) {}

	virtual void OnEndEvent_Implementation(bool bIsCancelled) {}
};
