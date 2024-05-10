// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Boss/Ability/Ability_BossJumpFall.h"
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
#include "Character/Enemy/DangerZone.h"
#include "Character/EnemyAIController.h"

UAbility_BossJumpFall::UAbility_BossJumpFall()
{
	FlightTime = 5;
	CustomGravityScale = 10;
	JumpFallImpactRadius = 100;
	MaxJumpFallCount = 3;

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAbility_BossJumpFall::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	TargetCharacter.Reset();

	AActionPortfolioCharacter* AvatarChar = Cast<AActionPortfolioCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(AvatarChar)) {
		PFLOG(Error, TEXT("Ability Call From Not Valid Actor."));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}


	if (AEnemyAIController* EnemyAIController = AvatarChar->GetController<AEnemyAIController>())
	{
		TargetCharacter = EnemyAIController->GetCurrentFocusedTarget();
	}
	else
	{
		
	}

	MakeFallDownLocation();

	/*
	UAbilityTask_PlayMontageAndWaitEvent* MeeleeAbilityTask = UAbilityTask_PlayMontageAndWaitEvent::PlayMontageAndWaitEvent(this, TaskInstanceName, MeeleeMontage, EventTagContainer, PlayLate, NAME_None, bStopWhenAbilityEnd);
	ensure(MeeleeAbilityTask);
	MeeleeAbilityTask->OnCompleted.AddDynamic(this, &UAbility_BossJumpFall::OnMontageEnded);
	MeeleeAbilityTask->OnBlendOut.AddDynamic(this, &UAbility_BossJumpFall::OnMontageEnded);
	MeeleeAbilityTask->OnInterrupted.AddDynamic(this, &UAbility_BossJumpFall::OnMontageCancelled);
	MeeleeAbilityTask->OnCancelled.AddDynamic(this, &UAbility_BossJumpFall::OnMontageCancelled);
	MeeleeAbilityTask->EventReceived.AddDynamic(this, &UAbility_BossJumpFall::OnEventReceived);

	MeeleeAbilityTask->ReadyForActivation();

	if (bBlockMoveInActing)
	{
		GetCurrentActorInfo()->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.BlockMove"));
		OnGameplayAbilityEnded.AddUObject(this, &UAbility_BossJumpFall::OffBlockMove);
	}

	UAbilityTask_WaitGameplayEvent* WaitJumpEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Ability.Boss.JumpFall.Jump"), nullptr, false);
	WaitJumpEvent->EventReceived.AddDynamic(this, &UAbility_BossJumpFall::OnJumpEventReceived);
	RemainJumpFallCount = MaxJumpFallCount;
	WaitJumpEvent->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitLandEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Ability.PublicEvent.OnLand"), nullptr, false);
	WaitLandEvent->EventReceived.AddDynamic(this, &UAbility_BossJumpFall::OnLandEventReceived);
	WaitLandEvent->ReadyForActivation();

	OnGameplayAbilityEnded.AddUObject(this, &UAbility_BossJumpFall::OnJumpFallEnd);

	FTransform DangerZoneSpawnTransform;
	DangerZoneSpawnTransform.SetLocation(FallDownLocation);
	DangerZonePtr = GetWorld()->SpawnActorDeferred<ADangerZone>(DangerZoneClass, DangerZoneSpawnTransform, AvatarChar, AvatarChar);
	DangerZonePtr->SetDangerZoneRadius(JumpFallImpactRadius);
	DangerZonePtr->FinishSpawning(DangerZoneSpawnTransform);
	*/
}

void UAbility_BossJumpFall::OnLandEventReceived(FGameplayEventData EventData)
{
/*
	if(DangerZonePtr.IsValid()) DangerZonePtr->Destroy();
	
	AActionPortfolioCharacter* AvatarChar = Cast<AActionPortfolioCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(AvatarChar)) {
		PFLOG(Error, TEXT("Ability Call From Not Valid Actor."));
		return;
	}

	//Particle
	if (OnLandParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnLandParticle, AvatarChar->GetActorLocation() + FVector(0,0, -50), AvatarChar->GetActorRotation());
	}

	// Check Character In Impact;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParams("FallDownImpact", false, AvatarChar);

	GetWorld()->OverlapMultiByProfile(OverlapResults, AvatarChar->GetActorLocation(), FQuat::Identity, "OverlapAll", FCollisionShape::MakeSphere(JumpFallImpactRadius), CollisionParams);

	#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), AvatarChar->GetActorLocation(), JumpFallImpactRadius, 32, !OverlapResults.IsEmpty() ? FColor::Red : FColor::Blue, false, 1);
	#endif

	// Apply Damage Effect To Checked Character
	FActionPFEffectContainer* EffectsToApply = DamageMap.Find(FGameplayTag::RequestGameplayTag("Ability.Boss.JumpFall.Impact"));
	TArray<AActor*> AlreadyEffectedTargets;
	bool bAttackedTarget = false;

	if (EffectsToApply != nullptr && !EffectsToApply->EffectClasses.IsEmpty())
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			if(AlreadyEffectedTargets.Contains(OverlapResult.GetActor())) continue;
			AlreadyEffectedTargets.Add(OverlapResult.GetActor());

			AActionPortfolioCharacter* TargetChar = Cast<AActionPortfolioCharacter>(OverlapResult.GetActor());
			if (!IsValid(TargetChar) || AvatarChar->GetTeamAttitudeTowards(*TargetChar) == ETeamAttitude::Friendly) continue;

			bAttackedTarget = true;

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

	if (bAttackedTarget)
	{
		UAbilityTask_WaitGameplayEvent* WaitExplosionEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Ability.Boss.JumpFall.Explosion"), nullptr, true);
		WaitExplosionEvent->EventReceived.AddDynamic(this, &UAbility_BossJumpFall::OnExplosionEventReceived);
		WaitExplosionEvent->ReadyForActivation();

		MontageJumpToSection("Explosion");
	}
	else if (0 < RemainJumpFallCount && TargetCharacter.IsValid())
	{
		MakeFallDownLocation();

		MontageJumpToSection("Start");

		FTransform DangerZoneSpawnTransform;
		DangerZoneSpawnTransform.SetLocation(FallDownLocation);
		DangerZonePtr = GetWorld()->SpawnActorDeferred<ADangerZone>(DangerZoneClass, DangerZoneSpawnTransform, AvatarChar, AvatarChar);
		DangerZonePtr->SetDangerZoneRadius(JumpFallImpactRadius);
		DangerZonePtr->FinishSpawning(DangerZoneSpawnTransform);
	}
	else
	{
		MontageJumpToSection("Land");
	}
	*/
}

