// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/InteractableNPC.h"
#include "Interaction/InteractionType/InteractionSystemComponent_NPC.h"
#include "Components/CapsuleComponent.h"
#include "DialoguerComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "ActionPortfolio.h"

AInteractableNPC::AInteractableNPC()
{
	InteractionSystem = CreateDefaultSubobject<UInteractionSystemComponent_NPC>("InteractionNPC");
	DialoguerComponet = CreateDefaultSubobject<UDialoguerComponent>("NPCDialoguer");

	GetCapsuleComponent()->ComponentTags.AddUnique(FName("InteractionSensor"));
}


UInteractionSystemComponent* AInteractableNPC::GetInteractionSystemComponent()
{
	return InteractionSystem;
}

void AInteractableNPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
