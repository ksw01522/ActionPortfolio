// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Events/DialogueEvent.h"
#include "DialogueEvent_CallWorldEvent.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UDialogueEvent_CallWorldEvent : public UDialogueEvent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "DialogueEvent", meta = (AllowPrivateAccess = "true"))
	FString EventKey;


public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetEventKey() const {return EventKey;}

	virtual void OnCalledEvent_Implementation() override;

};
