// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ActionPortfolioCharacter.h"
#include "CharacterBoss.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API ACharacterBoss : public AActionPortfolioCharacter
{
	GENERATED_BODY()
	
	ACharacterBoss();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	int CollisionCountForStun;

	int CurrentCollisionCountForStun = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss", meta = (AllowPrivateAccess = "true"))
	float StunTime;


public:
	void OnBossCollisionBatteryBomb();
};
