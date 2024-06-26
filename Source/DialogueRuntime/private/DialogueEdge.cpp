// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueEdge.h"
#include "DialogueNode.h"
#include "DialogueSession.h"

#define LOCTEXT_NAMESPACE "DialogueEdge"

UDialogueEdge::UDialogueEdge()
{
}


UDialogueEdge::~UDialogueEdge()
{
}

UDialogueSession* UDialogueEdge::GetDialogueSession()
{
	return DialogueSession;
}

#if WITH_EDITOR

void UDialogueEdge::SetNodeTitle(const FText& NewTitle)
{
	NodeTitle = NewTitle;
}


#endif

bool UDialogueEdge::CanEnterNextNode() const
{
	bool CanEnter = true;

	for (const auto Condition: EnterNextConditions)
	{
		CanEnter = CanEnter && Condition->CanEnterNextNode();
		if(!CanEnter) break;
	}

	return CanEnter;
}


#undef LOCTEXT_NAMESPACE