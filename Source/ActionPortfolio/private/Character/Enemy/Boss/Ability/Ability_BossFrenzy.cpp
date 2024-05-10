// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Boss/Ability/Ability_BossFrenzy.h"
#include "Ability/Tasks/AbilityTask_PlayMontageAndWaitEvent.h"
#include "ActionPortfolio.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameplayEffect.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Character/Enemy/Boss/CharacterBoss.h"
#include "Character/Enemy/Boss/BossAIController.h"

UAbility_BossFrenzy::UAbility_BossFrenzy()
{
}



void UAbility_BossFrenzy::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	/*
	UAbilityTask_PlayMontageAndWaitEvent* MeeleeAbilityTask = UAbilityTask_PlayMontageAndWaitEvent::PlayMontageAndWaitEvent(this, TaskInstanceName, MeeleeMontage, EventTagContainer, PlayLate, NAME_None, bStopWhenAbilityEnd);
	ensure(MeeleeAbilityTask);
	MeeleeAbilityTask->OnCompleted.AddDynamic(this, &UAbility_BossFrenzy::OnMontageEnded);
	MeeleeAbilityTask->OnBlendOut.AddDynamic(this, &UAbility_BossFrenzy::OnMontageEnded);
	MeeleeAbilityTask->OnInterrupted.AddDynamic(this, &UAbility_BossFrenzy::OnMontageCancelled);
	MeeleeAbilityTask->OnCancelled.AddDynamic(this, &UAbility_BossFrenzy::OnMontageCancelled);
	MeeleeAbilityTask->EventReceived.AddDynamic(this, &UAbility_BossFrenzy::OnEventReceived);

	MeeleeAbilityTask->ReadyForActivation();

	if (bBlockMoveInActing)
	{
		GetCurrentActorInfo()->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.BlockMove"));
		OnGameplayAbilityEnded.AddUObject(this, &UAbility_BossFrenzy::OffBlockMove);
	}

	RemainFrenzyCount = FMath::RandRange(MinFrenzyCount, MaxFrenzyCount);

	UAbilityTask_WaitGameplayEvent* WaitFrenzyEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Ability.Boss.Frenzy.Check"));
	WaitFrenzyEvent->EventReceived.AddDynamic(this, &UAbility_BossFrenzy::CheckFrenzyCount);
	WaitFrenzyEvent->ReadyForActivation();

	RotateToTarget();
	*/
}

#if WITH_EDITOR

void UAbility_BossFrenzy::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UAbility_BossFrenzy, MinFrenzyCount) || PropertyName == GET_MEMBER_NAME_CHECKED(UAbility_BossFrenzy, MaxFrenzyCount))
	{
		if (MaxFrenzyCount < MinFrenzyCount) {
			MaxFrenzyCount = MinFrenzyCount;
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif

void UAbility_BossFrenzy::CheckFrenzyCount(FGameplayEventData EventData)
{
	if (--RemainFrenzyCount <= 0)
	{
		MontageJumpToSection("Discharge");

		RemainFrenzyCount = FMath::RandRange(MinFrenzyCount, MaxFrenzyCount);
	}
	else
	{
		RotateToTarget();
	}
}

void UAbility_BossFrenzy::RotateToTarget()
{
	ACharacterBoss* Boss = Cast<ACharacterBoss>(GetAvatarActorFromActorInfo());
	if(!IsValid(Boss)) return;

	AActionPortfolioCharacter* Target = Boss->GetController<ABossAIController>()->GetCurrentFocusedTarget();

	if (IsValid(Target))
	{
		FVector ToTargetVector = Target->GetActorLocation() - Boss->GetActorLocation();
		ToTargetVector.Z = 0;

		FRotator ToTargetRotation = ToTargetVector.Rotation();
		ToTargetRotation.Pitch = 0;
		ToTargetRotation.Roll = 0;

		Boss->SetActorRotation(ToTargetRotation);
	}
}
