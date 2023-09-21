// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/GameplayAbility_Meelee.h"
#include "Ability_BossJumpFall.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ACTIONPORTFOLIO_API UAbility_BossJumpFall : public UGameplayAbility_Meelee
{
	GENERATED_BODY()
	
	UAbility_BossJumpFall();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpFall", meta = (AllowPrivateAccess = "true"))
	float FlightTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpFall", meta = (AllowPrivateAccess = "true"))
	float CustomGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpFall", meta = (AllowPrivateAccess = "true"))
	float JumpFallImpactRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpFall", meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	int MaxJumpFallCount;
	int RemainJumpFallCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpFall", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ADangerZone> DangerZoneClass;

	TWeakObjectPtr<ADangerZone> DangerZonePtr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpFall", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* OnLandParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpFall", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* OnJumpParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpFallExplosion", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* OnExplosionParticle;

	TWeakObjectPtr<class AActionPortfolioCharacter> TargetCharacter;

	FVector FallDownLocation;

protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnLandEventReceived(FGameplayEventData EventData);

	UFUNCTION()
	void OnJumpEventReceived(FGameplayEventData EventData);

	UFUNCTION()
	void OnJumpFallEnd(UGameplayAbility* Ability);

	void MakeFallDownLocation();

	UFUNCTION()
	void OnExplosionEventReceived(FGameplayEventData EventData);
};
