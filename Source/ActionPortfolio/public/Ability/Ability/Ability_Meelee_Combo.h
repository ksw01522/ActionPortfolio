// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/GameplayAbility_Meelee.h"
#include "Ability_Meelee_Combo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EComboSelectPolicy : uint8
{
	Auto = 0,
	Menual = 1
};

UCLASS(Abstract)
class ACTIONPORTFOLIO_API UAbility_Meelee_Combo : public UGameplayAbility_Meelee
{
	GENERATED_BODY()
	
	UAbility_Meelee_Combo();

protected:

	TArray<FName> MontageSectionNames;
	int32 SectionIdx = 0;

	TWeakObjectPtr<class UAbilityTask_WaitGameplayEvent> ReactiveEventTask;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combo", meta = (AllowPrivateAccess = "true"))
	EComboSelectPolicy ComboSelectPolicy;

	UPROPERTY(EditDefaultsOnly, Category = "Combo", meta = (AllowPrivateAccess = "true", EditCondition = "ComboSelectPolicy==EComboSelectPolicy::Menual", EditConditionHides))
	TArray<FName> MenualComboNameArray;

public:
	bool bAttackedTarget;

protected:

	virtual void OnEventReceived(FGameplayEventData EventData) override;

	UFUNCTION()
	virtual void ActNextComboFromAI(FGameplayEventData EventData);

public:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void ReactivateAbility() override;

	virtual bool CanReactivateAbility() const override { return true; };

	
};
