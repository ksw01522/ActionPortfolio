// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/GameplayAbility_Meelee.h"
#include "Ability/Tasks/AbilityTask_PlayMontageAndWaitEvent.h"
#include "ActionPortfolio.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameplayEffect.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Ability/Effects/KnockbackExecutionCalculation.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Ability/Effects/GameplayEffect_Damage.h"
#include "Character/ActionPortfolioCharacter.h"

UGameplayAbility_Meelee::UGameplayAbility_Meelee() : Super()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	bStopWhenAbilityEnd = true;
	PlayLate = 1.f;
}

void UGameplayAbility_Meelee::OnEventRecevied(FGameplayEventData EventData)
{
	FActionPFDamageStruct* DamageStruct = DamageMap.Find(EventData.EventTag);
	if(DamageStruct == nullptr || EventData.Instigator.IsNull() || EventData.Target.IsNull()) return;

	UActionPFAbilitySystemComponent* SourceAbilitySystem = Cast<UActionPFAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	UActionPFAbilitySystemComponent* TargetAbilitySystem = nullptr;

	const IAbilitySystemInterface* Inter = Cast<const IAbilitySystemInterface>(EventData.Target.Get());
	if(Inter == nullptr) return;

	TargetAbilitySystem = Cast<UActionPFAbilitySystemComponent>(Inter->GetAbilitySystemComponent());

	if (!IsValid(SourceAbilitySystem) || !IsValid(TargetAbilitySystem)) return;
	AActionPortfolioCharacter* TargetCharacter = Cast<AActionPortfolioCharacter>(TargetAbilitySystem->GetOwnerActor());


	if (DamageStruct->DamageEffectClass.Get() != nullptr) {

		FGameplayEffectContextHandle EffectContext = SourceAbilitySystem->MakeEffectContext();
		EffectContext.AddSourceObject(GetCurrentSourceObject());
		
		FGameplayEffectSpecHandle NewHandle = SourceAbilitySystem->MakeOutgoingSpec(DamageStruct->DamageEffectClass, GetAbilityLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = SourceAbilitySystem->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), TargetAbilitySystem);
		}
		
	}

	bool bForceDown = false;

	EHitReactionDirection HitReactionDir = EHitReactionDirection::Front;

	if (DamageStruct->Knockback != nullptr) {
		DamageStruct->Knockback->KnockbackEffect(EventData.Instigator.Get(), EventData.Target.Get());
		bForceDown = DamageStruct->Knockback->IsForceDown();

		if (TargetCharacter) {
			FVector KnockbackDir = DamageStruct->Knockback->GetKnockbackVector(EventData.Instigator.Get(), EventData.Target.Get());
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
			else if(DotP <= -0.707)
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
		}

		
	}

	

	if (TargetCharacter) {
		TargetCharacter->CharacterRigidity(DamageStruct->RigidityTime);
		TargetCharacter->HitReact(HitReactionDir, bForceDown);
	}
}

void UGameplayAbility_Meelee::OnMontageEnded(FGameplayEventData EventData)
{
	check(CurrentActorInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Meelee::OnMontageCancelled(FGameplayEventData EventData)
{
	check(CurrentActorInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Meelee::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) 
	{ 
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true); 
		return;
	}
	
	UAbilityTask_PlayMontageAndWaitEvent* MeeleeAbilityTask = UAbilityTask_PlayMontageAndWaitEvent::PlayMontageAndWaitEvent(this, TaskInstanceName, MeeleeMontage, EventTagContainer, PlayLate, NAME_None, bStopWhenAbilityEnd);
	ensure(MeeleeAbilityTask);
	MeeleeAbilityTask->OnCompleted.AddDynamic(this, &UGameplayAbility_Meelee::OnMontageEnded);
	MeeleeAbilityTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_Meelee::OnMontageEnded);
	MeeleeAbilityTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_Meelee::OnMontageCancelled);
	MeeleeAbilityTask->OnCancelled.AddDynamic(this, &UGameplayAbility_Meelee::OnMontageCancelled);
	MeeleeAbilityTask->EventReceived.AddDynamic(this, &UGameplayAbility_Meelee::OnEventRecevied);

	MeeleeAbilityTask->ReadyForActivation();
}

bool UGameplayAbility_Meelee::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;
	return MeeleeMontage != nullptr;
}

