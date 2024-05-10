// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Ability_Rigidity.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/Tasks/AbilityTask_CustomTick.h"
#include "Ability/CustomAbilityHelperInterface.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ActionPortfolio.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Ability/Tasks/AbilityTask_GrantTags.h"

URigidityEventData::URigidityEventData()
{
	RigidityTime = 0;
	HitResult = nullptr;
	RigidityAnim = nullptr;
}

UAbility_Rigidity::UAbility_Rigidity()
{
	FGameplayTag RigidityTag = FGameplayTag::RequestGameplayTag("State.Etc.Rigidity");

	AbilityTags.AddTag(RigidityTag);

	CancelAbilitiesWithTag.AddTag(RigidityTag);

	ActivationOwnedTags.AddTag(RigidityTag);

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Rigidity"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Down"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
}

void UAbility_Rigidity::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	FGameplayTag IsInAirTag = FGameplayTag::RequestGameplayTag("State.Etc.IsInAir");

	ActorInfo->AbilitySystemComponent->UnregisterGameplayTagEvent(DelegateHandle, IsInAirTag);

	MontageStop();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_Rigidity::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(TriggerEventData);
	URigidityEventData* RigidityData = Cast<URigidityEventData>(TriggerEventData->OptionalObject);
	check(RigidityData);

	RemainTime = RigidityData->RigidityTime;

	if (RemainTime <= 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	//Rigidity Tick
	UAbilityTask_CustomTick* CustomTick = UAbilityTask_CustomTick::CustomTick(this, "RigidityTick");
	CustomTick->OnCustomTickEvent.AddDynamic(this, &UAbility_Rigidity::RigidityTick);
	CustomTick->ReadyForActivation();

	//Play Animation
	UAnimMontage* RigidityAnim = RigidityData->RigidityAnim;
	if (RigidityAnim == nullptr)
	{
		ICustomAbilityHelperInterface* Helper = Cast<ICustomAbilityHelperInterface>(ActorInfo->AvatarActor);
		RigidityAnim = Helper->GetRigidityAnim(RemainTime, RigidityData->HitResult);
	}

	check(RigidityAnim);

	UAbilityTask_PlayMontageAndWait* PlayAnim = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "RigidityAnim", RigidityAnim);
	PlayAnim->OnCompleted.AddDynamic(this, &UAbility_Rigidity::OnRigidityMontageEnd);
	PlayAnim->ReadyForActivation();

	//Check In Air
	FGameplayTag IsInAirTag = FGameplayTag::RequestGameplayTag("State.Etc.IsInAir");
	
	if (bool bIsInAir = ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(IsInAirTag))
	{
		IsInAirWithRigidity();
	}
	else
	{
		ActorInfo->AbilitySystemComponent->RegisterGameplayTagEvent(IsInAirTag).AddUObject(this, &UAbility_Rigidity::WaitIsInAirInRigidity);
	}
}

void UAbility_Rigidity::WaitIsInAirInRigidity(const FGameplayTag IsInAirTag, int TagCount)
{
	if(TagCount <= 0) return;

	UAbilitySystemComponent* ASC = GetCurrentActorInfo()->AbilitySystemComponent.Get();

	ASC->UnregisterGameplayTagEvent(DelegateHandle, IsInAirTag);

	IsInAirWithRigidity();
}




void UAbility_Rigidity::OnRigidityEnd()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if(ASC == nullptr) return;

	EndTaskByInstanceName("RigidityTick");

	if(ASC->IsAnimatingAbility(this) && ASC->GetCurrentMontageSectionName() == "Down")
	{
		MontageJumpToSection("DownRecovery");
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UAbility_Rigidity::RigidityTick(float DeltaTime)
{
	RemainTime -= DeltaTime;

	FGameplayTag IsInAirTag = FGameplayTag::RequestGameplayTag("State.Etc.IsInAir");
	bool bIsInAir = GetCurrentActorInfo()->AbilitySystemComponent->HasMatchingGameplayTag(IsInAirTag);
	FString TestString = bIsInAir ? "In Air" : "On Ground";

	if (RemainTime <= 1.5 && bIsInAir)
	{ 
		RemainTime = 1.5;
	}


	if (RemainTime <= 0) {
		OnRigidityEnd();
	}
}

void UAbility_Rigidity::IsInAirWithRigidity()
{
	UAbilitySystemComponent* ASC = GetCurrentActorInfo()->AbilitySystemComponent.Get();
	FGameplayTag IsInAirTag = FGameplayTag::RequestGameplayTag("State.Etc.IsInAir");

	MontageJumpToSection("Air");

	UAbilityTask_WaitGameplayEvent* WaitOnLandedEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("CommonEvent.OnLanded"), nullptr, true);
	WaitOnLandedEvent->EventReceived.AddDynamic(this, &UAbility_Rigidity::OnLandedInRigidity);
	WaitOnLandedEvent->ReadyForActivation();
}



void UAbility_Rigidity::OnLandedInRigidity(FGameplayEventData Payload)
{
	CharacterDown();
}

void UAbility_Rigidity::CharacterDown()
{
	UAbilitySystemComponent* ASC = GetCurrentActorInfo()->AbilitySystemComponent.Get();
	FGameplayTag DownTag = FGameplayTag::RequestGameplayTag("State.Etc.Down");

	UAbilityTask_GrantTags* GrantTags = UAbilityTask_GrantTags::GrantTags(this, "GrantDownTag", FGameplayTagContainer(DownTag));
	GrantTags->ReadyForActivation();

	MontageJumpToSection("Down");
}

void UAbility_Rigidity::OnRigidityMontageEnd()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

FGameplayAbilitySpecHandle UAbility_Rigidity::RigidityToTarget(TSubclassOf<UAbility_Rigidity> InClass, UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, float InTime, const FHitResult* HitResult, UAnimMontage* InAnim)
{
	URigidityEventData* RigidityData = NewObject<URigidityEventData>();
	RigidityData->RigidityTime = InTime;
	RigidityData->HitResult = HitResult;
	FGameplayEventData RigidityEventData;
	RigidityEventData.OptionalObject = RigidityData;
	RigidityEventData.Instigator = SourceASC->GetAvatarActor();
	RigidityEventData.Target = TargetASC->GetAvatarActor();

	FGameplayAbilitySpec AbilitySpec(InClass, 1, -1, SourceASC);

	return TargetASC->GiveAbilityAndActivateOnce(AbilitySpec, &RigidityEventData);
}
