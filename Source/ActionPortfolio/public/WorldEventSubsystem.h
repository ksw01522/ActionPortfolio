// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WorldEventSubsystem.generated.h"

/**
 * 
 */
 DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWorldCustomEvent);

UCLASS(BlueprintType)
class ACTIONPORTFOLIO_API UWorldEventObject : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	friend class UWorldEventSubsystem;

protected:
	UWorldEventObject();

	FString EventKey;
	bool bDestroyAfterCall;

private:
	bool AddEventInSubsystem(UWorldEventSubsystem& Subsystem);

protected:
	virtual void FinishDestroy() override;

	virtual void DestroyEvent();

public:
	UPROPERTY(BlueprintAssignable)
	FOnWorldCustomEvent WorldCustomEvent;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWorldEventObject* MakeWorldEvent(UWorldEventSubsystem* EventSubSystem, FString EventKey, bool bDestroyAfterCall);

private:
	void CallWorldCustomEvent();
};

UCLASS()
class ACTIONPORTFOLIO_API UWorldEventSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	UWorldEventSubsystem();

private:
	UPROPERTY()
	TMap<FString , UWorldEventObject*> CustomEvents;

	friend bool UWorldEventObject::AddEventInSubsystem(UWorldEventSubsystem&);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

public:
	UFUNCTION(BlueprintCallable, Category = "WorldEvent")
	bool CallWorldCustomEvent(FString EventKey);

	UFUNCTION(BlueprintCallable, Category = "WorldEvent")
	void RemoveCustomEvent(FString EventKey);

};


