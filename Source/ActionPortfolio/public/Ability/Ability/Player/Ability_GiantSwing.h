// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/GrabAbility.h"
#include "Ability/Effects/Damage/DamageEffect.h"
#include "Ability/Ability/Ability_Rigidity.h"
#include "Ability_GiantSwing.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ACTIONPORTFOLIO_API UAbility_GiantSwing : public UGrabAbility
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<class UAbility_GiantSwingTarget> SwingTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> SwingAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	FName GrabSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	FName GrabTargetSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float GrabDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float ScanCenterDelta;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	FVector ScanBoxHalfExtent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rigidity", meta = (AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	FRigidityData RigidityData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Swing", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float SwingTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Swing", meta = (AllowPrivateAccess = "true"))
	FVector2D ThrowPower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	FDamageDataStruct SwingDamage;

public:
	FName GetGrabSocketName() const {return GrabSocket;}
	FName GetGrabTargetSocketName() const {return GrabTargetSocket;}

	void SetSwingTargetAbility(class UAbility_GiantSwingTarget* Target);

protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


private:
	UFUNCTION()
	void OnAnimComplete();

	UFUNCTION()
	void OnAnimInterrupted();

	UFUNCTION()
	void OnAnimCancelled();

	UFUNCTION()
	void ScanGrabTarget();

	void OnGrabSucceed(const FGameplayAbilitySpecHandle TargetHandle, class UAbilitySystemComponent* TargetASC);


	UFUNCTION()
	void ThrowTarget();

public:
	FRigidityData GetRigidityData() const { return RigidityData; }

	FGameplayEffectSpecHandle CreateDamageEffect() const;
};

UCLASS(NotBlueprintable)
class ACTIONPORTFOLIO_API UAbility_GiantSwingTarget : public UGrabTargetAbility
{
	GENERATED_BODY()

	UAbility_GiantSwingTarget();

	friend class UAbility_GiantSwing;

private:
	FName GrabSocket;

	FName GrabTargetSocket;

	FGameplayEffectSpecHandle DamageHandle;

	FRigidityData RigidityData;

	void OnThrowed(FVector Direction, FVector2D Power);

	void OnCancelled();

	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override final;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override final;

	UFUNCTION()
	void OnLanded(FGameplayEventData Payload);

public:
	void SetDamageHandle(const FGameplayEffectSpecHandle& InHandle) { DamageHandle = InHandle; }
};
