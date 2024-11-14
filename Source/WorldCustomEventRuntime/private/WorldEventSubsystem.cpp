// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldEventSubsystem.h"
#include "WorldCustomEventRuntime.h"


UWorldEventObject::UWorldEventObject() : EventKey(""), bDestroyAfterCall(false)
{

}



void UWorldEventObject::DestroyEvent()
{
	//PFLOG(Log, TEXT("Ready To Destroy World Event : %s"), *EventKey);

	MarkAsGarbage();
}

void UWorldEventObject::ActivateEvent()
{
	LOG_INFO(TEXT("Activate World Custom Event Key = %s"), *EventKey);
	UWorldEventSubsystem* EventSS = Cast<UWorldEventSubsystem>(GetOuter());
	if (!IsValid(EventSS))
	{
		DestroyEvent();
		LOG_ERROR(TEXT("Event Object's Outer is not WorldEventSubsystem."));
		return;
	}

	if (WorldCustomEvent.IsBound()) {WorldCustomEvent.Broadcast();}
	else {
		LOG_ERROR(TEXT("Not Bind World Custom Event : Key = %s"), *EventKey);
		EventSS->RemoveWorldCustomEvent(GetEventKey());
		return;
	}

	if (bDestroyAfterCall)
	{
		{
			EventSS->RemoveWorldCustomEvent(GetEventKey());
			return;
		}
	}
}



UWorldEventObject* UWorldEventObject::CreateWorldEvent(UObject* WorldContextObject,
													FString EventKey, bool bDestroyAfterCall)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	UWorldEventSubsystem* WorldEventSS = World->GetSubsystem<UWorldEventSubsystem>();

	UWorldEventObject* NewWorldEvent = NewObject<UWorldEventObject>(WorldEventSS);
	NewWorldEvent->EventKey = EventKey;
	NewWorldEvent->bDestroyAfterCall = bDestroyAfterCall;

	if (!WorldEventSS->AddWorldCustomEvent(*NewWorldEvent))
	{
		NewWorldEvent->DestroyEvent();
		LOG_ERROR(TEXT("Can't Make Event by Key : %s"), *EventKey);
		return nullptr;
	}

	LOG_INFO(TEXT("Complete Make Event by Key : %s"), *EventKey);

	return NewWorldEvent;
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
		if(IsValid(CustomEvent.Value)) CustomEvent.Value->DestroyEvent();
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
	LOG_INFO(TEXT("Call World Custom Event Key = %s"), *EventKey);

	TObjectPtr<UWorldEventObject>* FindedEvent = CustomEvents.Find(EventKey);
	if(FindedEvent == nullptr)
	{
		LOG_ERROR(TEXT("Can't Find Event From \"%s\""), *EventKey);
		return false;
	}

	UWorldEventObject* EventPtr = *FindedEvent;
	if (!IsValid(EventPtr)) {
		LOG_ERROR(TEXT("Not Valid Event From \"%s\""), *EventKey);

		CustomEvents.Remove(EventKey);
		return false;
	}

	EventPtr->ActivateEvent();
	
	return true;
}

bool UWorldEventSubsystem::RemoveWorldCustomEvent(FString EventKey)
{
	if (!CustomEvents.Contains(EventKey))
	{
		LOG_WARNING(TEXT("Can't Remove World Event by key : %s"), *EventKey);
		return false;
	}

	LOG_INFO(TEXT("Remove World Event by key : %s"), *EventKey);
	CustomEvents[EventKey]->DestroyEvent();
	CustomEvents.Remove(EventKey);
	
	return true;
}

UWorldEventSubsystem* UWorldEventSubsystem::GetWorldEventSubsystem(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	UWorldEventSubsystem* WorldEventSS = World->GetSubsystem<UWorldEventSubsystem>();
	if (!IsValid(WorldEventSS))
	{
		LOG_ERROR(TEXT("Can't find World Event Subsystem"));
		return nullptr;
	}

	return WorldEventSS;
}

bool UWorldEventSubsystem::CallWorldCustomEvent(UObject* WorldContextObject, FString EventKey)
{
	UWorldEventSubsystem* WorldEventSS = GetWorldEventSubsystem(WorldContextObject);
	if (WorldEventSS)
	{
		WorldEventSS->CallWorldCustomEvent(EventKey);
	}

	return false;
}

bool UWorldEventSubsystem::RemoveWorldCustomEvent(UObject* WorldContextObject, FString EventKey)
{
	UWorldEventSubsystem* WorldEventSS = GetWorldEventSubsystem(WorldContextObject);
	if (WorldEventSS)
	{
		WorldEventSS->RemoveWorldCustomEvent(EventKey);
	}

	return false;
}

bool UWorldEventSubsystem::AddWorldCustomEvent(UWorldEventObject& NewWorldEventObject)
{
	const FString NewEventKey = NewWorldEventObject.GetEventKey();

	if (CustomEvents.Contains(NewEventKey)) {
		if (IsValid(CustomEvents[NewEventKey])) {
			LOG_ERROR(TEXT("Already Exist In CustomEvents EventKey : %s"), *NewEventKey);
			return false;
		}

		CustomEvents[NewEventKey] = &NewWorldEventObject;
		return true;
	}
	else
	{
		CustomEvents.Add(NewEventKey, &NewWorldEventObject);
		return true;
	}
}
