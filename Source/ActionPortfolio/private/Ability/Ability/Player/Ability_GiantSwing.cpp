// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Player/Ability_GiantSwing.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ActionPortfolio.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Character/ActionPortfolioCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Ability/Tasks/AbilityTask_GrantTags.h"
#include "Character/ActionPFAnimInstance.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Ability/Ability/Ability_Rigidity.h"

void UAbility_GiantSwing::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
#if WITH_EDITOR
	if (SwingAnim == nullptr)
	{
		PFLOG(Error, TEXT("Giant Swing Anim is \'nullptr\'"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (SwingAnim->GetSectionLength(0) <= GrabDelay)
	{
		PFLOG(Error, TEXT("Start Section Length <= GrabDelay"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
#endif

	UAbilityTask_PlayMontageAndWait* PlaySwingAnim = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "PlaySwingAnim", SwingAnim);
	PlaySwingAnim->OnCompleted.AddDynamic(this, &UAbility_GiantSwing::OnAnimComplete);
	PlaySwingAnim->OnInterrupted.AddDynamic(this, &UAbility_GiantSwing::OnAnimInterrupted);
	PlaySwingAnim->OnCancelled.AddDynamic(this, &UAbility_GiantSwing::OnAnimCancelled);
	PlaySwingAnim->ReadyForActivation();

	UAbilityTask_WaitDelay* WaitGrab = UAbilityTask_WaitDelay::WaitDelay(this, GrabDelay);
	WaitGrab->OnFinish.AddDynamic(this, &UAbility_GiantSwing::ScanGrabTarget);
	WaitGrab->ReadyForActivation();
}

void UAbility_GiantSwing::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (SwingTarget.IsValid())
	{
		SwingTarget->OnCancelled();

		SetSwingTargetAbility(nullptr);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_GiantSwing::SetSwingTargetAbility(UAbility_GiantSwingTarget* Target)
{
	if (Target == nullptr)
	{
		SwingTarget.Reset();
	}
	else
	{
		SwingTarget = Target;
	}
}

void UAbility_GiantSwing::OnAnimComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAbility_GiantSwing::OnAnimInterrupted()
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAbility_GiantSwing::OnAnimCancelled()
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UAbility_GiantSwing::ScanGrabTarget()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(Avatar);

	FCollisionObjectQueryParams GrabTrace(ECollisionChannel::ECC_GameTraceChannel2);
	
	FCollisionShape Shape = FCollisionShape::MakeSphere(ScanBoxHalfExtent.X);
	FCollisionQueryParams CollisionParam(NAME_None, false, Avatar);

	TArray<FOverlapResult> OverlapResults;

	FVector ScanCenter = Avatar->GetActorLocation()+ ScanCenterDelta * Avatar->GetActorForwardVector();

	UWorld* CurrentWorld = Avatar->GetWorld();


	//bool bResult = CurrentWorld->OverlapMultiByObjectType(OverlapResults, ScanCenter, Avatar->GetActorQuat(), GrabTrace, Shape, CollisionParam);
	bool bResult = CurrentWorld->OverlapMultiByProfile(OverlapResults, ScanCenter, Avatar->GetActorQuat(), FName("AbilityEffect"), Shape, CollisionParam);

	for (auto& OverlapResult : OverlapResults)
	{
		//잡기 시도
		AActor* TargetTemp = OverlapResult.GetActor();


		ETeamAttitude::Type TeamAttitude = TeamAgent->GetTeamAttitudeTowards(*TargetTemp);
		if(TeamAttitude != ETeamAttitude::Hostile) { continue; }

		if(IAbilitySystemInterface* TargetASI = Cast<IAbilitySystemInterface>(TargetTemp))
		{
			UAbilitySystemComponent* TargetASC = TargetASI->GetAbilitySystemComponent();

			FGameplayEventData SwingGrabEventData;
			SwingGrabEventData.OptionalObject = this;

			FGameplayAbilitySpec GrabTargetAbilitySpec(UAbility_GiantSwingTarget::StaticClass());
			FGameplayAbilitySpecHandle SpecHandle = TargetASC->GiveAbilityAndActivateOnce(GrabTargetAbilitySpec, &SwingGrabEventData);
				
			if (SpecHandle.IsValid())
			{
				OnGrabSucceed(SpecHandle, TargetASC);
				break;
			}
			
		}
	}
	

#if WITH_EDITOR
	DrawDebugBox(CurrentWorld, ScanCenter, ScanBoxHalfExtent, Avatar->GetActorQuat(), bResult ? FColor::Green : FColor::Red, false, 5);

#endif

}


void UAbility_GiantSwing::OnGrabSucceed(const FGameplayAbilitySpecHandle TargetHandle, UAbilitySystemComponent* TargetASC)
{
	UAbilityTask_WaitDelay* WaitThrow = UAbilityTask_WaitDelay::WaitDelay(this, SwingTime);
	WaitThrow->OnFinish.AddDynamic(this, &UAbility_GiantSwing::ThrowTarget);
	WaitThrow->ReadyForActivation();

	MontageJumpToSection("Swing");
}

void UAbility_GiantSwing::ThrowTarget()
{
	if (SwingTarget.IsValid())
	{
		MontageJumpToSection("Throw");

		AActor* AvatarActor = GetAvatarActorFromActorInfo();
		FVector Direction = AvatarActor->GetActorForwardVector();
		SwingTarget->OnThrowed(Direction, ThrowPower);

		SetSwingTargetAbility(nullptr);
	}
}

FGameplayEffectSpecHandle UAbility_GiantSwing::CreateDamageEffect() const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.SetAbility(this);

	FGameplayEffectSpecHandle DamageHandle = UDamageEffect::CreateDamageEffectSpec(SwingDamage, Context);

	return DamageHandle;
}

UAbility_GiantSwingTarget::UAbility_GiantSwingTarget()
{
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Rigidity"));
}

void UAbility_GiantSwingTarget::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
#if WITH_EDITOR
	if (TriggerEventData == nullptr)
	{
		PFLOG(Error, TEXT("Giant Swing Target Acitivate By No EventData."));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
#endif

	Super::ActivateAbility_CPP(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Set Source Ability
	UAbility_GiantSwing* SwingAbility = Cast<UAbility_GiantSwing>(GetGrabAbility());

	SwingAbility->SetSwingTargetAbility(this);
	GrabSocket = SwingAbility->GetGrabSocketName();
	GrabTargetSocket = SwingAbility->GetGrabTargetSocketName();

	//Create Damage Effect Handle
	UAbilitySystemComponent* SourceASC = SwingAbility->GetAbilitySystemComponentFromActorInfo();
	UAbilitySystemComponent* TargetASC = GetAbilitySystemComponentFromActorInfo();

	SourceASC->MakeEffectContext();

	DamageHandle = SwingAbility->CreateDamageEffect();

	//Play Rigidity Animation
	AActionPortfolioCharacter* PFChar = Cast<AActionPortfolioCharacter>(GetAvatarActorFromActorInfo());
	RigidityData = SwingAbility->GetRigidityData();

	if (PFChar)
	{
		if (UAnimMontage* RigidityAnim = PFChar->GetAnimInstance()->GetAnimMontageByTag(RigidityData.RigidityAnimTag))
		{
			UAbilityTask_PlayMontageAndWait* PlaySwingAnim = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "PlaySwingAnim", RigidityAnim);

			PlaySwingAnim->ReadyForActivation();
		}
	}

	UAbilityTask_GrantTags* GrantGrapTag = UAbilityTask_GrantTags::GrantTags(this, "GrantGrapTag", FGameplayTagContainer(FGameplayTag::RequestGameplayTag("State.Etc.Grab")));
	GrantGrapTag->ReadyForActivation();

	ACharacter* AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor);

	//잡기
	AvatarCharacter->AttachToActor(GetGrappler(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), GrabSocket);

	FRotator NewRotation(270, 0, 0);
	AvatarCharacter->SetActorRelativeRotation(NewRotation);
	FVector GrabTargetLocation = AvatarCharacter->GetMesh()->GetSocketLocation(GrabTargetSocket);
	FVector NewLocation = AvatarCharacter->GetActorLocation() - GrabTargetLocation;
	
	AvatarCharacter->SetActorRelativeLocation(NewLocation);
}

void UAbility_GiantSwingTarget::OnThrowed(FVector Direction, FVector2D Power)
{
	EndTaskByInstanceName("GrantGrapTag");

	DetachFromGrappler();

	//던져지기
	FVector Impulse = Direction * Power.X + FVector::UnitZ() * Power.Y;

	ACharacter* AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	UCharacterMovementComponent* Movement = AvatarCharacter->GetCharacterMovement();

	AvatarCharacter->SetActorRotation(Direction.Rotation() + FRotator(0, 180, 0));
	Movement->AddImpulse(Impulse, true);


	//던져진 후에 땅에 닿는 이벤트 기다리기
	UAbilityTask_WaitGameplayEvent* WaitOnLandedEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("CommonEvent.OnLanded"), nullptr, true);
	WaitOnLandedEvent->EventReceived.AddDynamic(this, &UAbility_GiantSwingTarget::OnLanded);
	WaitOnLandedEvent->ReadyForActivation();

	//Rigidity 적용
	UAbility_Rigidity::RigidityToTarget(GetGrabAbility()->GetAbilitySystemComponentFromActorInfo(), GetAbilitySystemComponentFromActorInfo(), RigidityData);
}

void UAbility_GiantSwingTarget::OnCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void UAbility_GiantSwingTarget::OnLanded(FGameplayEventData Payload)
{
	//데미지 추가
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	DamageHandle.Data->GetContext().AddHitResult(*Payload.ContextHandle.GetHitResult(), true);

	ASC->ApplyGameplayEffectSpecToSelf(*DamageHandle.Data.Get());

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAbility_GiantSwingTarget::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (bWasCancelled)
	{
		DetachFromGrappler();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}



