// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldEventSubsystem.h"
#include "ActionPortfolio.h"


UWorldEventObject::UWorldEventObject()
{

}

void UWorldEventObject::FinishDestroy()
{
	PFLOG(Warning, TEXT("Destroy World Event"));
	Super::FinishDestroy();
}



void UWorldEventObject::CallWorldCustomEvent()
{
	if(WorldCustomEvent.IsBound()) WorldCustomEvent.Broadcast();
	if (bDestroyAfterCall)
	{
		UWorldEventSubsystem* EventSubSystem = GetWorld()->GetSubsystem<UWorldEventSubsystem>();
		EventSubSystem->RemoveCustomEvent(EventKey);
	}
}

/////////////////////////////// World Event Sub system ////////////////////////

UWorldEventSubsystem::UWorldEventSubsystem()
{
}

void UWorldEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CustomEvents.Empty();
}

void UWorldEventSubsystem::Deinitialize()
{
	for (auto CustomEvent : CustomEvents)
	{
		CustomEvent.Value->MarkAsGarbage();
	}
	
	CustomEvents.Empty();
	Super::Deinitialize();
}

UWorldEventObject* UWorldEventSubsystem::MakeWorldEvent(FString EventKey, bool bDestroyAfterCall)
{
	if (CustomEvents.Contains(EventKey)) {
		PFLOG(Warning, TEXT("Already Has World Custom Event By %s"), *EventKey);
		return nullptr;
	}

	UWorldEventObject* ReturnOBJ = NewObject<UWorldEventObject>(this);
	ReturnOBJ->EventKey = EventKey;
	ReturnOBJ->bDestroyAfterCall = bDestroyAfterCall;

	CustomEvents.Add(EventKey, ReturnOBJ);

	return ReturnOBJ;
}

bool UWorldEventSubsystem::CallWorldCustomEvent(FString EventKey)
{
	UWorldEventObject** FindedEvent = CustomEvents.Find(EventKey);
	if(FindedEvent == nullptr)	return false;

	UWorldEventObject* EventPtr = *FindedEvent;
	if (EventPtr == nullptr) {
		CustomEvents.Remove(EventKey);
		return false;
	}

	EventPtr->CallWorldCustomEvent();
	return true;
}

void UWorldEventSubsystem::RemoveCustomEvent(FString EventKey)
{
	CustomEvents.Remove(EventKey);
}
