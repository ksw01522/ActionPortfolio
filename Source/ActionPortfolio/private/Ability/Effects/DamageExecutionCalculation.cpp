// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/DamageExecutionCalculation.h"
#include "Ability/ActionPFAttributeSet.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "ActionPortfolio.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/ActionPortfolioCharacter.h"

struct ActionPFDamageStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefenseP);

	ActionPFDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UActionPFAttributeSet, Damage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UActionPFAttributeSet, DefenseP, Target, false);
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
	ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("DamageType.Meelee"));
	ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("DamageType.fire"));
	ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("DamageType.Ice"));
	ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("DamageType.Electric"));
#endif

	RelevantAttributesToCapture.Add(DamageStatics().DefensePDef);
}

void UDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	
	AActionPortfolioCharacter* SourceCharacter = Cast<AActionPortfolioCharacter>(SourceActor);
	AActionPortfolioCharacter* TargetCharacter = Cast<AActionPortfolioCharacter>(TargetActor);
	

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float TempDamage = 0;
	float TotalDamage = 0;
	if(ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("DamageType.Meelee"), EvaluationParameters, TempDamage)) 
	{
		TotalDamage += FMath::Max<float>(TempDamage, 0);
	}

	float TempDefenceP = 0;
	if(ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(DamageStatics().DefensePDef, TempDefenceP))
	{
		
	}

	if (ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("DamageType.Fire"), EvaluationParameters, TempDamage))
	{
		TotalDamage += FMath::Max<float>(TempDamage, 0);
	}

	if (ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("DamageType.Ice"), EvaluationParameters, TempDamage))
	{
		TotalDamage += FMath::Max<float>(TempDamage, 0);
	}

	if (ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("DamageType.Electric"), EvaluationParameters, TempDamage))
	{
		TotalDamage += FMath::Max<float>(TempDamage, 0);
	}


	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, TotalDamage));

	if (TotalDamage > 0.f)
	{
	}

	TargetCharacter->OnDamageEvent(TotalDamage, SourceActor);
	SourceCharacter->OnAttackEvent(TotalDamage, TargetCharacter);
}
