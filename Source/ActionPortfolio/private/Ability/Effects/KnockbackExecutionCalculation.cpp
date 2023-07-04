// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/KnockbackExecutionCalculation.h"
#include "GameplayEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"

#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Character/ActionPortfolioCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "ActionPortfolio.h"

UKnockbackExecutionCalculation::UKnockbackExecutionCalculation()
{
}

void UKnockbackExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActionPortfolioCharacter* TargetCharacter = Cast<AActionPortfolioCharacter>(TargetActor);
	if(!IsValid(TargetCharacter)) return;
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FVector KnockbackVector = MakeKnockbackVector(ExecutionParams);

	EHitReactionDirection HitReactionDir = EHitReactionDirection::Front;


	if (!KnockbackVector.IsNearlyZero())
	{
		FVector KnockbackDir = KnockbackVector;
		KnockbackDir.Z = 0;
		KnockbackDir.Normalize();

		bool bIsRight = true;

		FVector TargetForward = TargetCharacter->GetActorForwardVector();
		TargetForward.Z = 0;
		TargetForward.Normalize();

		float DotP = FVector::DotProduct(KnockbackDir, TargetForward);
		if (0.707 <= DotP) {
			HitReactionDir = EHitReactionDirection::Back;
		}
		else if (DotP <= -0.707)
		{
			HitReactionDir = EHitReactionDirection::Front;
		}
		else
		{
			FVector Cross = FVector::CrossProduct(TargetForward, KnockbackDir);

			double temp = FVector::DotProduct(Cross, FVector::UpVector);
			bIsRight = 0 < temp;

			if (bIsRight) {
				HitReactionDir = EHitReactionDirection::Left;
			}
			else {
				HitReactionDir = EHitReactionDirection::Right;
			}
		}

		FVector CurrentVel = TargetCharacter->GetCharacterMovement()->Velocity;
		TargetCharacter->GetCharacterMovement()->AddImpulse(KnockbackVector - CurrentVel, true);
	}

	TargetCharacter->HitReact(HitReactionDir, KnockbackVector.Z != 0);
}

UKncokback_Forward::UKncokback_Forward()
{
	ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("Data.Knockback.Forward.Strength"));
	ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("Data.Knockback.Forward.ZStrength"));
	ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("Data.Knockback.Forward.bForceDown"));
}

FVector UKncokback_Forward::MakeKnockbackVector(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	FVector KnockbackVector;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	AActor* EffectInstigator = Spec.GetEffectContext().GetInstigator();
	
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActionPortfolioCharacter* TargetCharacter = Cast<AActionPortfolioCharacter>(TargetActor);

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Strength;
	ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Knockback.Forward.Strength"), EvaluationParameters, Strength);
	
	float bForceDown = 0;
	ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Knockback.Forward.bForceDown"), EvaluationParameters, bForceDown);

	KnockbackVector = EffectInstigator->GetActorForwardVector();

	KnockbackVector *= Strength;
	if (0 < bForceDown || TargetCharacter->GetCharacterMovement()->IsFalling()) {
		float ZStrength;
		ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Knockback.Forward.ZStrength"), EvaluationParameters, ZStrength);

		KnockbackVector.Z = ZStrength;
	}

	return KnockbackVector;
}
