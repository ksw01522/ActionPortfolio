// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueMCComponent.h"
#include "DialogueManager.h"
#include "DialogueNode.h"

// Sets default values for this component's properties
UDialogueMCComponent::UDialogueMCComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bCanEnterNextNodeInMC = true;
}


// Called when the game starts
void UDialogueMCComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


bool UDialogueMCComponent::IsActingDialogue() const
{
	IDialogueMCManagerInterface* MCManager = UDialogueManager::GetDialogueMCManager();
	
	return MCManager->IsActingDialogue(GetDialogueHandle());
}

bool UDialogueMCComponent::TryEnterNextNode(FNextDialogueNodeOptionalStruct* OptionalStruct)
{
	if(!bCanEnterNextNodeInMC) { return false; }

	IDialogueMCManagerInterface* MCManager = UDialogueManager::GetDialogueMCManager();

	FDialogueElementContainer ElementContatiner;
	bool Result = MCManager->EnterNextNode(ElementContatiner, Handle, OptionalStruct);
	if (Result)
	{
		OnSuccedEnterNextNode(ElementContatiner);
	}
	else {
		OnFailedEnterNextNode();
	}

	return Result;
}

void UDialogueMCComponent::EnterDialogue(const UDialogueSession* Session)
{
	IDialogueMCManagerInterface* MCManager = UDialogueManager::GetDialogueMCManager();

	if (IsActingDialogue()) {
		TryExitDialogue();
	}

	MCManager->EnterDialogue(Handle, Session);
	ensureMsgf(IsActingDialogue(), TEXT("Try Enter invalid Dialogue."));

	OnEnterDialogue();
}

void UDialogueMCComponent::TryExitDialogue()
{
	IDialogueMCManagerInterface* MCManager = UDialogueManager::GetDialogueMCManager();

	if (MCManager->ExitDialogue(Handle)) {
		OnExitDialogue(true);
	}
}

void UDialogueMCComponent::OnExitDialogue(bool bIsCancelled)
{
	if(OnExitDel.IsBound()){
		OnExitDel.Broadcast(bIsCancelled);
	}

	if (OnExitOnceDel.IsBound())
	{
		OnExitOnceDel.Broadcast(bIsCancelled);
		OnExitOnceDel.Clear();
	}

}

void UDialogueMCComponent::OnEnterDialogue()
{
	if(OnEnterDel.IsBound())
	{
		OnEnterDel.Broadcast();
	}

	if (OnEnterOnceDel.IsBound())
	{
		OnEnterOnceDel.Broadcast();
		OnEnterOnceDel.Clear();
	}
}




