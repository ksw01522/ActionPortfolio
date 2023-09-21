// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/GameplayAbility_Meelee.h"
#include "Ability_BossFrenzy.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ACTIONPORTFOLIO_API UAbility_BossFrenzy : public UGameplayAbility_Meelee
{
	GENERATED_BODY()
	
	UAbility_BossFrenzy();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Frenzy", meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	int MinFrenzyCount;

	UPROPERTY(EditDefaultsOnly, Category = "Frenzy", meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	int MaxFrenzyCount;

	int RemainFrenzyCount;

private:
	UFUNCTION()
	void CheckFrenzyCount(FGameplayEventData EventData);

	void RotateToTarget();

protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	
};
