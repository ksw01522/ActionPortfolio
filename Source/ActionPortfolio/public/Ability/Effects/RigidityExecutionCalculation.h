// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RigidityExecutionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API URigidityExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
	URigidityExecutionCalculation();

public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
