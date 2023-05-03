// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractionType/InteractionSystemComponent_NPC.h"
#include "GameFramework/PlayerController.h"

#define LOCTEXT_NAMESPACE "NPCIneract"

////////////////////////////// NPCInteract ////////////////////////////
UNPCInteract::UNPCInteract()
{

}





///////////////////////////// InteractionSystemComponent_NPC ////////////////////////////////////
UInteractionSystemComponent_NPC::UInteractionSystemComponent_NPC()
{
}

bool UInteractionSystemComponent_NPC::IsCanInteract_Implementation(AActor* InteractActor) const
{
	if(!IsValid(InteractActor)) return false;

	APlayerController* InteractPlayerController = InteractActor->GetInstigatorController<APlayerController>();
	if(InteractPlayerController == nullptr) return false;

	return true;
}

void UInteractionSystemComponent_NPC::Interact_Implementation(AActor* InteractActor)
{
	if (!IsCanInteract(InteractActor)) { return; }

	
}


#undef LOCTEXT_NAMESPACE