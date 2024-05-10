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
	
private:
	UPROPERTY(Transient)
	TObjectPtr<class UProgressBar> HPBarWidget;

protected:
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data) override;
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data) override;
};
