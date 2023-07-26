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

public:
	UPROPERTY(BlueprintAssignable)
	FOnWorldCustomEvent WorldCustomEvent;

public:
	UFUNCTION(BlueprintCallable, Category = "WorldEvent")
	static UWorldEventObject* MakeWorldEvent(FString EventKey);

private:
	void CallWorldCustomEvent();
};

UCLASS()
class ACTIONPORTFOLIO_API UWorldEventSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	UWorldEventSubsystem();

private:
	

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

};


