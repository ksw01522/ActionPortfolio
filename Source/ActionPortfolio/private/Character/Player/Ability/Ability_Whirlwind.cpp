// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Ability/Ability_Whirlwind.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "ActionPortfolio.h"
#include "GameplayEffect.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Ability/Tasks/AbilityTask_WaitEnterMontageSection.h"
#include "Ability/Tasks/AbilityTask_EffectUseCollision.h"
#include "Ability/Tasks/AbilityTask_WaitNewCollision.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Ability/Tasks/AbilityTask_GrantTags.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Ability/Ability/Ability_Knockback.h"
#include "Ability/Ability/Ability_Rigidity.h"
#include "Abilities/Tasks/AbilityTask_Repeat.h"
#include "Ability/ActionPFAbilityBFL.h"

UAbility_Whirlwind::UAbility_Whirlwind()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	CollisionProfileName = "AbilityEffect";

	bStopWhenAbilityEnd = true;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Down"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Rigidity"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Attacking"));

	GrantTagsDuringAttack.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Attacking"));

	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Block.Move"));

	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Ability.Meelee"));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Ability.Meelee"));

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Meelee"));
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Player.Whirlwind"));

	DamageCount = 1;
}

void UAbility_Whirlwind::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UAbilityTask_PlayMontageAndWait* MeeleeAbilityTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "MeeleeAnim", WhirlwindMontage, 1, NAME_None, bStopWhenAbilityEnd);
	ensure(MeeleeAbilityTask);
	MeeleeAbilityTask->OnCompleted.AddDynamic(this, &UAbility_Whirlwind::OnMontageEnded);
	MeeleeAbilityTask->OnBlendOut.AddDynamic(this, &UAbility_Whirlwind::OnMontageBlendOut);
	MeeleeAbilityTask->OnInterrupted.AddDynamic(this, &UAbility_Whirlwind::OnMontageInterrupted);
	MeeleeAbilityTask->OnCancelled.AddDynamic(this, &UAbility_Whirlwind::OnMontageCancelled);
	MeeleeAbilityTask->ReadyForActivation();

	UAbilityTask_WaitEnterMontageSection* WaitAttackSection = UAbilityTask_WaitEnterMontageSection::WaitEnterMontageSection(this, "WaitAttackSectionEnter", WhirlwindMontage, "Attack");
	WaitAttackSection->OnEnterSection.AddDynamic(this, &UAbility_Whirlwind::OnEnterAttackSection);
	WaitAttackSection->ReadyForActivation();

	if (!GrantTagsDuringAttack.IsEmpty())
	{
		UAbilityTask_GrantTags* GrantTagsTask = UAbilityTask_GrantTags::GrantTags(this, "GrantTagsDuringAttack", GrantTagsDuringAttack);
		GrantTagsTask->ReadyForActivation();
	}

	WhirlwindMaxTime = (DamageCount - 1) * DamageIntervalTime;
	
}

bool UAbility_Whirlwind::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCan = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	return bCan && WhirlwindMontage;
}

void UAbility_Whirlwind::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	WaitInputRelease.Reset();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_Whirlwind::OnEnterAttackSection(UAnimMontage* InMeeleeMontage, const FName& InSectionName)
{
	FGameplayEffectContextHandle ContextHandle = GetCurrentActorInfo()->AbilitySystemComponent->MakeEffectContext();
	int32 TempAbilityLevel = GetAbilityLevel();

	AttackRepeat = UAbilityTask_Repeat::RepeatAction(this, DamageIntervalTime, DamageCount);
	AttackRepeat->OnPerformAction.AddDynamic(this, &UAbility_Whirlwind::WhirlwindAttackCheck);
	AttackRepeat->OnFinished.AddDynamic(this, &UAbility_Whirlwind::OnWhirwindAttackFinished);
	AttackRepeat->ReadyForActivation();

	WaitInputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	WaitInputRelease->OnRelease.AddDynamic(this, &UAbility_Whirlwind::OnWhilrwindRelease);
	WaitInputRelease->ReadyForActivation();
}

