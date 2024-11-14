// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/GameplayEffect_Cooldown.h"

UGameplayEffect_Cooldown::UGameplayEffect_Cooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	FSetByCallerFloat SetByCaller;
	SetByCaller.DataTag = FGameplayTag::RequestGameplayTag(FName("Data.Duration"));
	DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);
}
