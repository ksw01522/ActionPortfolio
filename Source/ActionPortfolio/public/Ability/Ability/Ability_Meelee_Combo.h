// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/GameplayAbility_Meelee.h"
#include "Ability_Meelee_Combo.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UAbility_Meelee_Combo : public UGameplayAbility_Meelee
{
	GENERATED_BODY()
	
protected:

	TArray<FName> MontageSectionNames;
	int32 SectionIdx = 0;

	TWeakObjectPtr<class UAbilityTask_WaitGameplayEvent> ReactiveEventTask;

public:
	bool bActivateFromAI;
	bool bCanNextComboFromAI;

protected:

	virtual void OnEventReceived(FGameplayEventData EventData) override;


public:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void ReactivateAbility();

	virtual bool CanReactivateAbility() const override { return true; };

};
