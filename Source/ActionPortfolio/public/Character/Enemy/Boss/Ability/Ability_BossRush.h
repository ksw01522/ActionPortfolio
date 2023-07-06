// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/GameplayAbility_Meelee.h"
#include "Ability_BossRush.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UAbility_BossRush : public UGameplayAbility_Meelee
{
	GENERATED_BODY()
public:
	UAbility_BossRush();

protected:
	virtual void OnEventReceived(FGameplayEventData EventData) override;
};
