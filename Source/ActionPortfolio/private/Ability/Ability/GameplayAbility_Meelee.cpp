// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/GameplayAbility_Meelee.h"
#include "Ability/Tasks/AbilityTask_PlayMontageAndWaitEvent.h"
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
#include "Ability/AbilityDamageCreator.h"
#include "Ability/Tasks/AbilityTask_WaitNewCollision.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Ability/Tasks/AbilityTask_GrantTags.h"
#include "Ability/Ability/Ability_Knockback.h"
#include "Ability/Ability/Ability_Rigidity.h"

#include "Character/Player/ActionPFPlayerController.h"
#include "LockOn/LockOnTargetComponent.h"


UGameplayAbility_Meelee::UGameplayAbility_Meelee() : Super(), AttackCollisionProfileName("AbilityEffect")
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	bStopWhenAbilityEnd = true;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Down"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Rigidity"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Attacking"));

	GrantTagsDuringAttack.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Attacking"));

	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Block.Move"));

	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Ability.Meelee.Basic.Start"));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Ability.Meelee"));

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Meelee"));
	
	DamageCreator = CreateDefaultSubobject<UAbilityDamageCreator>("DamageCreator");

	CustomRigidityAnim = nullptr;
}

void UGameplayAbility_Meelee::OnMontageEnded()
{
	check(CurrentActorInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Meelee::OnMontageCancelled()
{
	check(CurrentActorInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Meelee::OnMontageBlendOut()
{
	check(CurrentActorInfo);

}

void UGameplayAbility_Meelee::OnMontageInterrupted()
{
	check(CurrentActorInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UGameplayAbility_Meelee::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) 
	{ 
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true); 
		return;
	}
	
	if (bAutoRotateToTarget)
	{
		if (ActorInfo->PlayerController.IsValid())
		{
			AActionPFPlayerController* PC = Cast<AActionPFPlayerController>(ActorInfo->PlayerController.Get());
			if (ULockOnTargetComponent* Target = PC->GetLockOnTarget())
			{
				FVector Dir = Target->GetComponentLocation() - ActorInfo->AvatarActor->GetActorLocation();
				Dir.Z = 0;
				ActorInfo->AvatarActor->SetActorRotation(Dir.Rotation());
			}
		}
	}

	FGameplayEffectContextHandle ContextHandle = ActorInfo->AbilitySystemComponent->MakeEffectContext();
	int32 TempAbilityLevel = GetAbilityLevel();

	check(DamageCreator);
	DamageEffectSpecHandle.Data = MakeShared<FGameplayEffectSpec>(DamageCreator->CreateDamageEffect(), ContextHandle, TempAbilityLevel);

	for (auto& OptionalEffect : OptionEffects)
	{
		UGameplayEffect* OptionalEffectObject = NewObject<UGameplayEffect>(this, OptionalEffect);
		FGameplayEffectSpec OptionalEffectSpec(OptionalEffectObject, ContextHandle, TempAbilityLevel);
		FGameplayEffectSpecHandle OptionalEffectHandle(&OptionalEffectSpec);

		DamageEffectSpecHandle.Data->TargetEffectSpecs.Add(OptionalEffectHandle);
	}
	
	UAbilityTask_PlayMontageAndWait* MeeleeAbilityTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "MeeleeAnim", MeeleeMontage, 1, NAME_None, bStopWhenAbilityEnd);
	ensure(MeeleeAbilityTask);
	MeeleeAbilityTask->OnCompleted.AddDynamic(this, &UGameplayAbility_Meelee::OnMontageEnded);
	MeeleeAbilityTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_Meelee::OnMontageBlendOut);
	MeeleeAbilityTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_Meelee::OnMontageInterrupted);
	MeeleeAbilityTask->OnCancelled.AddDynamic(this, &UGameplayAbility_Meelee::OnMontageCancelled);
	MeeleeAbilityTask->ReadyForActivation();

	UAbilityTask_WaitEnterMontageSection* WaitAttackSection = UAbilityTask_WaitEnterMontageSection::WaitEnterMontageSection(this, "WaitAttackSectionEnter", MeeleeMontage, "Attack");
	WaitAttackSection->OnEnterSection.AddDynamic(this, &UGameplayAbility_Meelee::OnEnterAttackSection);
	WaitAttackSection->ReadyForActivation();

	if (!GrantTagsDuringAttack.IsEmpty())
	{
		UAbilityTask_GrantTags* GrantTagsTask = UAbilityTask_GrantTags::GrantTags(this, "GrantTagsDuringAttack", GrantTagsDuringAttack);
		GrantTagsTask->ReadyForActivation();
	}
}

