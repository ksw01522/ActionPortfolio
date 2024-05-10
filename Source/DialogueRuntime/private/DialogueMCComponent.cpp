// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueMCComponent.h"
#include "DialogueManager.h"
#include "DialogueNode.h"
#include "DialogueNode_Start.h"
#include "DialogueSession.h"
#include "DialogueRuntime.h"

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
	if(!Handle.IsValid()) return false;
	
	UDialogueManager* Manager = UDialogueManager::GetManagerInstance();

	return Manager->IsActingDialogue(const_cast<FDialogueHandle&>(Handle));
}

bool UDialogueMCComponent::TryEnterNextNode(const UDialogueNode* NextNode)
{
	UDialogueManager* Manager = UDialogueManager::GetManagerInstance();

	bool bResult = Manager->TryEnterNextNode(Handle, NextNode);

	#if WITH_EDITOR
	FString MSG = bResult ? "Succeed Enter Next Dialogue" : "Fail Enter Next Dialogue";
	LOG_WARNING(TEXT("%s"), *MSG);
	#endif

	if (bResult)
	{
		CurrentNode = NextNode;

		OnSuccedEnterNextNode(NextNode);
	}
	else {
		OnFailedEnterNextNode(NextNode);
	}

	return bResult;
}

void UDialogueMCComponent::EnterDialogue(const UDialogueSession* Session)
{
	check(Session);
	UDialogueManager* Manager = UDialogueManager::GetManagerInstance();
	Manager->EnterDialogue(Handle, Session);

	CurrentSession = Session;
	CurrentNode = Session->GetStartNode();

	OnEnterDialogue();
}

void UDialogueMCComponent::TryExitDialogue(bool bCancelled)
{
	check(CurrentSession);

	UDialogueManager* Manager = UDialogueManager::GetManagerInstance();
	if (Manager->ExitDialogue(Handle, bCancelled))
	{
		OnExitDialogue(bCancelled);

		CurrentSession = nullptr;
		CurrentNode = nullptr;
	}
}







