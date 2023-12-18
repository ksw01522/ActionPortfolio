// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueBFL.h"
#include "DialogueNode.h"
#include "Nodes/DialogueNode_Basic.h"
#include "Nodes/DialogueNode_Question.h"
#include "DialogueRuntime.h"
#include "Kismet/GameplayStatics.h"
#include "DialogueManager.h"

UDialoguerComponent* UDialogueBFL::GetDialoguer(const FString& DialoguerID)
{
	
	//return UDialogueManager::DialogueManager->GetDialoguer(DialoguerID);
	return nullptr;
}
