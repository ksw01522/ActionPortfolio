// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "Character/ActionPortfolioCharacter.h"
#include "AbilitySystemGlobals.h"
#include "ActionPortfolio.h"



UActionPFAbilitySystemComponent::UActionPFAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionPFAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UActionPFGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
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

bool UActionPFAbilitySystemComponent::GetCooldownRemainingAndDurationByTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration)
{
	if (CooldownTags.Num() <= 0) {
		#if WITH_EDITOR
		PFLOG(Warning, TEXT("Called By Empty Tag"));
		#endif
		return false;
	}

	TimeRemaining = -1;
	CooldownDuration = -1;

	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
	TArray< TPair<float, float> > DurationAndTimeRemaining = GetActiveEffectsTimeRemainingAndDuration(Query);
	if (DurationAndTimeRemaining.Num() <= 0) {
		return false;
	}

	int32 LongestIDX = 0;
	float LongestTime = DurationAndTimeRemaining[0].Key;
	for (int Idx = 1; Idx < DurationAndTimeRemaining.Num(); Idx++)
	{
		if(DurationAndTimeRemaining[Idx].Key <= LongestTime) continue;

		LongestIDX = Idx;
		LongestTime = DurationAndTimeRemaining[Idx].Key;
	}

	TimeRemaining = DurationAndTimeRemaining[LongestIDX].Key;
	CooldownDuration = DurationAndTimeRemaining[LongestIDX].Value;

	return true;
}




