// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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


public:
	void CallEvent();
	void CallEndEvent(bool bIsCancelled);

	UFUNCTION(BlueprintNativeEvent, Category = "DialogueEvent")
	bool CanEnterNextNode() const;

	UDialogueEvent* GetEventForCall();

	EDialougeEventInstancingPolicy GetInstancingPolicy() const { return InstancingPolicy; }

public:
	UFUNCTION(BlueprintNativeEvent, Category = "DialogueEvent")
	void OnCalledEvent();

	UFUNCTION(BlueprintNativeEvent, Category = "DialogueEvent")
	void OnEndEvent(bool bIsCancelled);

protected:
	virtual bool CanEnterNextNode_Implementation() const { return true; }

	virtual void OnCalledEvent_Implementation() {}

	virtual void OnEndEvent_Implementation(bool bIsCancelled) {}
};
