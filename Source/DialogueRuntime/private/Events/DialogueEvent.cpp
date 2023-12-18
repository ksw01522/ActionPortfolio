// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/DialogueEvent.h"
#include "DialogueManager.h"
#include "DialogueRuntime.h"
#include "DialogueBFL.h"

void UDialogueEvent::CallEvent()
{
	OnCalledEvent();
}

void UDialogueEvent::CallEndEvent(bool bIsCancelled)
{
	OnEndEvent(bIsCancelled);

	if(InstancingPolicy != EDialougeEventInstancingPolicy::NotInstanced){MarkAsGarbage();}
}

UDialogueEvent* UDialogueEvent::GetEventForCall()
{
	UDialogueEvent* ReturnEvent = nullptr;
	
	if (InstancingPolicy == EDialougeEventInstancingPolicy::NotInstanced) {
		ReturnEvent = this;
	}
	else if (InstancingPolicy == EDialougeEventInstancingPolicy::InstancedPerExecution) {
		ReturnEvent = NewObject<UDialogueEvent>(GetTransientPackage(), GetClass(), NAME_None, EObjectFlags::RF_NoFlags, this);
	}

	return ReturnEvent;
}
