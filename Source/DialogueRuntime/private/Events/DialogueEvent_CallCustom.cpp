// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/DialogueEvent_CallCustom.h"
#include "DialogueBFL.h"
#include "DialogueManager.h"

UDialogueEvent_CallCustom::UDialogueEvent_CallCustom()
{
	InstancingPolicy = EDialougeEventInstancingPolicy::NotInstanced;
}

void UDialogueEvent_CallCustom::OnCalledEvent_Implementation()
{
	UDialogueBFL::GetDialogueManager()->CallCustomEvent(EventID);
}
