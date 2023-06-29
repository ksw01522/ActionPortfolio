// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/GameplayAbility_Meelee.h"
#include "Ability_GutPunch.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UAbility_GutPunch : public UGameplayAbility_Meelee
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GutPunchImpact", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayEffect_Damage> ImpactDamageEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GutPunchImpact", meta = (AllowPrivateAccess = "true"))
	float KnockbackStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GutPunchImpact", meta = (AllowPrivateAccess = "true"))
	float KnockbackZStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GutPunchImpact", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AGAEffectActor> GutImpactActorClass;

	UFUNCTION()
	virtual void OnImpactSpawnEventReceived(FGameplayEventData EventData);

public:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
