// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/GameplayAbility_Meelee.h"
#include "Ability_PlayerFallDown.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ACTIONPORTFOLIO_API UAbility_PlayerFallDown : public UGameplayAbility_Meelee
{
	GENERATED_BODY()
	
	UAbility_PlayerFallDown();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FallDown", meta = (AllowPrivateAccess = "true"))
	float FallDownSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FallDown", meta = (AllowPrivateAccess = "true"))
	float FallDownImpactRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FallDown", meta = (AllowPrivateAccess = "true"))
	float FallDownHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FallDown", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystem> FallDownImpactParticle;

	FVector PreFallDownLocation;


protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnLandEventReceived(FGameplayEventData EventData);

	UFUNCTION()
	void OnPreFallDownPosReached();

	UFUNCTION()
	void OnFallDownEnded(UGameplayAbility* Ability);
};
