// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/GameplayAbility_Meelee.h"
#include "Ability_PlayerPierce.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UAbility_PlayerPierce : public UGameplayAbility_Meelee
{
	GENERATED_BODY()
	
	UAbility_PlayerPierce();


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pierce", meta = (AllowPrivateAccess = "true", ClampMin = 0))
	int MaxPierceCount;

	int RemainPierceCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pierce", meta = (AllowPrivateAccess = "true", ClampMin = 0))
	float MaxActivePierceTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pierce", meta = (AllowPrivateAccess = "true"))
	float PierceDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pierce|PreMove", meta = (AllowPrivateAccess = "true"))
	float MaxPreMoveTime = 0.1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pierce", meta = (AllowPrivateAccess = "true", ClampMin = 0))
	float MaxPierceTime;

	FVector PrePiercePos;


	TWeakObjectPtr<class AActionPortfolioCharacter> TargetCharacter;

private:
	void EndPlayerPierce();

	AActionPortfolioCharacter* SearchTargetCharacter();

protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnPrePiercePosLeached();

	UFUNCTION()
	void OnPierceEnd();

public:
	int GetMaxPierceCount() const {return MaxPierceCount;}
};
