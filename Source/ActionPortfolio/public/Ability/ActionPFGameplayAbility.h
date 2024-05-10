// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ActionPFGameplayAbility.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityEnded, bool, bWasCancelled);

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	None = 0 UMETA(Hidden),
	Active,
	Passive
};

UENUM(BlueprintType)
enum class EAbilityState : uint8
{
	None = 0 UMETA(Hidden),
	Actable,
	Disactable,
	CoolTime
};

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
	

private:
	

public:
	UFUNCTION(BlueprintCallable, Category = "Ability")
	TArray<FGameplayEffectSpecHandle> MakeEffectSpecHandle(TArray<TSubclassOf<UGameplayEffect>> ArrayEffectClass);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

protected:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldowns")
	FScalableFloat CooldownDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldowns")
	FGameplayTagContainer CooldownTags;

	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;
private:
	bool bTempCooldownTagsInitialized = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	EAbilityType AbilityType = EAbilityType::Active;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Style")
	FSlateBrush DefaultIconBrush;


public:
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	virtual TSharedPtr<class SAbilityIcon> CreateAbilityIcon() const;

	virtual const FSlateBrush* GetAbilityIconBrush(TWeakObjectPtr<const UAbilitySystemComponent> InSystem) const;

	EAbilityType GetAbilityType() const { return AbilityType; }



public:
	

protected:
	static const FGameplayTag OnAttackStartTag;
	static const FGameplayTag OnAttackTargetTag;
	static const FGameplayTag OnDamagedTag;

};