void UAbility_Whirlwind::WhirlwindAttackCheck(int32 ActionNumber)
{
	//EffectContext
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();

	AActor* Avatar = GetAvatarActorFromActorInfo();

	FGameplayEffectSpecHandle SpecHandle = UDamageEffect::CreateDamageEffectSpec(DamageData, ContextHandle);
	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

	TArray<FOverlapResult> OverlapResults;

	FVector CenterPos = Avatar->GetActorLocation();

	FComponentQueryParams Params(NAME_None, Avatar);

	FCollisionShape WhirlWindShape = FCollisionShape::MakeSphere(WhirlwindRadius);
	FCollisionQueryParams CollisionParams(NAME_None, false, Avatar);

	GetWorld()->OverlapMultiByProfile(OverlapResults, CenterPos, FQuat::Identity, CollisionProfileName, WhirlWindShape, CollisionParams);

	IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo());

	for (auto& OverlapResult : OverlapResults)
	{
		AActor* TargetActor = OverlapResult.GetActor();
		if (TeamAgent->GetTeamAttitudeTowards(*TargetActor) == ETeamAttitude::Friendly) continue;
		UAbilitySystemComponent* TargetASC =  UActionPFAbilityBFL::GetAbilitySystemComponent(TargetActor);
		if (TargetASC == nullptr) continue;
		
		ASC->ApplyGameplayEffectSpecToTarget(*Spec, TargetASC);

		FGameplayEventData EventData;
		EventData.Instigator = GetAvatarActorFromActorInfo();
		EventData.Target = TargetActor;
		CurrentActorInfo->AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("CommonEvent.OnAttackTarget")), &EventData);

		UAbility_Rigidity::RigidityToTarget(GetCurrentActorInfo()->AbilitySystemComponent.Get(), TargetASC, RigidityData);

		FGameplayAbilitySpec KnockbackSpec(KnockbackClass, 1, -1, GetAvatarActorFromActorInfo());
		TargetASC->GiveAbilityAndActivateOnce(KnockbackSpec);
		
	}

#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), CenterPos, WhirlwindRadius, 64, OverlapResults.IsEmpty() ? FColor::Green : FColor::Red);
#endif
}

void UAbility_Whirlwind::OnWhilrwindRelease(float TimeHeld)
{
	
	OnWhirlwindEnd();
}

void UAbility_Whirlwind::OnWhirwindAttackFinished(int32 ActionNumber)
{	
	OnWhirlwindEnd();
}

void UAbility_Whirlwind::OnWhirlwindEnd()
{
	if (WaitInputRelease.IsValid()) { WaitInputRelease->EndTask(); }
	if (AttackRepeat.IsValid()) { AttackRepeat->EndTask(); }

	UAbilityTask_GrantTags* GrantsBlockMove = UAbilityTask_GrantTags::GrantTags(this, "GrantTagsDuringPostDelay", FGameplayTagContainer(FGameplayTag::RequestGameplayTag("State.Block.Move")));
	GrantsBlockMove->ReadyForActivation();

	MontageJumpToSection("PostDelay");

	if (!GrantTagsDuringAttack.IsEmpty())
	{
		EndTaskByInstanceName("GrantTagsDuringAttack");
	}
}

void UAbility_Whirlwind::OnMontageEnded()
{
	check(CurrentActorInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_Whirlwind::OnMontageCancelled()
{
	check(CurrentActorInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_Whirlwind::OnMontageBlendOut()
{
	check(CurrentActorInfo);

}

void UAbility_Whirlwind::OnMontageInterrupted()
{
	check(CurrentActorInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_Whirlwind::GetKnockbackVector(AActor* Target, FVector& OnGround, FVector& InAir)
{
	OnGround = FVector::ZeroVector;
	InAir = FVector(0,0, 100);
}
