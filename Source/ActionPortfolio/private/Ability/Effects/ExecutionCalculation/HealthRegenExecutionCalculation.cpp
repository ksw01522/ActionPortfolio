// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/ExecutionCalculation/HealthRegenExecutionCalculation.h"
#include "ActionPortfolio.h"
#include "Ability/ActionPFAttributeSet.h"
#include "Ability/ActionPFAbilitySystemComponent.h"


struct FHealthRegenStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthRegen);

	FHealthRegenStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, HealthRegen, Source, false);
	}

};

static const FHealthRegenStatics& HealthRegenStatics()
{
	static FHealthRegenStatics HRStatics;
	return HRStatics;
}

UHealthRegenExecutionCalculation::UHealthRegenExecutionCalculation()
{
	RelevantAttributesToCapture.Add(FHealthRegenStatics().HealthRegenDef);
}

void UHealthRegenExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters Aggregator;
	Aggregator.SourceTags = TargetTags;

	float RegenValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealthRegenStatics().HealthRegenDef, Aggregator, RegenValue);

	if (0 < RegenValue)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCharacterAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, RegenValue));
	}
}
