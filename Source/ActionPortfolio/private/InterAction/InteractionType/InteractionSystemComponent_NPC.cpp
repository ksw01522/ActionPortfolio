// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractionType/InteractionSystemComponent_NPC.h"
#include "Character/Player/ActionPFPlayerController.h"
#include "ActionPortfolio.h"
#include "DialogueSession.h"
#include "Kismet/KismetMathLibrary.h"
#include "DialoguerComponent.h"
#include "DialogueBFL.h"

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

bool UNPCInteract_Dialogue::IsCanNPCInteract_Implementation(AActionPFPlayerController* InteractPlayer) const
{
	return IsValid(InteractPlayer) && DialogueSession != nullptr;
}

void UNPCInteract_Dialogue::NPCInteract_Implementation(AActionPFPlayerController* InteractPlayer) const
{
	InteractPlayer->EnterDialogueInNPCInteract(DialogueSession);

}





///////////////////////////// InteractionSystemComponent_NPC ////////////////////////////////////
UInteractionSystemComponent_NPC::UInteractionSystemComponent_NPC()
{
	InteractionName = LOCTEXT("NPC InteractionSystem Name", "말 걸기");
}

void UInteractionSystemComponent_NPC::BeginPlay()
{
	Super::BeginPlay();

	for (auto Interaction : NPCInteractions) {
		if (Interaction == nullptr) {
			PFLOG(Error, TEXT("NPC Interaction is nullptr."));
			continue;
		}

		Interaction->SetOwnerSystem(this);
	}
	
}

bool UInteractionSystemComponent_NPC::CanInteract_CPP(AActor* InteractActor) const
{
	if(!IsValid(InteractActor)) return false;

	AActionPFPlayerController* InteractPlayerController = InteractActor->GetInstigatorController<AActionPFPlayerController>();
	if(InteractPlayerController == nullptr) return false;

	return true;
}

void UInteractionSystemComponent_NPC::Interact_CPP(AActor* InteractActor)
{
	ensure(IsValid(InteractActor));

	AActionPFPlayerController* InteractPlayerController = InteractActor->GetInstigatorController<AActionPFPlayerController>();
	if(!IsValid(InteractPlayerController)) return;

	InteractPlayerController->InteractWithNPC(this);
}

const UDialogueSession* UInteractionSystemComponent_NPC::GetGreetingDialogue(const AActionPFPlayerController* PlayerController) const
{
	if(GreetingDialogueSession.IsEmpty()) return nullptr;

	int32 Num = GreetingDialogueSession.Num();

	int32 Idx = UKismetMathLibrary::RandomInteger(Num);

	return GreetingDialogueSession[Idx];
}

const TArray<UNPCInteract*> UInteractionSystemComponent_NPC::GetAbleNPCInteractions(AActionPFPlayerController* PlayerController) const
{
	TArray<UNPCInteract*> ReturnInteractions;

	for (auto NPCInteract : NPCInteractions)
	{
		if(NPCInteract->IsCanNPCInteract(PlayerController)) ReturnInteractions.Add(NPCInteract);
	}

	return ReturnInteractions;
}

UDialoguerComponent* UInteractionSystemComponent_NPC::GetNPCDialoguerComponent()
{
	if(DialoguerComponent.IsValid()) return DialoguerComponent.Get();
	
	ensure(IsValid(GetOwner()));

	DialoguerComponent = Cast<UDialoguerComponent>( GetOwner()->GetComponentByClass(UDialoguerComponent::StaticClass()));

	if(!DialoguerComponent.IsValid()) PFLOG(Warning, TEXT("Can't find Dialoguer Component from %s"), *GetOwner()->GetName());

	return DialoguerComponent.Get();
}

void UInteractionSystemComponent_NPC::SetNPCDialoguerComponent(UDialoguerComponent* NewDialoguer)
{
	DialoguerComponent = NewDialoguer;
}







#undef LOCTEXT_NAMESPACE

