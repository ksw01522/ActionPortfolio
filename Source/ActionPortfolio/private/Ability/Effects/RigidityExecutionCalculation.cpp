// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/RigidityExecutionCalculation.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Character/ActionPortfolioCharacter.h"
#include "ActionPortfolio.h"

URigidityExecutionCalculation::URigidityExecutionCalculation()
{
#if WITH_EDITORONLY_DATA
	ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("Data.RigidityTime"));
#endif
}

void URigidityExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float RigidityTime;
	ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.RigidityTime"), EvaluationParameters, RigidityTime);
	
	if(RigidityTime <= 0) return;

	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActionPortfolioCharacter* SourceActor = SourceAbilitySystemComponent ? Cast<AActionPortfolioCharacter>(SourceAbilitySystemComponent->GetAvatarActor()) : nullptr;
	AActionPortfolioCharacter* TargetActor = TargetAbilitySystemComponent ? Cast<AActionPortfolioCharacter>(TargetAbilitySystemComponent->GetAvatarActor()) : nullptr;

	if (TargetActor) {
		TargetActor->CharacterRigidity(RigidityTime);
		TargetActor->HitReact(EHitReactionDirection::Front, false);
	}
}
