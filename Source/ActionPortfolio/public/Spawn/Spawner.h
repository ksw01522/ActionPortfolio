// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpawningEvent, int, Idx, TArray<AActionPortfolioCharacter*>, SpawnedCharacters);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnedCharacterDieEvent, AActionPortfolioCharacter*, SpawnedCharacters);

USTRUCT(Atomic, BlueprintType)
struct FSpawnDataStruct
{
	GENERATED_BODY();

	FSpawnDataStruct() {
		
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<class AActionPortfolioCharacter> SpawnTargetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FTransform SpawnTransform;
};

USTRUCT(Atomic, BlueprintType)
struct FSpawnNodeStruct
{
	GENERATED_BODY();

	FSpawnNodeStruct() {
		SpawnDataArray.Empty();
	
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<FSpawnDataStruct> SpawnDataArray;


};

UCLASS()
class ACTIONPORTFOLIO_API ASpawner : public AActor
{
	GENERATED_BODY()
	
	ASpawner();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TArray<FSpawnNodeStruct> SpawnNodeStructs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	bool bDestroyAfterSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	bool bAutoSpawnOnAllDestroy;

	int CurrentIDX;

	int CurrentSpawnedCount;

public:
	UPROPERTY(BlueprintAssignable)
	FOnSpawnEvent SpawnStartEvent;

	UPROPERTY(BlueprintAssignable)
	FOnSpawnEvent SpawnEndEvent;

	UPROPERTY(BlueprintAssignable)
	FOnSpawningEvent OnSpawningEvent;

	UPROPERTY(BlueprintAssignable)
	FOnSpawnedCharacterDieEvent OnSpawnedCharacterDieEvent;


protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Spawn")
	void SpawnStart();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Spawn")
	bool IsSpawning() const;

private:
	void SpawnCharacters();

	UFUNCTION()
	void OnSpawnedCharacterDie(class AActionPortfolioCharacter* SpawnedChar);

	void OnSpawnEnd();
};
