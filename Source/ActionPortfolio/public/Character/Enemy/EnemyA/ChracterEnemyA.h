// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/CharacterEnemy.h"
#include "ChracterEnemyA.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API AChracterEnemyA : public ACharacterEnemy
{
	GENERATED_BODY()
	

public:
	AChracterEnemyA();

protected:
	virtual void Tick(float DeltaSeconds) override;
};