void UAbility_BossJumpFall::OnJumpEventReceived(FGameplayEventData EventData)
{
	AActionPortfolioCharacter* AvatarChar = Cast<AActionPortfolioCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(AvatarChar)) {
		return;
	}

	RemainJumpFallCount--;

	if (OnJumpParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnJumpParticle, AvatarChar->GetActorLocation() + FVector(0, 0, -50), AvatarChar->GetActorRotation());
	}

	AvatarChar->GetCharacterMovement()->GravityScale = CustomGravityScale;

	FVector FinalSpeed;
	FVector JumpStartLocation = AvatarChar->GetActorLocation();
	FinalSpeed = (FallDownLocation - JumpStartLocation) / FlightTime;
	FinalSpeed.Z = AvatarChar->GetCharacterMovement()->GetGravityZ() * FlightTime * -0.5;

	AvatarChar->GetCharacterMovement()->AddImpulse(FinalSpeed - AvatarChar->GetVelocity(), true);
}

void UAbility_BossJumpFall::OnJumpFallEnd(UGameplayAbility* Ability)
{
	AActionPortfolioCharacter* AvatarChar = Cast<AActionPortfolioCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(AvatarChar)) {
		PFLOG(Error, TEXT("Ability Call From Not Valid Actor."));
		return;
	}

	AvatarChar->ResetMovement();
}

void UAbility_BossJumpFall::MakeFallDownLocation()
{
	AActionPortfolioCharacter* AvatarChar = Cast<AActionPortfolioCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(AvatarChar)) {
		PFLOG(Error, TEXT("Can't Find Valid Avatar."));
		return;
	}

	bool bHitSucceeded;
	FHitResult HitResult;
	float TraceLength = 3000;
	FCollisionQueryParams CollisionQueryParam("BossJumpFallTrace", false, AvatarChar);

	if (TargetCharacter.IsValid())
	{
		CollisionQueryParam.AddIgnoredActor(TargetCharacter.Get());
		bHitSucceeded = AvatarChar->GetWorld()->LineTraceSingleByProfile(HitResult, TargetCharacter->GetActorLocation(), TargetCharacter->GetActorLocation() - FVector(0, 0, TraceLength), "Pawn", CollisionQueryParam);
	}
	else
	{
		bHitSucceeded = AvatarChar->GetWorld()->LineTraceSingleByProfile(HitResult, AvatarChar->GetActorLocation(), AvatarChar->GetActorLocation() - FVector(0, 0, TraceLength), "Pawn", CollisionQueryParam);
	}

	if (bHitSucceeded) {
		FallDownLocation = HitResult.Location;
	}
	else {
		FallDownLocation = TargetCharacter.IsValid() ? TargetCharacter->GetActorLocation() - FVector(0, 0, TraceLength) : AvatarChar->GetActorLocation() - FVector(0, 0, TraceLength);
	}
}

void UAbility_BossJumpFall::OnExplosionEventReceived(FGameplayEventData EventData)
{
	AActionPortfolioCharacter* AvatarChar = Cast<AActionPortfolioCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(AvatarChar)) {
		PFLOG(Error, TEXT("Ability Call From Not Valid Actor."));
		return;
	}

	if (OnExplosionParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnLandParticle, AvatarChar->GetActorLocation() + FVector(0, 0, -50), AvatarChar->GetActorRotation());
	}

	// Check Character In Impact;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParams("FallDownImpact", false, AvatarChar);

	GetWorld()->OverlapMultiByProfile(OverlapResults, AvatarChar->GetActorLocation(), FQuat::Identity, "OverlapAll", FCollisionShape::MakeSphere(JumpFallImpactRadius), CollisionParams);

#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), AvatarChar->GetActorLocation(), JumpFallImpactRadius, 32, !OverlapResults.IsEmpty() ? FColor::Red : FColor::Blue, false, 1);
#endif

	/*
	FActionPFEffectContainer* EffectsToApply = DamageMap.Find(FGameplayTag::RequestGameplayTag("Ability.Boss.JumpFall.Explosion"));
	TArray<AActor*> AlreadyEffectedTargets;
	bool bAttackedTarget = false;

	if (EffectsToApply != nullptr && !EffectsToApply->EffectClasses.IsEmpty())
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			if (AlreadyEffectedTargets.Contains(OverlapResult.GetActor())) continue;
			AlreadyEffectedTargets.Add(OverlapResult.GetActor());

			AActionPortfolioCharacter* TargetChar = Cast<AActionPortfolioCharacter>(OverlapResult.GetActor());
			if (!IsValid(TargetChar) || AvatarChar->GetTeamAttitudeTowards(*TargetChar) == ETeamAttitude::Friendly) continue;

			bAttackedTarget = true;

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
	*/
}
