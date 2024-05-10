// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Ability_Knockback.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "GameFramework/RootMotionSource.h"
#include "ActionPortfolio.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemGlobals.h"

UAbility_Knockback::UAbility_Knockback()
{
	FGameplayTag KnockbackTag = FGameplayTag::RequestGameplayTag("CommonEvent.Knockback");

	AbilityTags.AddTag(KnockbackTag);

	CancelAbilitiesWithTag.AddTag(KnockbackTag);

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Down"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Knockback"));

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UAbility_Knockback::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	
}

void UAbility_Knockback::OnKnockbackFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

UKnockback_Forward::UKnockback_Forward()
{

}

void UKnockback_Forward::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const AActor* ImpactActor = TriggerEventData != nullptr ?	TriggerEventData->Instigator : 
																Cast<AActor>(GetSourceObject(Handle, ActorInfo));
	check(ImpactActor);

	bool bIsInAir = 0 < ActorInfo->AbilitySystemComponent->GetGameplayTagCount(FGameplayTag::RequestGameplayTag("State.Etc.IsInAir"));

	FVector KnockbackVector = bIsInAir ? ImpactActor->GetActorForwardVector() * PowerInAir.X + FVector::ZAxisVector * PowerInAir.Y
		: ImpactActor->GetActorForwardVector() * PowerOnGround.X + FVector::ZAxisVector * PowerOnGround.Y;

	if (UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent))
	{
		CharacterMovement->AddImpulse(-CharacterMovement->Velocity, true);
		CharacterMovement->AddImpulse(KnockbackVector, true);
	}
	/*무브먼트 다른거 사용시
	else if(Cast<>(ActorInfo->MovementComponent))
	{
	
	}
	
	*/

	FGameplayEventData EventDataToASC;
	UKnockbackEventDataToASC* KnockbackDataToASC = NewObject<UKnockbackEventDataToASC>();
	KnockbackDataToASC->KnockbackVector = KnockbackVector;
	KnockbackDataToASC->SourceObject =  GetSourceObject(Handle, ActorInfo);
	EventDataToASC.OptionalObject = KnockbackDataToASC;

	ActorInfo->AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("CommonEvent.Knockback"), &EventDataToASC);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UKnockback_Radial::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const AActor* ImpactActor = TriggerEventData != nullptr ? TriggerEventData->Instigator :
		Cast<AActor>(GetSourceObject(Handle, ActorInfo));
	check(ImpactActor);

	FVector Direction = GetAvatarActorFromActorInfo()->GetActorLocation() - ImpactActor->GetActorLocation();
	Direction.Normalize();

	bool bIsInAir = 0 < ActorInfo->AbilitySystemComponent->GetGameplayTagCount(FGameplayTag::RequestGameplayTag("State.Etc.IsInAir"));

	FVector KnockbackVector = bIsInAir ? Direction * PowerInAir.X + FVector::ZAxisVector * PowerInAir.Y
		: Direction * PowerOnGround.X + FVector::ZAxisVector * PowerOnGround.Y;

	if (UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent))
	{
		CharacterMovement->AddImpulse(-CharacterMovement->Velocity, true);
		CharacterMovement->AddImpulse(KnockbackVector, true);
	}
	/*무브먼트 다른거 사용시
	else if(Cast<>(ActorInfo->MovementComponent))
	{

	}

	*/

	FGameplayEventData EventDataToASC;
	UKnockbackEventDataToASC* KnockbackDataToASC = NewObject<UKnockbackEventDataToASC>();
	KnockbackDataToASC->KnockbackVector = KnockbackVector;
	KnockbackDataToASC->SourceObject = GetSourceObject(Handle, ActorInfo);
	EventDataToASC.OptionalObject = KnockbackDataToASC;

	ActorInfo->AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("CommonEvent.Knockback"), &EventDataToASC);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
