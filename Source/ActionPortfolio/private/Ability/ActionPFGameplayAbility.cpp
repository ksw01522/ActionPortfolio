// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ActionPFGameplayAbility.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "ActionPortfolio.h"
#include "Character/ActionPortfolioCharacter.h"

TArray<FGameplayEffectSpecHandle> UActionPFGameplayAbility::MakeEffectSpecHandle(TArray<TSubclassOf<UGameplayEffect>> ArrayEffectClass)
{
	TArray<FGameplayEffectSpecHandle> ReturnArray;

	for (auto EffectClass : ArrayEffectClass)
	{
		ReturnArray.Add(MakeOutgoingGameplayEffectSpec(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectClass, GetAbilityLevel()));
	}

	return ReturnArray;
}

void UActionPFGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (bHasBlueprintActivate)
	{
		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
		K2_ActivateAbility();
	}
	else if (bHasBlueprintActivateFromEvent)
	{
		if (TriggerEventData)
		{
			// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
			K2_ActivateAbilityFromEvent(*TriggerEventData);
		}
		else
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
	}
	else
	{
		ActivateAbility_CPP(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}

}

void UActionPFGameplayAbility::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

}

void UActionPFGameplayAbility::ReactivateAbility()
{
	
}
