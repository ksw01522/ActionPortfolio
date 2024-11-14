// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ActionPortfolioCharacter.h"
#include "CharacterEnemy.generated.h"

/**
 * 
 */


UCLASS()
class ACTIONPORTFOLIO_API ACharacterEnemy : public AActionPortfolioCharacter
{
	GENERATED_BODY()
	
public:
	ACharacterEnemy();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<class UProgressBar> HPBarWidget;

	UPROPERTY()
	TObjectPtr<class UEnemyAttributeSet> EnemyAttributeSet;

public:
	float GetBountyXP() const;
	float GetBountyGold() const;

protected:
	virtual void InitializeAttributes() override;

	virtual void OnCharacterDie(const FGameplayEffectModCallbackData& Data) override;

};
