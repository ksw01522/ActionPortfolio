// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NPC/NonePlayerCharacter.h"
#include "Interaction/InteractionSystemInterface.h"
#include "InteractableNPC.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API AInteractableNPC : public ANonePlayerCharacter, public IInteractionSystemInterface
{
	GENERATED_BODY()
public:
	AInteractableNPC();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	class UInteractionSystemComponent_NPC* InteractionSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	class UDialoguerComponent* DialoguerComponet;


public:
	virtual UInteractionSystemComponent* GetInteractionSystemComponent() override;
	virtual void PostInitializeComponents() override;
	

	
};