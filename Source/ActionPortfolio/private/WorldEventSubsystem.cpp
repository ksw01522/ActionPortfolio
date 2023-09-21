// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldEventSubsystem.h"
#include "ActionPortfolio.h"


UWorldEventObject::UWorldEventObject()
{

}

bool UWorldEventObject::AddEventInSubsystem(UWorldEventSubsystem& Subsystem)
{
	if (Subsystem.CustomEvents.Contains(EventKey)) {
		if (IsValid(Subsystem.CustomEvents[EventKey])) {
			PFLOG(Error, TEXT("Already Exist In CustomEvents EventKey : %s"), *EventKey);
			return false;
		}

		Subsystem.CustomEvents[EventKey] = this;
		return true;
	}
	else
	{
		Subsystem.CustomEvents.Add(EventKey, this);
		return true;
	}
}

void UWorldEventObject::FinishDestroy()
{
	PFLOG(Log, TEXT("Finish Destroy World Event : %s"), *EventKey);
	Super::FinishDestroy();
}

void UWorldEventObject::DestroyEvent()
{
	PFLOG(Log, TEXT("Ready To Destroy World Event : %s"), *EventKey);

	SetReadyToDestroy();
	MarkAsGarbage();
}



UWorldEventObject* UWorldEventObject::MakeWorldEvent(UWorldEventSubsystem* EventSubSystem, FString EventKey, bool bDestroyAfterCall)
{
	if (EventSubSystem == nullptr) {
		PFLOG(Error, TEXT("EventSubSystem == nullptr from %s"), *EventKey);
		return nullptr;
	}

	UWorldEventObject* NewWorldEvent = NewObject<UWorldEventObject>(EventSubSystem);
	NewWorldEvent->EventKey = EventKey;
	NewWorldEvent->bDestroyAfterCall = bDestroyAfterCall;

	NewWorldEvent->AddEventInSubsystem(*EventSubSystem);
	
	return NewWorldEvent;
}

void UWorldEventObject::CallWorldCustomEvent()
{
	if(WorldCustomEvent.IsBound()) WorldCustomEvent.Broadcast();
}



/////////////////////////////// World Event Sub system ////////////////////////

UWorldEventSubsystem::UWorldEventSubsystem()
{
}

void UWorldEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UWorldEventSubsystem::Deinitialize()
{
	for (auto& CustomEvent : CustomEvents)
	{
		if(IsValid(CustomEvent.Value)) CustomEvent.Value->MarkAsGarbage();
	}
	CustomEvents.Empty();
	Super::Deinitialize();
}

void UWorldEventSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	CustomEvents.Empty();

}



bool UWorldEventSubsystem::CallWorldCustomEvent(FString EventKey)
{
	PFLOG(Log, TEXT("Call World Custom Event Key = %s"), *EventKey);

	UWorldEventObject** FindedEvent = CustomEvents.Find(EventKey);
	if(FindedEvent == nullptr)
	{
		PFLOG(Warning, TEXT("Can't Find Event From \"%s\""), *EventKey);
		return false;
	}

	UWorldEventObject* EventPtr = *FindedEvent;
	if (!IsValid(EventPtr)) {
		PFLOG(Warning, TEXT("Not Valid Event From \"%s\""), *EventKey);

		CustomEvents.Remove(EventKey);
		return false;
	}

	EventPtr->CallWorldCustomEvent();
	if (EventPtr->bDestroyAfterCall)
	{
		RemoveCustomEvent(EventKey);
	}

	return true;
}

void UWorldEventSubsystem::RemoveCustomEvent(FString EventKey)
{
	if (CustomEvents.Contains(EventKey)) 
	{
		CustomEvents[EventKey]->DestroyEvent();
		CustomEvents.Remove(EventKey);
	}
	else {
		PFLOG(Warning, TEXT("Can't Remove World Event by key : %s"), *EventKey);
	}
}
