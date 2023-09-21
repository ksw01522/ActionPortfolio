// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "GameplayAbility_Meelee.generated.h"

/**
 * 
 */


UCLASS(Abstract)
class ACTIONPORTFOLIO_API UGameplayAbility_Meelee : public UActionPFGameplayAbility
{
	GENERATED_BODY()
public:
	UGameplayAbility_Meelee();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee", meta = (AllowPrivateAccess = "true"))
	FName TaskInstanceName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee", meta = (AllowPrivateAccess = "true"))
	float PlayLate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee", meta = (AllowPrivateAccess = "true"))
	bool bStopWhenAbilityEnd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer EventTagContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, FActionPFEffectContainer> DamageMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meelee", meta = (AllowPrivateAccess = "true"))
	bool bBlockMoveInActing;

protected:
	UFUNCTION()
	virtual void OnEventReceived(FGameplayEventData EventData);
	UFUNCTION()
	virtual void OnMontageEnded(FGameplayEventData EventData);
	UFUNCTION()
	virtual void OnMontageCancelled(FGameplayEventData EventData);

	void OffBlockMove(UGameplayAbility* Ability);

protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

};
