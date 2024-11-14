// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageExecutionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UDamageExecutionCalculation();

};

UCLASS()
class ACTIONPORTFOLIO_API UDamageExecutionCalculation_Meelee : public UDamageExecutionCalculation
{
	GENERATED_BODY()

public:
	UDamageExecutionCalculation_Meelee();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};

UCLASS()
class ACTIONPORTFOLIO_API UDamageExecutionCalculation_Fire : public UDamageExecutionCalculation
{
	GENERATED_BODY()

public:
	UDamageExecutionCalculation_Fire();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};

UCLASS()
class ACTIONPORTFOLIO_API UDamageExecutionCalculation_Ice : public UDamageExecutionCalculation
{
	GENERATED_BODY()

public:
	UDamageExecutionCalculation_Ice();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};

UCLASS()
class ACTIONPORTFOLIO_API UDamageExecutionCalculation_Electric : public UDamageExecutionCalculation
{
	GENERATED_BODY()

public:
	UDamageExecutionCalculation_Electric();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};

UCLASS()
class ACTIONPORTFOLIO_API UDamageExecutionCalculation_Poison: public UDamageExecutionCalculation
{
	GENERATED_BODY()

public:
	UDamageExecutionCalculation_Poison();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};