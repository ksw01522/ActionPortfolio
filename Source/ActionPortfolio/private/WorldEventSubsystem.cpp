// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldEventSubsystem.h"

UWorldEventSubsystem::UWorldEventSubsystem()
{
}

void UWorldEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UWorldEventSubsystem::Deinitialize()
{

	Super::Deinitialize();
}

UWorldEventObject::UWorldEventObject()
{
}

UWorldEventObject* UWorldEventObject::MakeWorldEvent(FString EventKey)
{
	UWorldEventObject* ReturnOBJ = NewObject<UWorldEventObject>();
	ReturnOBJ->EventKey = EventKey;
	return ReturnOBJ;
}

void UWorldEventObject::CallWorldCustomEvent()
{
	
}
