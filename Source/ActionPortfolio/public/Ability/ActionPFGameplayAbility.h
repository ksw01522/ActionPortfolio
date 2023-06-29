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
	TArray<TSubclassOf<UGameplayEffect>> EffectClass;
};

UCLASS()
class ACTIONPORTFOLIO_API UActionPFGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	FGameplayTag ReactivateEventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee", meta = (AllowPrivateAccess = "true"))
	bool bRactiveEventTriggerOnce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee", meta = (AllowPrivateAccess = "true"))
	bool bReactiveEventTagMatchExact;

public:
	UFUNCTION(BlueprintCallable, Category = "Ability")
	TArray<FGameplayEffectSpecHandle> MakeEffectSpecHandle(TArray<TSubclassOf<UGameplayEffect>> ArrayEffectClass);


	UFUNCTION(BlueprintCallable, BlueprintPure, Category ="Abilty")
	const FGameplayTag GetReactivateEventTag() const {return ReactivateEventTag;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilty")
	virtual bool CanReactivateAbility() const;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

};

USTRUCT(BlueprintType)
struct FActionPFDamageStruct {
	GENERATED_BODY()

	FActionPFDamageStruct() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Damage")
	class UActionPFKnockbackObject* Knockback;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<class UGameplayEffect_Damage> DamageEffectClass;
};