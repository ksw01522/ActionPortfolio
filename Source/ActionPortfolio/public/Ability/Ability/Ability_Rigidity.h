// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Ability_Rigidity.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FRigidityData
{
	GENERATED_BODY()
	FRigidityData(){}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Rigidity")
	TSubclassOf<class UAbility_Rigidity> RigidityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Rigidity")
	float RigidityTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Rigidity", meta = (Categories = "Animation.Rigidity"))
	FGameplayTag RigidityAnimTag;
};

UCLASS(BlueprintType)
class ACTIONPORTFOLIO_API URigidityEventData : public UObject
{
	GENERATED_BODY()

	URigidityEventData();

private:
	float RigidityTime;

	FGameplayTag AnimTag;

public:	
	void InitRigidityData(const FRigidityData& InData)
	{
		RigidityTime = InData.RigidityTime;
		AnimTag = InData.RigidityAnimTag;
	}

	float GetRigidityTime() const { return RigidityTime; }

	FGameplayTag GetAnimTag() const { return AnimTag; }
};

UCLASS()
class ACTIONPORTFOLIO_API UAbility_Rigidity : public UGameplayAbility
{
	GENERATED_BODY()

	UAbility_Rigidity();
	
private:
	float RemainTime;

	FDelegateHandle DelegateHandle;

	UAnimMontage* RigidityAnim;

private:
	void PlayRigidityAnim(const FGameplayTag InTag);

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

	void RigidityInAir();

	void CharacterDown();

	UFUNCTION()
	void OnRigidityMontageEnd();

	static FGameplayAbilitySpecHandle RigidityToTarget( UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FRigidityData& InData);
};
