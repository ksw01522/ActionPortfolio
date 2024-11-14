// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/GrabAbility.h"

void UGrabTargetAbility::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(TriggerEventData);

	GrabAbility = Cast<UGrabAbility>(TriggerEventData->OptionalObject);
	check(GrabAbility.IsValid())

	Grappler = GrabAbility->GetAvatarActorFromActorInfo();
}

bool UGrabTargetAbility::DetachFromGrappler()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if(Avatar == nullptr) return false;

	AActor* AttachParent = Avatar->GetAttachParentActor();
	if(AttachParent != Grappler) return false;

	Avatar->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false));
	return true;
}
