// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/Events/DialogueEvent_CallWorldEvent.h"
#include "WorldEventSubsystem.h"
#include "ActionPortfolio.h"

void UDialogueEvent_CallWorldEvent::OnCalledEvent_Implementation(FActingDialogueHandle& Handle)
{
	if (GEngine == nullptr) return ;
	if (GEngine->GameViewport == nullptr) return ;
	UWorld* World = GEngine->GameViewport->GetWorld();
	if (World == nullptr) return ;
	UWorldEventSubsystem* WorldeventSubsystem = World->GetSubsystem<UWorldEventSubsystem>();
	
	if (WorldeventSubsystem->CallWorldCustomEvent(EventKey))
	{

	}
	else
	{
		PFLOG(Warning, TEXT("Can't Call World Custom Event : %s"), *EventKey);
	}
	
}
