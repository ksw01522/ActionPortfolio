// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldEventSubsystem.generated.h"

/**
 * 
 */
 DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWorldCustomEvent);

UCLASS()
class ACTIONPORTFOLIO_API UWorldEventObject : public UObject
{
	GENERATED_BODY()

	friend class UWorldEventSubsystem;

protected:
	UWorldEventObject();

	FString EventKey;
	bool bDestroyAfterCall;

	virtual void FinishDestroy() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnWorldCustomEvent WorldCustomEvent;

public:
	

private:
	void CallWorldCustomEvent();
};

UCLASS()
class ACTIONPORTFOLIO_API UWorldEventSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	UWorldEventSubsystem();

private:
	TMap<FString , UWorldEventObject*> CustomEvents;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "WorldEvent")
	UWorldEventObject* MakeWorldEvent(FString EventKey, bool bDestroyAfterCall);

	UFUNCTION(BlueprintCallable, Category = "WorldEvent")
	bool CallWorldCustomEvent(FString EventKey);

	UFUNCTION(BlueprintCallable, Category = "WorldEvent")
	void RemoveCustomEvent(FString EventKey);
};


