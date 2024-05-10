// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ActionPFGameplayAbility.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "ActionPortfolio.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Engine/Texture.h"
#include "Ability/Widget/SAbilityIcon.h"
#include "Ability/Effects/DamageExecutionCalculation.h"

const FGameplayTag UActionPFGameplayAbility::OnAttackStartTag = FGameplayTag::RequestGameplayTag("CommonEvent.OnAttackStart");
const FGameplayTag UActionPFGameplayAbility::OnAttackTargetTag = FGameplayTag::RequestGameplayTag("CommonEvent.OnAttackTarget");
const FGameplayTag UActionPFGameplayAbility::OnDamagedTag = FGameplayTag::RequestGameplayTag("CommonEvent.OnDamaged");

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


void UActionPFGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (AbilityType == EAbilityType::Passive)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UActionPFGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UActionPFGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
												const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE && 0 < CooldownDuration.GetValueAtLevel(GetAbilityLevel()))
	{
		FGameplayEffectSpecHandle CoolDownHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		CoolDownHandle.Data->DynamicGrantedTags.AppendTags(CooldownTags);
		CoolDownHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.CooldownTime")), 
															CooldownDuration.GetValueAtLevel(GetAbilityLevel()));
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, CoolDownHandle);
	}

}

const FGameplayTagContainer* UActionPFGameplayAbility::GetCooldownTags() const
{
	if (!bTempCooldownTagsInitialized)
	{
		const FGameplayTagContainer* ParentTags = Super::GetCooldownTags();
		if (ParentTags)
		{
			const_cast<FGameplayTagContainer&>(TempCooldownTags).AppendTags(*ParentTags);
		}
		const_cast<FGameplayTagContainer&>(TempCooldownTags).AppendTags(CooldownTags);
		const_cast<bool&>(bTempCooldownTagsInitialized) = true;
	}

	return &TempCooldownTags;
}

TSharedPtr<class SAbilityIcon> UActionPFGameplayAbility::CreateAbilityIcon() const
{
	return
	SNew(SAbilityIcon)
	.Ability(const_cast<UActionPFGameplayAbility*>(this));
}


const FSlateBrush* UActionPFGameplayAbility::GetAbilityIconBrush(TWeakObjectPtr<const UAbilitySystemComponent> InSystem) const
{
	return &DefaultIconBrush;
}

