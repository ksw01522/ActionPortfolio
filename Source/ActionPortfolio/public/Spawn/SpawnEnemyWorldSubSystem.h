// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpawnEnemyWorldSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API USpawnEnemyWorldSubSystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TArray<TObjectPtr<class USpawnRule>> ActableSpawnRules;

	bool bSpawnStarted = false;

	float SpawnStartTime = 0.f;

	UPROPERTY()
	TArray<TObjectPtr<class USpawnRule>> ActivatedSpawnRules;

	TArray<TObjectPtr<class USpawnRule>> RemoveTargetSpawnRules;

protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;


public:
	void StartSpawn();

	FVector GetSpawnCenterLocation() const;

	void RemoveActivatedSpawnRule(USpawnRule* InRule);
};
