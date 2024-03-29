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

	ActionPFDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UActionPFAttributeSet, Damage, Source, true);
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
	ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("Data.Damage"));
#endif
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

	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage"), EvaluationParameters, Damage);
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage"), false, 0 ), 0 );

	if (Damage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, Damage));
	}
	TargetCharacter->OnDamageEvent(Damage, SourceActor);
	SourceCharacter->OnAttackEvent(Damage, TargetCharacter);

}
