// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Tasks/AbilityTask_GrantTags.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "ActionPortfolio.h"

void UAbilityTask_GrantTags::Activate()
{
	AbilitySystemComponent->AddLooseGameplayTags(TagsForGrant);
}

void UAbilityTask_GrantTags::OnDestroy(bool AbilityEnding)
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->RemoveLooseGameplayTags(TagsForGrant);
	}

	Super::OnDestroy(AbilityEnding);
}

UAbilityTask_GrantTags* UAbilityTask_GrantTags::GrantTags(UGameplayAbility* OwningAbility, FName TaskName, FGameplayTagContainer GrantTags)
{
	UAbilityTask_GrantTags* ReturnTask = NewAbilityTask<UAbilityTask_GrantTags>(OwningAbility, TaskName);
	ReturnTask->TagsForGrant = GrantTags;

	return ReturnTask;
}
