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

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Down"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Rigidity"));
}

void UGameplayAbility_Meelee::OnEventReceived(FGameplayEventData EventData)
{
	FActionPFEffectContainer* EffectsToApply = DamageMap.Find(EventData.EventTag);
	if(EffectsToApply == nullptr || EffectsToApply->EffectClasses.IsEmpty() || EventData.Instigator.IsNull() || EventData.Target.IsNull()) return;

	UActionPFAbilitySystemComponent* SourceAbilitySystem = Cast<UActionPFAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	UActionPFAbilitySystemComponent* TargetAbilitySystem = nullptr;

	const IAbilitySystemInterface* Inter = Cast<const IAbilitySystemInterface>(EventData.Target.Get());
	if(Inter == nullptr) return;

	TargetAbilitySystem = Cast<UActionPFAbilitySystemComponent>(Inter->GetAbilitySystemComponent());

	if (!IsValid(SourceAbilitySystem) || !IsValid(TargetAbilitySystem)) return;
	AActionPortfolioCharacter* SourceCharacter = Cast<AActionPortfolioCharacter>(SourceAbilitySystem->GetOwnerActor());
	AActionPortfolioCharacter* TargetCharacter = Cast<AActionPortfolioCharacter>(TargetAbilitySystem->GetOwnerActor());
	
	if(SourceCharacter->GetTeamAttitudeTowards(*TargetCharacter) == ETeamAttitude::Friendly) return;


	FGameplayEffectContextHandle EffectContext = SourceAbilitySystem->MakeEffectContext();
	EffectContext.AddSourceObject(GetCurrentSourceObject());

	for (auto EffectToApply : EffectsToApply->EffectClasses)
	{
		FGameplayEffectSpecHandle NewHandle = SourceAbilitySystem->MakeOutgoingSpec(EffectToApply, GetAbilityLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = SourceAbilitySystem->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), TargetAbilitySystem);
		}
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

void UGameplayAbility_Meelee::OffBlockMove(UGameplayAbility* Ability)
{
	GetCurrentActorInfo()->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.BlockMove"));
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
	MeeleeAbilityTask->EventReceived.AddDynamic(this, &UGameplayAbility_Meelee::OnEventReceived);

	MeeleeAbilityTask->ReadyForActivation();

	if (bBlockMoveInActing)
	{
		GetCurrentActorInfo()->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.BlockMove"));
		OnGameplayAbilityEnded.AddUObject(this, &UGameplayAbility_Meelee::OffBlockMove);
	}
}

bool UGameplayAbility_Meelee::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;
	return MeeleeMontage != nullptr;
}

