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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackShape", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LeftPunchBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackShape", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RightPunchBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss", meta = (AllowPrivateAccess = "true"))
	float StunTime;
};
