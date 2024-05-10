// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "Ability_Whirlwind.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UAbility_Whirlwind : public UActionPFGameplayAbility
{
	GENERATED_BODY()
	
public:
	UAbility_Whirlwind();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bStopWhenAbilityEnd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> WhirlwindMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Whirlwind", meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	int DamageCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Whirlwind", meta = (AllowPrivateAccess = "true"))
	float DamageIntervalTime;

	float WhirlwindMaxTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer GrantTagsDuringAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind|Damage", Instanced, meta = (AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	TObjectPtr<class UAbilityDamageCreator> DamageCreator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind|Collision", meta = (AllowPrivateAccess = "true"))
	float WhirlwindRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind|Collision", meta = (AllowPrivateAccess = "true"))
	FName CollisionProfileName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind|Collision", meta = (AllowPrivateAccess = "true"))
	FTransform RelativeTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind|Rigidity", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UAbility_Rigidity> RigidityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind|Rigidity", meta = (AllowPrivateAccess = "true"))
	float RigidityTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind|Knockback", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UAbility_Knockback> KnockbackClass;

	TWeakObjectPtr<class UAbilityTask_WaitInputRelease> WaitInputRelease;
	TWeakObjectPtr<class UAbilityTask_Repeat> AttackRepeat;
protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


private:
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UFUNCTION()
	void OnEnterAttackSection(UAnimMontage* InMeeleeMontage, const FName& InSectionName);

	UFUNCTION()
	void WhirlwindAttackCheck(int32 ActionNumber);

	UFUNCTION()
	void OnWhilrwindRelease(float TimeHeld);

	UFUNCTION()
	void OnWhirwindAttackFinished(int32 ActionNumber);

	void OnWhirlwindEnd();

protected:
	UFUNCTION()
	virtual void OnMontageEnded();
	UFUNCTION()
	virtual void OnMontageCancelled();

	UFUNCTION()
	virtual void OnMontageBlendOut();

	UFUNCTION()
	virtual void OnMontageInterrupted();

	void GetKnockbackVector(AActor* Target, FVector& OnGround, FVector& InAir);
};
