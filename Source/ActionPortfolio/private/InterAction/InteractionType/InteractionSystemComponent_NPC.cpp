// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractionType/InteractionSystemComponent_NPC.h"
#include "ActionPFPlayerController.h"
#include "ActionPortfolio.h"

#define LOCTEXT_NAMESPACE "NPCIneract"

////////////////////////////// NPCInteract ////////////////////////////
UNPCInteract::UNPCInteract()
{

}



// Dialogue
UNPCInteract_Dialogue::UNPCInteract_Dialogue()
{
	NPCInteractionName = LOCTEXT("Name_Dialogue", "대화하기");
}





///////////////////////////// InteractionSystemComponent_NPC ////////////////////////////////////
UInteractionSystemComponent_NPC::UInteractionSystemComponent_NPC()
{
	InteractionName = LOCTEXT("NPC InteractionSystem Name", "NPC 상호작용 시스템");
}

void UInteractionSystemComponent_NPC::BeginPlay()
{
	for (auto Interaction : NPCInteractions) {
		if (Interaction == nullptr) {
			PFLOG(Error, TEXT("NPC Interaction is nullptr."));
			continue;
		}

		Interaction->SetOwnerSystem(this);
	}
}

bool UInteractionSystemComponent_NPC::IsCanInteract_Implementation(AActor* InteractActor) const
{
	if(!IsValid(InteractActor)) return false;

	AActionPFPlayerController* InteractPlayerController = InteractActor->GetInstigatorController<AActionPFPlayerController>();
	if(InteractPlayerController == nullptr) return false;

	return true;
}

void UInteractionSystemComponent_NPC::Interact_Implementation(AActor* InteractActor)
{
	if (!IsCanInteract(InteractActor)) { return; }

	AActionPFPlayerController* InteractPlayerController = InteractActor->GetInstigatorController<AActionPFPlayerController>();
	TArray<UNPCInteract*> Interactions;
	
	for (auto Interaction : NPCInteractions) {
		if (Interaction == nullptr) {
			PFLOG(Error, TEXT("NPC Interaction is nullptr."));
			continue;
		}

		if(Interaction->IsCanNPCInteract(InteractPlayerController)) Interactions.Add(Interaction);
	}

	InteractPlayerController->InteractWithNPC(Interactions);
}







#undef LOCTEXT_NAMESPACE

