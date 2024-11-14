// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PlayGameState.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterDie, class AActionPortfolioCharacter*);

UCLASS()
class ACTIONPORTFOLIO_API APlayGameState : public AGameState
{
	GENERATED_BODY()

//적 스폰관련	
private:
	UPROPERTY(EditAnywhere, Category = "EnemySpawn", Instanced, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class USpawnRule>> SpawnRules;

public:
	TArray<USpawnRule*> GetSpawnRules() const { return SpawnRules; }


//Character 죽음 관련
private:
	FOnCharacterDie OnCharacterDieDelegate;

public:
	FDelegateHandle AddCharacterDieEvent(const TDelegate<void (AActionPortfolioCharacter*)>& InDel);
	bool RemoveCharacterDieEvent(FDelegateHandle& InHandle);
	void OnCharacterDie(AActionPortfolioCharacter* InChar);
};
