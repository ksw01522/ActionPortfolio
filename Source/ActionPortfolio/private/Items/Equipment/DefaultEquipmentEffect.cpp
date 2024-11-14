// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equipment/DefaultEquipmentEffect.h"
#include "Ability/ActionPFAttributeSet.h"

UDefaultEquipmentEffect::UDefaultEquipmentEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Modifiers.SetNum(4);

	//MaxHP¼³Á¤
	FGameplayModifierInfo& MaxHPInfo = Modifiers[0];
	MaxHPInfo.Attribute = UCharacterAttributeSet::GetMaxHealthAttribute();
	MaxHPInfo.ModifierOp = EGameplayModOp::Additive;
	FSetByCallerFloat MaxHPCaller;
	MaxHPCaller.DataName = "MaxHP";
	MaxHPInfo.ModifierMagnitude = MaxHPCaller;

	//MaxStamina
	FGameplayModifierInfo& MaxStaminaInfo = Modifiers[1];
	MaxStaminaInfo.Attribute = UCharacterAttributeSet::GetMaxStaminaAttribute();
	MaxStaminaInfo.ModifierOp = EGameplayModOp::Additive;
	FSetByCallerFloat MaxStaminaCaller;
	MaxStaminaCaller.DataName = "MaxStamina";
	MaxStaminaInfo.ModifierMagnitude = MaxStaminaCaller;

	//AttackP
	FGameplayModifierInfo& AttackPInfo = Modifiers[2];
	AttackPInfo.Attribute = UCharacterAttributeSet::GetAttackPAttribute();
	AttackPInfo.ModifierOp = EGameplayModOp::Additive;
	FSetByCallerFloat AttackPCaller;
	AttackPCaller.DataName = "AttackP";
	AttackPInfo.ModifierMagnitude = AttackPCaller;

	//DefenseP
	FGameplayModifierInfo& DefensePInfo = Modifiers[3];
	DefensePInfo.Attribute = UCharacterAttributeSet::GetDefensePAttribute();
	DefensePInfo.ModifierOp = EGameplayModOp::Additive;
	FSetByCallerFloat DefensePCaller;
	DefensePCaller.DataName = "DefenseP";
	DefensePInfo.ModifierMagnitude = DefensePCaller;
}
