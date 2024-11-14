// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "CustomProgressBar.generated.h"

/**
 * 
 * 
 */

UCLASS()
class ACTIONPORTFOLIO_API UCustomProgressBar : public UProgressBar
{
	GENERATED_BODY()

private:
	float MaxValue;
	float CurrentValue;


	void UpdateCustomProgressBar();

public:
	void SetMaxValue(float InValue);
	void SetCurrentValue(float InValue);

};
