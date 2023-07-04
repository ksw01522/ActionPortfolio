// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "Character/ActionPortfolioCharacter.h"
#include "AbilitySystemGlobals.h"
#include "ActionPortfolio.h"



UActionPFAbilitySystemComponent::UActionPFAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActionPFAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UActionPFGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UActionPFGameplayAbility>(ActiveAbility));
		}
	}
}

bool UActionPFAbilitySystemComponent::IsActingAbilityByClass(TSubclassOf<UGameplayAbility> CheckAbility)
{
	const UGameplayAbility* AbilityCDO = CheckAbility.GetDefaultObject();

	for (FGameplayAbilitySpec Spec : ActivatableAbilities.Items)
	{
		if (AbilityCDO == Spec.Ability)
		{
			return Spec.IsActive();
		}
	}

	return false;
}

bool UActionPFAbilitySystemComponent::TryActivatePFAbilityByClass(TSubclassOf<class UActionPFGameplayAbility> AbilityClass)
{
	if (IsActingAbilityByClass(AbilityClass))
	{
		FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass);
		if (AbilitySpec == nullptr || AbilitySpec->GetAbilityInstances().IsEmpty()) {
			return false;
		}

		UActionPFGameplayAbility* ActionPFAbilityInstance = Cast<UActionPFGameplayAbility>(AbilitySpec->GetAbilityInstances()[0]);
		if (ActionPFAbilityInstance == nullptr || !ActionPFAbilityInstance->CanReactivateAbility()) {
			return false;
		}

		ActionPFAbilityInstance->ReactivateAbility();
		return true;
	}
	else
	{
		return TryActivateAbilityByClass(AbilityClass);
	}
}



UActionPFAbilitySystemComponent* UActionPFAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	return Cast<UActionPFAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}




