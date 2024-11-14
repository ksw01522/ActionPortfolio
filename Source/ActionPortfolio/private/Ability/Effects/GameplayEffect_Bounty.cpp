// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/GameplayEffect_Bounty.h"
#include "Ability/ActionPFAttributeSet.h"

UGameplayEffect_Bounty::UGameplayEffect_Bounty()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	//°æÇèÄ¡ È¹µæ
	{
		FGameplayModifierInfo InfoToAdd;

		InfoToAdd.Attribute = UPlayerAttributeSet::GetXPAttribute();
		InfoToAdd.ModifierOp = EGameplayModOp::Additive;

		FSetByCallerFloat Caller;

		Caller.DataName = UPlayerAttributeSet::GetXPName();

		InfoToAdd.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);
		Modifiers.Add(InfoToAdd);
	}

	//°ñµå È¹µæ
	{
		FGameplayModifierInfo InfoToAdd;

		InfoToAdd.Attribute = UPlayerAttributeSet::GetGoldAttribute();
		InfoToAdd.ModifierOp = EGameplayModOp::Additive;

		FSetByCallerFloat Caller;

		Caller.DataName = UPlayerAttributeSet::GetGoldName();

		InfoToAdd.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);
		Modifiers.Add(InfoToAdd);
	}
}
