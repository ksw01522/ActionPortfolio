// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/ExecutionCalculation/StaminaRegenExecutionCalculation.h"
#include "ActionPortfolio.h"
#include "Ability/ActionPFAttributeSet.h"
#include "Ability/ActionPFAbilitySystemComponent.h"


struct FStaminaRegenStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(StaminaRegen);

	FStaminaRegenStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, StaminaRegen, Source, false);
	}

};

static const FStaminaRegenStatics& StaminaRegenStatics()
{
	static FStaminaRegenStatics SRStatics;
	return SRStatics;
}


UStaminaRegenExecutionCalculation::UStaminaRegenExecutionCalculation()
{
#if WITH_EDITORONLY_DATA
	
#endif

	RelevantAttributesToCapture.Add(StaminaRegenStatics().StaminaRegenDef);
}

void UStaminaRegenExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* ASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.TargetTags = TargetTags;

	float RegenValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StaminaRegenStatics().StaminaRegenDef, EvaluationParameters, RegenValue);

	if (0 < RegenValue)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCharacterAttributeSet::GetStaminaAttribute(), EGameplayModOp::Additive, RegenValue));
	}
}