bool UGameplayAbility_Meelee::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;
	return MeeleeMontage != nullptr;
}

void UGameplayAbility_Meelee::OnEnterAttackSection(UAnimMontage* InMeeleeMontage, const FName& InSectionName)
{
	UAbilityTask_WaitNewCollision* MeeleeAttackShape = UAbilityTask_WaitNewCollision::WaitNewCollision(this, "AttackTask", false);
	switch (CreateAttackShapePolicy)
	{
	case ECreateAttackShapePolicy::Sphere:
		MeeleeAttackShape->MakeSphereCollision(SphereRadius, AttackCollisionProfileName);
		break;
	case ECreateAttackShapePolicy::Capsule:
		MeeleeAttackShape->MakeCapsuleCollision(CapsuleRadius, CapsuleHalfHeight, AttackCollisionProfileName);
		break;
	case ECreateAttackShapePolicy::Box:
		MeeleeAttackShape->MakeBoxCollision(BoxExtent, AttackCollisionProfileName);
		break;
	case ECreateAttackShapePolicy::Custom:
		MeeleeAttackShape->SetCustomCollision(CreateCustomAttackShape());
		break;
	default:
		break;
	}

	MeeleeAttackShape->AddIgnoreActor(GetAvatarActorFromActorInfo());
	MeeleeAttackShape->OnOverlapDel.AddDynamic(this, &UGameplayAbility_Meelee::OnAttackBeginOverlap);
	MeeleeAttackShape->AttachToAvatar(FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	MeeleeAttackShape->SetShapeTransform(AttachedRelativeTransform);

	if (bOverrideCollisionLifeTime)
	{
		float PlayRate = 1;
		MeeleeAttackShape->SetCollisionLifeTime(CollisionLifeTimeOverride / PlayRate);
	}

	MeeleeAttackShape->ReadyForActivation();

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningActorFromActorInfo();
	CurrentActorInfo->AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("CommonEvent.OnAttackStart")), &EventData);


	UAbilityTask_WaitEnterMontageSection* WaitPostDelay = UAbilityTask_WaitEnterMontageSection::WaitEnterMontageSection(this, "WaitPostDelay", MeeleeMontage, "PostDelay");
	WaitPostDelay->OnEnterSection.AddDynamic(this, &UGameplayAbility_Meelee::OnEnterPostDelay);
	WaitPostDelay->ReadyForActivation();
}

void UGameplayAbility_Meelee::OnEnterPostDelay(UAnimMontage* InMeeleeMontage, const FName& InSectionName)
{
	if (!GrantTagsDuringAttack.IsEmpty())
	{
		EndTaskByInstanceName("GrantTagsDuringAttack");
	}

	if (!bOverrideCollisionLifeTime)
	{
		EndTaskByInstanceName("AttackTask");
	}
}

void UGameplayAbility_Meelee::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::InstancedPerActor)
	{
		DamageEffectSpecHandle.Clear();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Meelee::OnAttackBeginOverlap(const FGameplayAbilityTargetDataHandle& TargetData)
{
	IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>( GetAvatarActorFromActorInfo());
	
	for (auto& Target : TargetData.Data)
	{
		TArray<TWeakObjectPtr<AActor>> TargetActors = Target->GetActors();
		for (TWeakObjectPtr<AActor>& TargetActor : TargetActors)
		{
			if (!TargetActor.IsValid() || TeamAgent->GetTeamAttitudeTowards(*TargetActor.Get()) == ETeamAttitude::Friendly) continue;
			IAbilitySystemInterface* TargetASI = Cast<IAbilitySystemInterface>(TargetActor);
			UAbilitySystemComponent* TargetASC = TargetASI->GetAbilitySystemComponent();
			if (TargetASC == nullptr) continue;

			checkf(DamageEffectSpecHandle.IsValid(), TEXT("Can't find Damage Effect In : %s"), *GetName());
			CurrentActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data, TargetASC);

			FGameplayEventData EventData;
			EventData.Instigator = GetAvatarActorFromActorInfo();
			EventData.Target = TargetActor.Get();
			CurrentActorInfo->AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("CommonEvent.OnAttackTarget")), &EventData);
		
			UAbility_Rigidity::RigidityToTarget(RigidityClass, GetCurrentActorInfo()->AbilitySystemComponent.Get(), TargetASC, RigidityTime, Target->GetHitResult(), CustomRigidityAnim);

			FGameplayAbilitySpec KnockbackSpec(KnockbackClass, 1, -1, GetAvatarActorFromActorInfo());
			TargetASC->GiveAbilityAndActivateOnce(KnockbackSpec);
		}
	}
}




