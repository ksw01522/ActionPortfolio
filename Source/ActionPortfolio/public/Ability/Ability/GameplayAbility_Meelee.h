// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "GameplayAbility_Meelee.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class ECreateAttackShapePolicy : uint8
{
	Sphere = 0,
	Capsule,
	Box,
	Custom
};

UCLASS(Abstract)
class ACTIONPORTFOLIO_API UGameplayAbility_Meelee : public UActionPFGameplayAbility
{
	GENERATED_BODY()
public:
	UGameplayAbility_Meelee();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bStopWhenAbilityEnd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> MeeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", Instanced, meta = (AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	TObjectPtr<class UAbilityDamageCreator> DamageCreator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> OptionEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer GrantTagsDuringAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Collision", meta = (AllowPrivateAccess = "true"))
	FName AttackCollisionProfileName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Collision", meta = (AllowPrivateAccess = "true"))
	FTransform AttachedRelativeTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Collision", meta = (AllowPrivateAccess = "true"))
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Collision", meta = (AllowPrivateAccess = "true"))
	ECreateAttackShapePolicy CreateAttackShapePolicy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Collision", meta = (AllowPrivateAccess = "true", EditConditionHides, EditCondition = "CreateAttackShapePolicy == ECreateAttackShapePolicy::Sphere"))
	float SphereRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Collision", meta = (AllowPrivateAccess = "true", EditConditionHides, EditCondition = "CreateAttackShapePolicy == ECreateAttackShapePolicy::Capsule"))
	float CapsuleRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Collision", meta = (AllowPrivateAccess = "true", EditConditionHides, EditCondition = "CreateAttackShapePolicy == ECreateAttackShapePolicy::Capsule"))
	float CapsuleHalfHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Collision", meta = (AllowPrivateAccess = "true", EditConditionHides, EditCondition = "CreateAttackShapePolicy == ECreateAttackShapePolicy::Box"))
	FVector BoxExtent;

	UPROPERTY()
	bool bOverrideCollisionLifeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Collision", meta = (AllowPrivateAccess = "true", EditCondition = "bOverrideCollisionLifeTime"))
	float CollisionLifeTimeOverride;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Rigidity", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UAbility_Rigidity> RigidityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Rigidity", meta = (AllowPrivateAccess = "true"))
	float RigidityTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee|Rigidity", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CustomRigidityAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee")
	TSubclassOf<class UAbility_Knockback> KnockbackClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee")
	bool bAutoRotateToTarget;

protected:
	UFUNCTION()
	virtual void OnMontageEnded();
	UFUNCTION()
	virtual void OnMontageCancelled();

	UFUNCTION()
	virtual void OnMontageBlendOut();

	UFUNCTION()
	virtual void OnMontageInterrupted();

protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;


private:
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UFUNCTION()
	virtual void OnEnterAttackSection(UAnimMontage* InMeeleeMontage, const FName& InSectionName);

protected:
	UFUNCTION()
	virtual void OnEnterPostDelay(UAnimMontage* InMeeleeMontage, const FName& InSectionName);

protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	UFUNCTION()
	void OnAttackBeginOverlap(const FGameplayAbilityTargetDataHandle& TargetData);


	UFUNCTION(BlueprintNativeEvent)
	UPrimitiveComponent* CreateCustomAttackShape();
	virtual UPrimitiveComponent* CreateCustomAttackShape_Implementation() { return nullptr;};
};
