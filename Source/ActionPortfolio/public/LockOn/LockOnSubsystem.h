// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LockOnSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API ULockOnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	ULockOnSubsystem();

private:
	TArray<TWeakObjectPtr< class ULockOnTargetComponent>> Targets;

protected:
	//virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	//virtual void PostInitialize() {}

	//virtual void OnWorldBeginPlay(UWorld& InWorld) {}

public:
	void RegisterLockOnTarget(ULockOnTargetComponent* InTarget);
	void UnregisterLockOnTarget(ULockOnTargetComponent* InTarget);

	bool GetLockOnAbleTargets(TArray<ULockOnTargetComponent*>& OutArray, const TDelegate<bool (ULockOnTargetComponent*)>& InDel);
};
