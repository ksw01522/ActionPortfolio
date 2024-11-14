// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/CharacterRegenEffect.h"
#include "Ability/Effects/ExecutionCalculation/StaminaRegenExecutionCalculation.h"
#include "Ability/Effects/ExecutionCalculation/HealthRegenExecutionCalculation.h"
#include "Ability/ActionPFAttributeSet.h"

UCharacterRegenEffect::UCharacterRegenEffect()
{
	Period = 1;
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	//Basic HealthRegen
	FGameplayEffectExecutionDefinition HealthExecutionDef;
	HealthExecutionDef.CalculationClass = UHealthRegenExecutionCalculation::StaticClass();

	FGameplayEffectExecutionScopedModifierInfo HealthRegenCaptureInfo(FGameplayEffectAttributeCaptureDefinition(UCharacterAttributeSet::GetHealthRegenAttribute(), EGameplayEffectAttributeCaptureSource::Source, false));
	//HealthRegenCaptureInfo.TargetTags.IgnoreTags = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("StateEffect.Debuff.HealBan"));

	HealthExecutionDef.CalculationModifiers.Add(HealthRegenCaptureInfo);

	Executions.Add(HealthExecutionDef);

	//Basic StaminaRegen
	FGameplayEffectExecutionDefinition StaminaExecutionDef;
	StaminaExecutionDef.CalculationClass = UStaminaRegenExecutionCalculation::StaticClass();

	FGameplayEffectExecutionScopedModifierInfo StaminaRegenCaptureInfo(FGameplayEffectAttributeCaptureDefinition(UCharacterAttributeSet::GetStaminaRegenAttribute(), EGameplayEffectAttributeCaptureSource::Source, false));
	
	StaminaExecutionDef.CalculationModifiers.Add(StaminaRegenCaptureInfo);

	Executions.Add(StaminaExecutionDef);
}
