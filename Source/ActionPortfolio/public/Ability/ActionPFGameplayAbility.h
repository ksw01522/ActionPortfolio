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

UCLASS()
class ACTIONPORTFOLIO_API UActionPFGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:


public:
	UFUNCTION(BlueprintCallable, Category = "Ability")
	TArray<FGameplayEffectSpecHandle> MakeEffectSpecHandle(TArray<TSubclassOf<UGameplayEffect>> ArrayEffectClass);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilty")
	virtual bool CanReactivateAbility() const {return false;};

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void ReactivateAbility();
};
