// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Player/Ability_GutPunch.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ActionPortfolio.h"
#include "Character/GAEffectActor.h"
#include "Character/Player/PlayerCharacter.h"

void UAbility_GutPunch::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitCheck(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Super::ActivateAbility_CPP(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitGameplayEvent* ImpactSpawnEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Ability.PublicEvent.Spawn"), nullptr, true, true);
	ImpactSpawnEvent->EventReceived.AddDynamic(this, &UAbility_GutPunch::OnImpactSpawnEventReceived);

	ImpactSpawnEvent->ReadyForActivation();
}

void UAbility_GutPunch::OnImpactSpawnEventReceived(FGameplayEventData EventData)
{
	if (GutImpactActorClass.GetDefaultObject() == nullptr) {
		PFLOG(Warning, TEXT("Can't Find Gut Impact Actor Class."));
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(PlayerCharacter))
	{
		PFLOG(Warning, TEXT("Don't Activate From PlayerCharacter."));
		return;
	}

	FTransform FinalImpactSpawnTransform = PlayerCharacter->GetMesh()->GetSocketTransform("FX_handDir_r");

	AGAEffectActor* ImpactActor = GetWorld()->SpawnActorDeferred<AGAEffectActor>(GutImpactActorClass, FinalImpactSpawnTransform, GetOwningActorFromActorInfo(), PlayerCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	FGameplayEffectSpecHandle ImpactDamageSpecHandle = MakeOutgoingGameplayEffectSpec(ImpactDamageEffectClass, GetAbilityLevel());
	ImpactActor->EffectSpecHandles.Add(ImpactDamageSpecHandle);

	ImpactActor->FinishSpawning(FinalImpactSpawnTransform);
}