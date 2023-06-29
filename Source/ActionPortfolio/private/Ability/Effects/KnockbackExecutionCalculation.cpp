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

	FVector KnockbackVector(0,0,0);
	KnockbackVector.X = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.KnockbackVector.X"), false, 0), 0);
	KnockbackVector.Y = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.KnockbackVector.Y"), false, 0), 0);
	KnockbackVector.Z = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.KnockbackVector.Z"), false, 0), 0);
	float ForceDown = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.KnockbackVector.ForceDown"), false);
	if(ForceDown <= 0 && !TargetCharacter->GetCharacterMovement()->IsFalling()) KnockbackVector.Z = 0;

	EHitReactionDirection HitReactionDir = EHitReactionDirection::Front;

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


	if (!KnockbackVector.IsNearlyZero())
	{
		FVector CurrentVel = TargetCharacter->GetCharacterMovement()->Velocity;
		TargetCharacter->GetCharacterMovement()->AddImpulse(KnockbackVector - CurrentVel, true);
	}

	TargetCharacter->HitReact(HitReactionDir, KnockbackVector.Z != 0);
}

FVector UKnockbackExecutionCalculation::MakeKnockbackVector() const
{
	return FVector(0, 0, 0);
}


FVector UKncokback_Forward::GetKnockbackVector(const AActor* Instigator, const AActor* Target) const
{
	const ACharacter* TargetCharacter = Cast<ACharacter>(Target);

	FVector Direction = Instigator->GetActorForwardVector();
	Direction.Z = 0;
	Direction.Normalize();
	Direction = Direction * KnockbackStrength;
	Direction.Z = FlyStrength;

	return Direction;
}


void UActionPFKnockbackObject::KnockbackEffect(const AActor* Instigator, const AActor* Target) const
{
	UAbilitySystemComponent* SourceAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Instigator);
	UAbilitySystemComponent* TargetAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (!IsValid(SourceAbilitySystem) || !IsValid(TargetAbilitySystem)) return;

	FGameplayEffectSpecHandle NewHandle = MakeEffectSpecHandle(Instigator, Target);

	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = SourceAbilitySystem->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), TargetAbilitySystem);
	}
}

FGameplayEffectSpecHandle UActionPFKnockbackObject::MakeEffectSpecHandle(const AActor* Instigator, const AActor* Target) const
{
	UAbilitySystemComponent* SourceAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Instigator);
	UAbilitySystemComponent* TargetAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (!IsValid(SourceAbilitySystem) || !IsValid(TargetAbilitySystem)) return FGameplayEffectSpecHandle();

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
	NewSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.KnockbackVector.ForceDown"), bForceDown);

	FGameplayEffectSpecHandle NewHandle(NewSpec);

	return NewHandle;
}
