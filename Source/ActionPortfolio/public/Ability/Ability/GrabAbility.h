// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "GrabAbility.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UGrabAbility : public UActionPFGameplayAbility
{
	GENERATED_BODY()
	
};


UCLASS()
class ACTIONPORTFOLIO_API UGrabTargetAbility : public UActionPFGameplayAbility
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<UGrabAbility> GrabAbility;
	TWeakObjectPtr<AActor> Grappler; 

protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	bool DetachFromGrappler();

public:
	UGrabAbility* GetGrabAbility() const { return GrabAbility.Get(); }

	AActor* GetGrappler() const { return Grappler.Get(); }
};