// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Ability_Rigidity.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ACTIONPORTFOLIO_API URigidityEventData : public UObject
{
	GENERATED_BODY()
public:
	URigidityEventData();

	float RigidityTime;

	const FHitResult* HitResult;

	UAnimMontage* RigidityAnim;
};

UCLASS()
class ACTIONPORTFOLIO_API UAbility_Rigidity : public UGameplayAbility
{
	GENERATED_BODY()

	UAbility_Rigidity();
	
private:
	float RemainTime;

	FDelegateHandle DelegateHandle;

protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void WaitIsInAirInRigidity(const FGameplayTag IsInAirTag, int TagCount);

	UFUNCTION()
	void OnLandedInRigidity(FGameplayEventData Payload);

public:
	UFUNCTION()
	void OnRigidityEnd();

	UFUNCTION()
	void RigidityTick(float DeltaTime);

	void IsInAirWithRigidity();

	void CharacterDown();

	UFUNCTION()
	void OnRigidityMontageEnd();

	static FGameplayAbilitySpecHandle RigidityToTarget(TSubclassOf<UAbility_Rigidity> InClass, UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, float InTime, const FHitResult* HitResult = nullptr, UAnimMontage* InAnim = nullptr);
};
