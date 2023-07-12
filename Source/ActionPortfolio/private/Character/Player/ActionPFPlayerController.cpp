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
#include "UI/DialogueSlate.h"
#include "GameFramework/CharacterMovementComponent.h"

AActionPFPlayerController::AActionPFPlayerController()
{
	PlayerDialoguer = CreateDefaultSubobject<UDialoguerComponent>(TEXT("Dialoguer"));
}



void AActionPFPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DialogueAnimDel_NPC.BindUObject(this, &AActionPFPlayerController::AnimDialogue, EActionPFDialogueType::NPC);
	DialogueAnimDel_Basic.BindUObject(this, &AActionPFPlayerController::AnimDialogue, EActionPFDialogueType::Basic);
}

void AActionPFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetGenericTeamId(1);

	ChangeGameInputMode();
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

		
	EnterNextDialogue(EActionPFDialogueType::NPC);
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

void AActionPFPlayerController::EndDialogueSlate()
{
	if (DialogueSlate.IsValid()) {
		GEngine->GameViewport->RemoveViewportWidgetForPlayer(
			GetLocalPlayer(),
			DialogueSlate.ToSharedRef()
		);
	}

	ChangeGameInputMode();
}

void AActionPFPlayerController::OnMouseButtonDownInDialogueBox()
{
	if (DialogueSlate->IsDialogueAnimating()) {
		ForceDialogueAnimComplete(EActionPFDialogueType::Basic);
	}
	else if (PlayerDialoguer->IsInDialogue()) {
		EnterNextDialogue(EActionPFDialogueType::Basic);
	}
}

void AActionPFPlayerController::EnterDialogueBasic(UDialogueSession* NewSession)
{
	if(NewSession == nullptr) return;

	UDialogueManager* DialogueManager = UDialogueBFL::GetDialogueManager();
	TArray<FString> DialoguerIDs;
	DialoguerIDs.Add(PlayerDialoguer->GetDialoguerID());
	DialogueManager->EnterDialogue(DialoguerIDs, NewSession);

	SAssignNew(DialogueSlate, SDialogueSlate).OwnerPlayer(this);
	GEngine->GameViewport->AddViewportWidgetForPlayer(GetLocalPlayer(), DialogueSlate.ToSharedRef(), 1);

	if (DialogueSlate.IsValid()) {
		ChangeUIInputMode();
		EnterNextDialogue(EActionPFDialogueType::Basic);
	}
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

	UDialogueManager* DialogueManager = UDialogueBFL::GetDialogueManager();
	TArray<FDialogueElement> DialogueElements;
	EDialogueNodeType DialogueNodeType = DialogueManager->EnterNextNode(DialogueElements, PlayerDialoguer);

	switch (Type)
	{
		case EActionPFDialogueType::NPC:
			if (!NPCInteractWidget.IsValid()){return;}
			NPCInteractWidget->SetShowInteractButton(false);

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

		case EActionPFDialogueType::Basic:
			if(!DialogueSlate.IsValid()){return;}


			switch (DialogueNodeType)
			{
			case EDialogueNodeType::Basic:
				DialogueSlate->SetDialoguerName(DialogueElements[0].Name);
				DialogueSlate->SetDialogueText(DialogueElements[0].DialogString);
				DialogueSlate->SetDialogueStyleSet(DialogueElements[0].DialogueStyleSet);
				DialogueSlate->SetDialogueDecorator(DialogueElements[0].DialogueSlateDecorators);
				AnimDialogue(EActionPFDialogueType::Basic);
				break;

			case EDialogueNodeType::Question:
				PFLOG(Warning, TEXT("Node type question"));
				break;

			case EDialogueNodeType::End:
			case EDialogueNodeType::None:
				EndDialogueSlate();
				break;
			}

		break;
	}

	

	
}

void AActionPFPlayerController::AnimDialogue(EActionPFDialogueType Type)
{
	if(!PlayerDialoguer->IsInDialogue()) return;

	UActionPortfolioInstance* ActionPFInstance = GetGameInstance<UActionPortfolioInstance>();

	switch (Type)
	{
		case EActionPFDialogueType::NPC:
			if (!NPCInteractWidget.IsValid()) return;

			NPCInteractWidget->AnimDialogueText();


			if (NPCInteractWidget->IsDialogueAnimating()) {
				GetWorld()->GetTimerManager().SetTimer(DialogueAnimHandle_NPC, DialogueAnimDel_NPC, ActionPFInstance->GetDialogueAnimTime(), false);
			}
			else {
				OnCompleteDialogueAnim(EActionPFDialogueType::NPC);
			}

		break;

		case EActionPFDialogueType::Basic:
		if(!DialogueSlate.IsValid()) return;

		DialogueSlate->AnimDialogueText();

		if (DialogueSlate->IsDialogueAnimating()) {
			GetWorld()->GetTimerManager().SetTimer(DialogueAnimHandle_Basic, DialogueAnimDel_Basic, ActionPFInstance->GetDialogueAnimTime(), false);
		}
		else {
			OnCompleteDialogueAnim(EActionPFDialogueType::Basic);
		}
		break;
		
	}
}

void AActionPFPlayerController::ForceDialogueAnimComplete(EActionPFDialogueType Type)
{
	switch (Type) {
		case EActionPFDialogueType::NPC:
			if (!NPCInteractWidget.IsValid()) return;

			NPCInteractWidget->ForceDialogueAnimComplete();
			GetWorld()->GetTimerManager().ClearTimer(DialogueAnimHandle_NPC);
			OnCompleteDialogueAnim(EActionPFDialogueType::NPC);
			break;

		case EActionPFDialogueType::Basic:
			if (!DialogueSlate.IsValid()) return;

			DialogueSlate->ForceDialogueAnimComplete();
			GetWorld()->GetTimerManager().ClearTimer(DialogueAnimHandle_Basic);
			OnCompleteDialogueAnim(EActionPFDialogueType::Basic);

		break;
	}
}

void AActionPFPlayerController::OnCompleteDialogueAnim(EActionPFDialogueType Type)
{
	
}

void AActionPFPlayerController::ChangeUIInputMode()
{

	FInputModeUIOnly InputUIMode;
	SetShowMouseCursor(true);
	SetInputMode(InputUIMode);

	InputComponent->bBlockInput = true;
}

void AActionPFPlayerController::ChangeGameInputMode()
{
	FInputModeGameOnly InputGameMode;
	SetShowMouseCursor(false);
	SetInputMode(InputGameMode);

	InputComponent->bBlockInput = false;
}
