// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ActionPFGameplayAbility.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityEnded, bool, bWasCancelled);

USTRUCT(BlueprintType)
struct FActionPFEffectContainer
{
	GENERATED_BODY()

	FActionPFEffectContainer() {}


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ActionPFEffectContainer)
	TArray<TSubclassOf<UGameplayEffect>> EffectClasses;
};

UCLASS(Abstract)
class ACTIONPORTFOLIO_API UActionPFGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category = "Ability")
	TArray<FGameplayEffectSpecHandle> MakeEffectSpecHandle(TArray<TSubclassOf<UGameplayEffect>> ArrayEffectClass);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilty")
	virtual bool CanReactivateAbility() const {return false;};

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void ReactivateAbility();


protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FScalableFloat CooldownDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FGameplayTagContainer CooldownTags;

	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
		TSoftObjectPtr<UTexture2D> AbilityIconTexture;

public:
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	virtual TSoftObjectPtr<UTexture2D> GetAbilityIconTexture() const;
};
