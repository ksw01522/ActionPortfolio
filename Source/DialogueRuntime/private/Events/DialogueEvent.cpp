// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/DialogueEvent.h"
#include "DialogueManager.h"
#include "DialogueRuntime.h"
#include "DialogueBFL.h"

void UDialogueEvent::CallEvent(const FDialogueHandle& Handle)
{
	if (!Handle.IsValid()) {
		LOG_ERROR(TEXT("Dialogue Handle Is Not Valid."));
		return;
	}

	if (InstancingPolicy == EDialougeEventInstancingPolicy::NotInstanced) {
		OnCalledEvent(Handle);
		CalledDialogueID = Handle;
	}
	else if (InstancingPolicy == EDialougeEventInstancingPolicy::InstancedPerExecution) {
		UDialogueEvent* InstancedObject = NewObject<UDialogueEvent>(GetTransientPackage(), GetClass(), NAME_None, EObjectFlags::RF_NoFlags, this);
		UDialogueManager* Manager = UDialogueBFL::GetDialogueManager();
		Manager->AddDialogueEvent(InstancedObject, Handle);
		InstancedObject->OnCalledEvent(Handle);
		InstancedObject->CalledDialogueID = Handle;
	}
}
