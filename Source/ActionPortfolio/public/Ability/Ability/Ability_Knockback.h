// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Ability_Knockback.generated.h"

/**
 * 
 */
class UAbilitySystemComponent;

UCLASS()
class ACTIONPORTFOLIO_API UKnockbackEventDataToASC : public UObject
{
	GENERATED_BODY()
public:
	FVector KnockbackVector;
	TWeakObjectPtr<UObject> SourceObject;
};


UCLASS(Abstract)
class ACTIONPORTFOLIO_API UAbility_Knockback : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UAbility_Knockback();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:
	UFUNCTION()
	void OnKnockbackFinished();
};

UCLASS(Abstract)
class ACTIONPORTFOLIO_API UKnockback_Forward : public UAbility_Knockback
{
	GENERATED_BODY()

	UKnockback_Forward();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	FVector2D PowerOnGround;

	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	FVector2D PowerInAir;


protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};

UCLASS(Abstract)
class ACTIONPORTFOLIO_API UKnockback_Radial : public UAbility_Knockback
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	FVector2D PowerOnGround;

	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	FVector2D PowerInAir;


protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};