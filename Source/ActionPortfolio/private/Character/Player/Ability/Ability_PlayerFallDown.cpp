// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Ability/Ability_PlayerFallDown.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Ability/Tasks/AbilityTask_PlayMontageAndWaitEvent.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ActionPortfolio.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/Tasks/AbilityTask_CustomTick.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "GameFramework/CharacterMovementComponent.h"

UAbility_PlayerFallDown::UAbility_PlayerFallDown()
{
	FallDownSpeed = 600;
	FallDownImpactRadius = 200;
	FallDownHeight = 200;
}

void UAbility_PlayerFallDown::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActionPortfolioCharacter* AvatarChar = Cast<AActionPortfolioCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(AvatarChar)) {
		PFLOG(Error, TEXT("Ability Call From Not Valid Actor."));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWaitEvent* MeeleeAbilityTask = UAbilityTask_PlayMontageAndWaitEvent::PlayMontageAndWaitEvent(this, TaskInstanceName, MeeleeMontage, EventTagContainer, PlayLate, NAME_None, bStopWhenAbilityEnd);
	ensure(MeeleeAbilityTask);
	MeeleeAbilityTask->OnCompleted.AddDynamic(this, &UAbility_PlayerFallDown::OnMontageEnded);
	MeeleeAbilityTask->OnBlendOut.AddDynamic(this, &UAbility_PlayerFallDown::OnMontageEnded);
	MeeleeAbilityTask->OnInterrupted.AddDynamic(this, &UAbility_PlayerFallDown::OnMontageCancelled);
	MeeleeAbilityTask->OnCancelled.AddDynamic(this, &UAbility_PlayerFallDown::OnMontageCancelled);
	MeeleeAbilityTask->EventReceived.AddDynamic(this, &UAbility_PlayerFallDown::OnEventReceived);

	MeeleeAbilityTask->ReadyForActivation();

	if (bBlockMoveInActing)
	{
		GetCurrentActorInfo()->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.BlockMove"));
		OnGameplayAbilityEnded.AddUObject(this, &UAbility_PlayerFallDown::OffBlockMove);
	}


	AActionPortfolioCharacter* Target = AvatarChar->GetLastAttackedTarget();

	PreFallDownLocation = IsValid(Target) ? Target->GetActorLocation() + FVector(0, 0, FallDownHeight + Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) : AvatarChar->GetActorLocation() + FVector(0, 0, FallDownHeight);

	UAbilityTask_MoveToLocation* MoveToPrePiercePos = UAbilityTask_MoveToLocation::MoveToLocation(this, "PreFallDownMove", PreFallDownLocation, 0.1, nullptr, nullptr);
	MoveToPrePiercePos->OnTargetLocationReached.AddDynamic(this, &UAbility_PlayerFallDown::OnPreFallDownPosReached);
	MoveToPrePiercePos->ReadyForActivation();


}

void UAbility_PlayerFallDown::OnLandEventReceived(FGameplayEventData EventData)
{
	AActionPortfolioCharacter* AvatarChar = Cast<AActionPortfolioCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(AvatarChar)) {
		PFLOG(Error, TEXT("Ability Call From Not Valid Actor."));
		return;
	}

	if (FallDownImpactParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FallDownImpactParticle, AvatarChar->GetActorTransform());
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParams("FallDownImpact", false, AvatarChar);

	bool bOverlapped = GetWorld()->OverlapMultiByProfile(OverlapResults, AvatarChar->GetActorLocation(), FQuat::Identity, "OverlapAll", FCollisionShape::MakeSphere(FallDownImpactRadius), CollisionParams);

	#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), AvatarChar->GetActorLocation(), FallDownImpactRadius, 32, bOverlapped ? FColor::Red : FColor::Blue, false, 1);
	#endif

	FActionPFEffectContainer* EffectsToApply = DamageMap.Find(FGameplayTag::RequestGameplayTag("Ability.Player.FallDown.Impact"));
	TArray<AActor*> AlreadyEffectedTargets;

	if (EffectsToApply != nullptr && !EffectsToApply->EffectClasses.IsEmpty())
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			if (AlreadyEffectedTargets.Contains(OverlapResult.GetActor())) continue;
			AlreadyEffectedTargets.Add(OverlapResult.GetActor());

			AActionPortfolioCharacter* TargetChar = Cast<AActionPortfolioCharacter>(OverlapResult.GetActor());
			if (!IsValid(TargetChar) || AvatarChar->GetTeamAttitudeTowards(*TargetChar) == ETeamAttitude::Friendly) continue;


			UActionPFAbilitySystemComponent* SourceAbilitySystem = Cast<UActionPFAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
			UActionPFAbilitySystemComponent* TargetAbilitySystem = Cast<UActionPFAbilitySystemComponent>(TargetChar->GetAbilitySystemComponent());

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
	}

	AvatarChar->GetMesh()->GetAnimInstance()->Montage_JumpToSection("End");
}

void UAbility_PlayerFallDown::OnPreFallDownPosReached()
{
	AActionPortfolioCharacter* AvatarChar = Cast<AActionPortfolioCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(AvatarChar)) {
		PFLOG(Error, TEXT("Ability Call From Not Valid Actor."));
		return;
	}

	AvatarChar->GetMesh()->GetAnimInstance()->Montage_JumpToSection("Loop");
	AvatarChar->GetCharacterMovement()->GravityScale = 0;
	AvatarChar->GetCharacterMovement()->AddImpulse(FVector(0,0, -FallDownSpeed) - AvatarChar->GetCharacterMovement()->Velocity, true);

	UAbilityTask_WaitGameplayEvent* WaitLandEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Ability.PublicEvent.OnLand"), nullptr, true);
	WaitLandEvent->EventReceived.AddDynamic(this, &UAbility_PlayerFallDown::OnLandEventReceived);
	WaitLandEvent->ReadyForActivation();

	OnGameplayAbilityEnded.AddUObject(this, &UAbility_PlayerFallDown::OnFallDownEnded);
}

void UAbility_PlayerFallDown::OnFallDownEnded(UGameplayAbility* Ability)
{
	AActionPortfolioCharacter* AvatarChar = Cast<AActionPortfolioCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(AvatarChar)) {
		PFLOG(Error, TEXT("Ability Call From Not Valid Actor."));
		return;
	}

	AvatarChar->ResetMovement();
}


