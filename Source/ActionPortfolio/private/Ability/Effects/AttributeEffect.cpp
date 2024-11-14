// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/AttributeEffect.h"
#include "Ability/ActionPFAttributeSet.h"

#define ADD_ATTRIBUTE_MODIFER(ClassName, PropertyName) \
{\
	FGameplayModifierInfo InfoToAdd;\
	\
	InfoToAdd.Attribute = ClassName::Get##PropertyName##Attribute(); \
	InfoToAdd.ModifierOp = EGameplayModOp::Override; \
	\
	FSetByCallerFloat Caller; \
	\
	Caller.DataName = ClassName::Get##PropertyName##Name(); \
	\
	InfoToAdd.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller); \
	Modifiers.Add(InfoToAdd); \
}


UAttributeEffect_Base::UAttributeEffect_Base()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	ADD_ATTRIBUTE_MODIFER(UCharacterAttributeSet, CharacterLevel)
	ADD_ATTRIBUTE_MODIFER(UCharacterAttributeSet, MaxHealth)
	ADD_ATTRIBUTE_MODIFER(UCharacterAttributeSet, HealthRegen)
	ADD_ATTRIBUTE_MODIFER(UCharacterAttributeSet, MaxStamina)
	ADD_ATTRIBUTE_MODIFER(UCharacterAttributeSet, StaminaRegen)
	ADD_ATTRIBUTE_MODIFER(UCharacterAttributeSet, AttackP)
	ADD_ATTRIBUTE_MODIFER(UCharacterAttributeSet, DefenseP)
	ADD_ATTRIBUTE_MODIFER(UCharacterAttributeSet, FireResistance)
	ADD_ATTRIBUTE_MODIFER(UCharacterAttributeSet, IceResistance)
	ADD_ATTRIBUTE_MODIFER(UCharacterAttributeSet, ElectricResistance)
}

UAttributeEffect_Enemy::UAttributeEffect_Enemy()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	ADD_ATTRIBUTE_MODIFER(UEnemyAttributeSet, BountyXP)
	ADD_ATTRIBUTE_MODIFER(UEnemyAttributeSet, BountyGold)
}



UAttributeEffect_Player::UAttributeEffect_Player()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	ADD_ATTRIBUTE_MODIFER(UPlayerAttributeSet, PlayerLevel)
	ADD_ATTRIBUTE_MODIFER(UPlayerAttributeSet, XP)
	ADD_ATTRIBUTE_MODIFER(UPlayerAttributeSet, XPForLevelUp)
}

#undef ADD_ATTIRBUTE_MODIFER