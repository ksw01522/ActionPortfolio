// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/Damage/DamageExecutionCalculation.h"
#include "Ability/ActionPFAttributeSet.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "ActionPortfolio.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/ActionPortfolioCharacter.h"

struct ActionPFDamageStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackP);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefenseP);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IceResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ElectricResistance);

	ActionPFDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, AttackP, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Damage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, DefenseP, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, IceResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, ElectricResistance, Target, false);

	}

};

static const ActionPFDamageStatics& DamageStatics()
{
	static ActionPFDamageStatics DStatics;
	return DStatics;
}

UDamageExecutionCalculation::UDamageExecutionCalculation()
{
#if WITH_EDITORONLY_DATA
	ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("Data.Damage.Absolute"));
	ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("Data.Damage.Relative"));
#endif

	RelevantAttributesToCapture.Add(DamageStatics().AttackPDef);
}

UDamageExecutionCalculation_Meelee::UDamageExecutionCalculation_Meelee()
{
	RelevantAttributesToCapture.Add(DamageStatics().DefensePDef);
}

void UDamageExecutionCalculation_Meelee::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;


	float Damage_Absolute = 0;
	ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage.Absolute"), EvaluationParameters, Damage_Absolute);
	
	float Damage_Relative = 0;
	if (ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage.Relative"), EvaluationParameters, Damage_Relative) && 0 < Damage_Relative)
	{
		float AttackP = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPDef, EvaluationParameters, AttackP);

		Damage_Relative *= AttackP;
	}

	float TotalDamage = Damage_Absolute + Damage_Relative;

	if (0 < TotalDamage)
	{
		float DefenceP = 0;

		if (ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefensePDef, EvaluationParameters, DefenceP))
		{
			//�и� ������ ���� ����

		}
	}

	if (TotalDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, TotalDamage));
	}
}

UDamageExecutionCalculation_Fire::UDamageExecutionCalculation_Fire()
{
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
}

void UDamageExecutionCalculation_Fire::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Damage_Absolute = 0;
	ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage.Absolute"), EvaluationParameters, Damage_Absolute);

	float Damage_Relative = 0;
	if (ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage.Relative"), EvaluationParameters, Damage_Relative) && 0 < Damage_Relative)
	{
		float AttackP = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPDef, EvaluationParameters, AttackP);

		Damage_Relative *= AttackP;
	}

	float TotalDamage = Damage_Absolute + Damage_Relative;

	if(0 < TotalDamage)
	{
		float Resistance = 0;

		if (ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().FireResistanceDef, EvaluationParameters, Resistance))
		{
			//������ ���� ����

		}
	}

	if (0 < TotalDamage)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, TotalDamage));
	}
}

UDamageExecutionCalculation_Ice::UDamageExecutionCalculation_Ice()
{
	RelevantAttributesToCapture.Add(DamageStatics().IceResistanceDef);
}

void UDamageExecutionCalculation_Ice::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Damage_Absolute = 0;
	ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage.Absolute"), EvaluationParameters, Damage_Absolute);

	float Damage_Relative = 0;
	if (ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage.Relative"), EvaluationParameters, Damage_Relative) && 0 < Damage_Relative)
	{
		float AttackP = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPDef, EvaluationParameters, AttackP);

		Damage_Relative *= AttackP;
	}

	float TotalDamage = Damage_Absolute + Damage_Relative;

	if (0 < TotalDamage)
	{
		float Resistance = 0;

		if (ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().IceResistanceDef, EvaluationParameters, Resistance))
		{
			//������ ���� ����

		}
	}

	if (0 < TotalDamage)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, TotalDamage));
	}
}

UDamageExecutionCalculation_Electric::UDamageExecutionCalculation_Electric()
{
	RelevantAttributesToCapture.Add(DamageStatics().ElectricResistanceDef);
}

void UDamageExecutionCalculation_Electric::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Damage_Absolute = 0;
	ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage.Absolute"), EvaluationParameters, Damage_Absolute);

	float Damage_Relative = 0;
	if (ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage.Relative"), EvaluationParameters, Damage_Relative) && 0 < Damage_Relative)
	{
		float AttackP = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPDef, EvaluationParameters, AttackP);

		Damage_Relative *= AttackP;
	}
	float TotalDamage = Damage_Absolute + Damage_Relative;

	if (0 < TotalDamage)
	{
		float Resistance = 0;

		if (ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ElectricResistanceDef, EvaluationParameters, Resistance))
		{
			//������ ���� ����

		}
	}

	if (0 < TotalDamage)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, TotalDamage));
	}
}

UDamageExecutionCalculation_Poison::UDamageExecutionCalculation_Poison()
{
}

void UDamageExecutionCalculation_Poison::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Damage = 0;
	if(ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage.Absolute"), EvaluationParameters, Damage))

	if (Damage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, Damage));
	}
}
