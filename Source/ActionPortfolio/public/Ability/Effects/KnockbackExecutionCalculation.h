// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "KnockbackExecutionCalculation.generated.h"

/**
 * 
 */



UCLASS(Abstract)
class ACTIONPORTFOLIO_API UKnockbackExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UKnockbackExecutionCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	virtual FVector MakeKnockbackVector(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const {return FVector::ZeroVector; };
};


UCLASS()
class ACTIONPORTFOLIO_API UKncokback_Forward : public UKnockbackExecutionCalculation
{
	GENERATED_BODY()
public:
	UKncokback_Forward();

protected:


private:
	virtual FVector MakeKnockbackVector(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const override;
};