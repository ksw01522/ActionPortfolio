// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ActionPortfolioCharacter.h"
#include "ChracterEnemyA.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API AChracterEnemyA : public AActionPortfolioCharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackShape", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LeftPunchBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackShape", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RightPunchBox;

public:
	AChracterEnemyA();
};
