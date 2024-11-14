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
class WORLDCUSTOMEVENTRUNTIME_API UWorldEventObject : public UObject
{
	GENERATED_BODY()

protected:
	UWorldEventObject();

	FString EventKey;
	bool bDestroyAfterCall;

public:
	virtual void DestroyEvent();
	virtual void ActivateEvent();

public:
	UPROPERTY(BlueprintAssignable)
	FOnWorldCustomEvent WorldCustomEvent;

public:
	UFUNCTION(BlueprintCallable, Category = "World Custom Event", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UWorldEventObject* CreateWorldEvent(UObject* WorldContextObject, FString EventKey, bool bDestroyAfterCall);

	FORCEINLINE FString GetEventKey() const { return EventKey; }

};

UCLASS()
class WORLDCUSTOMEVENTRUNTIME_API UWorldEventSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	UWorldEventSubsystem();

private:
	UPROPERTY()
	TMap<FString , TObjectPtr<UWorldEventObject>> CustomEvents;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	static UWorldEventSubsystem* GetWorldEventSubsystem(UObject* WorldContextObject);

public:
	UFUNCTION(BlueprintCallable, Category = "World Custom Event", meta = (WorldContext = "WorldContextObject"))
	static bool CallWorldCustomEvent(UObject* WorldContextObject, FString EventKey);

	UFUNCTION(BlueprintCallable, Category = "World Custom Event", meta = (WorldContext = "WorldContextObject"))
	static bool RemoveWorldCustomEvent(UObject* WorldContextObject, FString EventKey);

	bool CallWorldCustomEvent(FString EventKey);
	bool RemoveWorldCustomEvent(FString EventKey);
	bool AddWorldCustomEvent(UWorldEventObject& NewWorldEventObject);
};


