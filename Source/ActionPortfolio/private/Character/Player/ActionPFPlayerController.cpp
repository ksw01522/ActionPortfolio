// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/ActionPFPlayerController.h"
#include "ActionPortfolio.h"
#include "Interaction/InteractionType/InteractionSystemComponent_NPC.h"
#include "Interaction/InteractionType/SNPCInteractWidget.h"
#include "Blueprint/UserWidget.h"
#include "DialogueSession.h"
#include "Nodes/DialogueNode_Basic.h"
#include "DialogueManager.h"
#include "DialogueBFL.h"
#include "DialoguerComponent.h"
#include "Engine/GameViewportClient.h"
#include "Engine/DataTable.h"
#include "ActionPortfolioInstance.h"
#include "Character/Player/PlayerCharacter.h"


AActionPFPlayerController::AActionPFPlayerController()
{
	PlayerDialoguer = CreateDefaultSubobject<UDialoguerComponent>(TEXT("Dialoguer"));
}



void AActionPFPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DialogueAnimDel_NPC.BindUObject(this, &AActionPFPlayerController::AnimDialogue, EActionPFDialogueType::NPC);
}

void AActionPFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetGenericTeamId(1);
}

void AActionPFPlayerController::InteractWithNPC(UInteractionSystemComponent_NPC* NPCInteractionSystem)
{
	if (!IsValid(NPCInteractionSystem)) {
		PFLOG(Warning, TEXT("Can't find NPCInteraction System Component."));
		return;
	}

	ChangeUIInputMode();

	SAssignNew(NPCInteractWidget, SNPCInteractWidget).OwnerPlayer(this).NPCInteractionSystem(NPCInteractionSystem);
	GEngine->GameViewport->AddViewportWidgetForPlayer(GetLocalPlayer(), NPCInteractWidget.ToSharedRef(), 0);

		
	OnEnterDialogue(EActionPFDialogueType::NPC);
}



void AActionPFPlayerController::ExitInteractNPC()
{
	GEngine->GameViewport->RemoveViewportWidgetForPlayer(
		GetLocalPlayer(),
		NPCInteractWidget.ToSharedRef()
	);

	ChangeGameInputMode();
}

void AActionPFPlayerController::OnMouseButtonDownInDialogueBox_NPC()
{
	if (NPCInteractWidget->IsDialogueAnimating()) {
		ForceDialogueAnimComplete(EActionPFDialogueType::NPC);
	}
	else if(PlayerDialoguer->IsInDialogue()) {
		EnterNextDialogue(EActionPFDialogueType::NPC);
	}
}

FString AActionPFPlayerController::GetDialoguerID() const
{
	return PlayerDialoguer->GetDialoguerID();
}

void AActionPFPlayerController::OnEnterDialogue(EActionPFDialogueType Type)
{
	EnterNextDialogue(Type);
}

UAbilitySystemComponent* AActionPFPlayerController::GetAbilitySystemComponent() const
{
	AActionPortfolioCharacter* ControlPawn = GetPawn<AActionPortfolioCharacter>();
	if(!IsValid(ControlPawn)) return nullptr;

	return ControlPawn->GetAbilitySystemComponent();
}

void AActionPFPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (TeamID != NewTeamID) {
		TeamID = NewTeamID;
	}
}


void AActionPFPlayerController::EnterNextDialogue(EActionPFDialogueType Type)
{
	if(!PlayerDialoguer->IsInDialogue()) return;

	switch (Type)
	{
		case EActionPFDialogueType::NPC:
			if (!NPCInteractWidget.IsValid()){return;}
			NPCInteractWidget->SetShowInteractButton(false);
			UDialogueManager* DialogueManager = UDialogueBFL::GetDialogueManager();

			TArray<FDialogueElement> DialogueElements;
			EDialogueNodeType DialogueNodeType = DialogueManager->EnterNextNode(DialogueElements, PlayerDialoguer);

			switch (DialogueNodeType)
			{
			case EDialogueNodeType::Basic:
				NPCInteractWidget->SetDialoguerName(DialogueElements[0].Name);
				NPCInteractWidget->SetDialogueText(DialogueElements[0].DialogString);
				NPCInteractWidget->SetDialogueStyleSet(DialogueElements[0].DialogueStyleSet);
				NPCInteractWidget->SetDialogueDecorator(DialogueElements[0].DialogueSlateDecorators);
				AnimDialogue(EActionPFDialogueType::NPC);
				break;

			case EDialogueNodeType::Question:
				PFLOG(Warning, TEXT("Node type question"));
				break;

			case EDialogueNodeType::End:
			case EDialogueNodeType::None:
				NPCInteractWidget->SetShowInteractButton(true);
				break;
			}
		break;
	}

	

	
}

void AActionPFPlayerController::AnimDialogue(EActionPFDialogueType Type)
{
	if(!PlayerDialoguer->IsInDialogue()) return;

	switch (Type)
	{
		case EActionPFDialogueType::NPC:
			if (!NPCInteractWidget.IsValid()) return;

			NPCInteractWidget->AnimDialogueText();

			UActionPortfolioInstance* ActionPFInstance = GetGameInstance<UActionPortfolioInstance>();

			if (NPCInteractWidget->IsDialogueAnimating()) {
				GetWorld()->GetTimerManager().SetTimer(DialogueAnimHandle_NPC, DialogueAnimDel_NPC, ActionPFInstance->GetDialogueAnimTime(), false);
			}
			else {
				OnCompleteDialogueAnim(EActionPFDialogueType::NPC);
			}

		break;
	}
}

void AActionPFPlayerController::ForceDialogueAnimComplete(EActionPFDialogueType Type)
{
	if(!NPCInteractWidget.IsValid()) return;

	NPCInteractWidget->ForceDialogueAnimComplete();
	GetWorld()->GetTimerManager().ClearTimer(DialogueAnimHandle_NPC);
	OnCompleteDialogueAnim(EActionPFDialogueType::NPC);
}

void AActionPFPlayerController::OnCompleteDialogueAnim(EActionPFDialogueType Type)
{
	
}

void AActionPFPlayerController::ChangeUIInputMode()
{
	FInputModeUIOnly InputUIMode;
	SetShowMouseCursor(true);
	SetInputMode(InputUIMode);
}

void AActionPFPlayerController::ChangeGameInputMode()
{
	FInputModeGameOnly InputGameMode;
	SetShowMouseCursor(false);
	SetInputMode(InputGameMode);
}
