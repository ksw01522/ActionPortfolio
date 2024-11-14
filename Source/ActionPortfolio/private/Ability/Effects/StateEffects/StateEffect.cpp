// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/StateEffects/StateEffect.h"
#include "Ability/Effects/Damage/DamageExecutionCalculation.h"
#include "Ability/ActionPFAttributeSet.h"

UStateEffect::UStateEffect()
{
	Period = 0;
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat DurationSetByCaller;
	DurationSetByCaller.DataTag = FGameplayTag::RequestGameplayTag("Data.Duration");

	DurationMagnitude = FGameplayEffectModifierMagnitude(DurationSetByCaller);

	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));

}



UDebuffEffect_Poison::UDebuffEffect_Poison()
{
	Period = 1;
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag("StateEffect.Debuff.Poison"));

	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("StateEffect.Debuff.Poison"));

	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.StateEffect.Poison"));

	FGameplayEffectCue NewEffectCue;
	NewEffectCue.GameplayCueTags.AddTag(FGameplayTag::RequestGameplayTag("GameplayCue.Debuff.Poison.Effect"));
	NewEffectCue.GameplayCueTags.AddTag(FGameplayTag::RequestGameplayTag("GameplayCue.Debuff.Poisn.Icon"));

	GameplayCues.Add(NewEffectCue);
	//데미지 연산 식 추가
	{
		FGameplayEffectExecutionDefinition DamageExecution;
		DamageExecution.CalculationClass = UDamageExecutionCalculation_Poison::StaticClass();

		FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag("Data.Damage");

		FGameplayEffectExecutionScopedModifierInfo DEMI(DamageTag);
		DEMI.ModifierOp = EGameplayModOp::Additive;

		FSetByCallerFloat DamageCaller;
		DamageCaller.DataTag = DamageTag;

		DEMI.ModifierMagnitude = FGameplayEffectModifierMagnitude(DamageCaller);

		//데미지 면역
		DEMI.TargetTags.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Damage.All"));
		DEMI.TargetTags.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Damage.Poison"));

		DamageExecution.CalculationModifiers.Add(DEMI);

		Executions.Add(DamageExecution);
	}
}

UDebuffEffect_Stun::UDebuffEffect_Stun()
{
	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag("StateEffect.Debuff.Stun"));

	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("StateEffect.Debuff.Stun"));

	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.StateEffect.Stun"));

	FGameplayEffectCue NewEffectCue;
	NewEffectCue.GameplayCueTags.AddTag(FGameplayTag::RequestGameplayTag("GameplayCue.Debuff.Stun.Effect"));
	NewEffectCue.GameplayCueTags.AddTag(FGameplayTag::RequestGameplayTag("GameplayCue.Debuff.Stun.Icon"));

	GameplayCues.Add(NewEffectCue);
}
