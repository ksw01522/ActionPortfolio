// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Ability/Ability_PlayerPierce.h"
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
#include "Character/Player/PlayerCharacter.h"
#include "GameplayTagContainer.h"
#include "Character/Player/ActionPFPlayerController.h"
#include "Ability/Tasks/AbilityTask_CustomTick.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"

UAbility_PlayerPierce::UAbility_PlayerPierce()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	MaxActivePierceTime = 2;
	PierceDistance = 500;
	bBlockMoveInActing = true;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Player.Pierce"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Player.Pierce"));

}

void UAbility_PlayerPierce::EndPlayerPierce()
{
	RemainPierceCount = 0;
}

AActionPortfolioCharacter* UAbility_PlayerPierce::SearchTargetCharacter()
{
	AActionPortfolioCharacter* Avatar =  Cast<AActionPortfolioCharacter>(GetAvatarActorFromActorInfo());
	if (Avatar)
	{
		TargetCharacter = Avatar->GetLastAttackedTarget();
		if(!TargetCharacter.IsValid()) TargetCharacter.Reset();
		return TargetCharacter.Get();
	}

	return nullptr;
}

void UAbility_PlayerPierce::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (RemainPierceCount <= 0 && !CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());

	FRotator ControllerForwardRotator(0, Player->GetController()->GetControlRotation().Yaw, 0);
	Player->SetActorRotation(ControllerForwardRotator);

	if (RemainPierceCount <= 0)
	{
		FTimerHandle EndPierceTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(EndPierceTimerHandle, this, &UAbility_PlayerPierce::EndPlayerPierce, MaxActivePierceTime);
		SearchTargetCharacter();
		RemainPierceCount = MaxPierceCount;
	}

	//이동 위치 구하기
	if(!TargetCharacter.IsValid()) SearchTargetCharacter();
	if (TargetCharacter.IsValid()) {
		
		FVector PlayerLocation = Player->GetActorLocation();
		FVector TargetLocation = TargetCharacter->GetActorLocation();
		float TargetRadius = TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

		FVector TeleportLocation = PlayerLocation - TargetLocation;
		TeleportLocation.Z = 0;
		TeleportLocation.Normalize();

		TeleportLocation *= (TargetRadius + PierceDistance);
		TeleportLocation += TargetLocation;
		
		PrePiercePos = TeleportLocation;
	}
	else
	{
		PrePiercePos = Player->GetActorLocation();
	}

	UAbilityTask_PlayMontageAndWaitEvent* MeeleeAbilityTask = UAbilityTask_PlayMontageAndWaitEvent::PlayMontageAndWaitEvent(this, TaskInstanceName, MeeleeMontage, EventTagContainer, PlayLate, NAME_None, bStopWhenAbilityEnd);
	ensure(MeeleeAbilityTask);
	MeeleeAbilityTask->OnCompleted.AddDynamic(this, &UAbility_PlayerPierce::OnMontageEnded);
	MeeleeAbilityTask->OnBlendOut.AddDynamic(this, &UAbility_PlayerPierce::OnMontageEnded);
	MeeleeAbilityTask->OnInterrupted.AddDynamic(this, &UAbility_PlayerPierce::OnMontageCancelled);
	MeeleeAbilityTask->OnCancelled.AddDynamic(this, &UAbility_PlayerPierce::OnMontageCancelled);
	MeeleeAbilityTask->EventReceived.AddDynamic(this, &UAbility_PlayerPierce::OnEventReceived);
	MeeleeAbilityTask->ReadyForActivation();

	if (bBlockMoveInActing)
	{
		GetCurrentActorInfo()->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.BlockMove"));
		OnGameplayAbilityEnded.AddUObject(this, &UAbility_PlayerPierce::OffBlockMove);
	}

	//이동하기
	UAbilityTask_MoveToLocation* MoveToPrePiercePos = UAbilityTask_MoveToLocation::MoveToLocation(this, "PrePierceMove", PrePiercePos, 0.1, nullptr, nullptr);
	MoveToPrePiercePos->OnTargetLocationReached.AddDynamic(this, &UAbility_PlayerPierce::OnPrePiercePosLeached);
	MoveToPrePiercePos->ReadyForActivation();
}

void UAbility_PlayerPierce::OnPrePiercePosLeached()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());

	if (TargetCharacter.IsValid())
	{
		FVector PlayerLocation = Player->GetActorLocation();
		FVector TargetLocation = TargetCharacter->GetActorLocation();

		FVector PlayerToTarget = TargetLocation - PlayerLocation;
		PlayerToTarget.Z = 0;
		PlayerToTarget.Normalize();
		FRotator NewRotation = PlayerToTarget.Rotation();

		Player->SetActorRotation(NewRotation);
	}

	//꿰뚫기 끝지점 구하기
	FVector PierceStartLocation = Player->GetActorLocation() + 20 * Player->GetActorForwardVector() + 10 * FVector::UpVector;
	FVector PierceEndLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * 2 * (PierceDistance) + 10 * FVector::UpVector;
	if (TargetCharacter.IsValid())
	{
		PierceEndLocation += Player->GetActorForwardVector() * 2 * (TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius());
	}
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CQP("PierceCollision", false, Player);
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Player->GetCapsuleComponent()->GetScaledCapsuleRadius());
	GetWorld()->SweepMultiByProfile(HitResults, PierceStartLocation, PierceEndLocation, Player->GetCapsuleComponent()->GetComponentRotation().Quaternion(), "Pawn", CollisionShape, CQP);

	int ResultNum = HitResults.Num();

	for (int Idx = 0; Idx < ResultNum; Idx++)
	{
		if (AActionPortfolioCharacter* TargetChar = Cast<AActionPortfolioCharacter>(HitResults[Idx].GetActor()))
		{

		}
		else {
			PierceEndLocation = HitResults[Idx].Location;
			break;
		}
	}


	UAbilityTask_MoveToLocation* MoveToPiercePos = UAbilityTask_MoveToLocation::MoveToLocation(this, "PrePierceMove", PierceEndLocation, 0.1, nullptr, nullptr);
	MoveToPiercePos->OnTargetLocationReached.AddDynamic(this, &UAbility_PlayerPierce::OnPierceEnd);
	MoveToPiercePos->ReadyForActivation();

	GetAbilitySystemComponentFromActorInfo()->CurrentMontageJumpToSection("Looping");
}

void UAbility_PlayerPierce::OnPierceEnd()
{
	GetAbilitySystemComponentFromActorInfo()->CurrentMontageJumpToSection("End");

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(Player)) return;

	FRotator NewCameraRotator = Player->GetActorRotation();
	NewCameraRotator.Yaw = NewCameraRotator.Yaw + 180;
	NewCameraRotator.Pitch -= 20;

	Player->GetController()->SetControlRotation(NewCameraRotator);
}



