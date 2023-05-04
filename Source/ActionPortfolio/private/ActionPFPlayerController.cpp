// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPFPlayerController.h"
#include "Interaction/InteractionType/InteractionSystemComponent_NPC.h"
#include "Interaction/InteractionType/SNPCInteractWidget.h"
#include "Blueprint/UserWidget.h"

AActionPFPlayerController::AActionPFPlayerController()
{
}

void AActionPFPlayerController::InteractWithNPC(const TArray<UNPCInteract*> NPCInteracts)
{
	//if(NPCInteracts.IsEmpty()) return;

	// NPC상호작용을 고를 수 있는 Slate 혹은 UMG 창 만들어서 넣기
	TSharedPtr<SNPCInteractWidget> NPCInteractWidget;

	SAssignNew(NPCInteractWidget, SNPCInteractWidget).OwnerPlayer(this).NPCInteracts(NPCInteracts);
	GEngine->GameViewport->AddViewportWidgetForPlayer(GetLocalPlayer(), NPCInteractWidget.ToSharedRef(), 0);

	
}
