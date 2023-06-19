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
	ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	FVector KnockbackVector(0,0,0);
	KnockbackVector.X = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.KnockbackVector.X"), false, 0), 0);
	KnockbackVector.Y = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.KnockbackVector.Y"), false, 0), 0);
	KnockbackVector.Z = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.KnockbackVector.Z"), false, 0), 0);



	if (!KnockbackVector.IsNearlyZero())
	{
		FVector CurrentVel = TargetCharacter->GetCharacterMovement()->Velocity;
		TargetCharacter->GetCharacterMovement()->AddImpulse(KnockbackVector - CurrentVel, true);
	}
}


FVector UKncokback_Forward::GetKnockbackVector(const AActor* Instigator, const AActor* Target)
{
	const ACharacter* TargetCharacter = Cast<ACharacter>(Target);

	FVector Direction = Instigator->GetActorForwardVector();
	Direction.Z = 0;
	Direction.Normalize();
	Direction = Direction * KnockbackStrength;
	if (bForceDown || TargetCharacter->GetCharacterMovement()->IsFalling()) Direction.Z = FlyStrength;

	return Direction;
}


void UActionPFKnockbackObject::KnockbackEffect(const AActor* Instigator, const AActor* Target)
{
	UAbilitySystemComponent* SourceAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Instigator);
	UAbilitySystemComponent* TargetAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if(!IsValid(SourceAbilitySystem) || !IsValid(TargetAbilitySystem)) return;

	FVector KnockbackVector = GetKnockbackVector(Instigator, Target);
	UGameplayEffect* KnockbackEffect = NewObject<UGameplayEffect>(GetTransientPackage(), "Knockback");

	FGameplayEffectExecutionDefinition KnockbackEffectExecution;
	KnockbackEffectExecution.CalculationClass = UKnockbackExecutionCalculation::StaticClass();
	KnockbackEffect->Executions.Add(KnockbackEffectExecution);

	KnockbackEffect->ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Knockback"));

	FGameplayEffectContextHandle EffectContext = SourceAbilitySystem->MakeEffectContext();
	EffectContext.AddSourceObject(SourceAbilitySystem->GetOwnerActor());

	FGameplayEffectSpec* NewSpec = new FGameplayEffectSpec(KnockbackEffect, EffectContext, 1);
	NewSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.KnockbackVector.X"), KnockbackVector.X);
	NewSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.KnockbackVector.Y"), KnockbackVector.Y);
	NewSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.KnockbackVector.Z"), KnockbackVector.Z);

	FGameplayEffectSpecHandle NewHandle(NewSpec); 


	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = SourceAbilitySystem->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), TargetAbilitySystem);
	}
}
