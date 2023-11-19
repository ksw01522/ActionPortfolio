// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueBFL.h"
#include "DialogueNode.h"
#include "Nodes/DialogueNode_Basic.h"
#include "Nodes/DialogueNode_Question.h"
#include "DialogueRuntime.h"
#include "Kismet/GameplayStatics.h"


UDialogueManager* UDialogueBFL::GetDialogueManager()
{
	if(GEngine == nullptr || GEngine->GameViewport == nullptr) return nullptr;
	UWorld* World = GEngine->GameViewport->GetWorld();
	if (World == nullptr) return nullptr;
	UDialogueManager* DialogueManager = UGameInstance::GetSubsystem<UDialogueManager>(World->GetGameInstance());
	if (!IsValid(DialogueManager)) {
		LOG_ERROR(TEXT("Can't find Dialogue Manager."));
		return nullptr;
	}
	return DialogueManager;
}

UDialogueManager* UDialogueBFL::TestGetManager(UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject); World == nullptr)
	{
		LOG_ERROR(TEXT("Can't find World by Context Object."));
		return nullptr;
	}


	LOG_ERROR(TEXT("find World by Context Object."));

	return nullptr;
}
