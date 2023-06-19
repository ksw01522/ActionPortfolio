// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "KnockbackExecutionCalculation.generated.h"

/**
 * 
 */


UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew, ClassGroup = Damage, Abstract)
class ACTIONPORTFOLIO_API UActionPFKnockbackObject : public UObject
{
	GENERATED_BODY()

public:
	UActionPFKnockbackObject() {
		KnockbackStrength = 0;
		bForceDown = false;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Knockback")
		float KnockbackStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Knockback")
		bool bForceDown;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Knockback")
	virtual FVector GetKnockbackVector(const class AActor* Instigator, const AActor* Target) {return FVector::ZeroVector;}

	UFUNCTION(BlueprintCallable, Category = "Knockback")
	void KnockbackEffect(const AActor* Instigator, const AActor* Target);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Knockback")
	bool IsForceDown() const {return bForceDown;};
};

UCLASS()
class ACTIONPORTFOLIO_API UKncokback_Forward : public UActionPFKnockbackObject
{
	GENERATED_BODY()
public:
	UKncokback_Forward() {}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Knockback")
	float FlyStrength;

public:
	virtual FVector GetKnockbackVector(const class AActor* Instigator, const AActor* Target) override;
};

UCLASS()
class ACTIONPORTFOLIO_API UKnockbackExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UKnockbackExecutionCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
