// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/StateEffects/HPHealEffect.h"
#include "Ability/ActionPFAttributeSet.h"

UHPHealEffect_Burst::UHPHealEffect_Burst()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FSetByCallerFloat HealAmount;
	HealAmount.DataTag = FGameplayTag::RequestGameplayTag("Data.Amount");

	FGameplayModifierInfo CurrentHPInfo;
	CurrentHPInfo.Attribute = UCharacterAttributeSet::GetHealthAttribute();
	CurrentHPInfo.ModifierOp = EGameplayModOp::Additive;
	CurrentHPInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(HealAmount);

	Modifiers.Add(CurrentHPInfo);


	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));


	GameplayCues.Add(FGameplayEffectCue(FGameplayTag::RequestGameplayTag("GameplayCue.HPHeal.Burst"), 0, 0));
}
