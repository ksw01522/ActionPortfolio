// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "StateEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ACTIONPORTFOLIO_API UStateEffect : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UStateEffect();
};

UCLASS()
class ACTIONPORTFOLIO_API UDebuffEffect : public UStateEffect
{
	GENERATED_BODY()

};

UCLASS()
class ACTIONPORTFOLIO_API UDebuffEffect_Poison : public UDebuffEffect
{
	GENERATED_BODY()

public:
	UDebuffEffect_Poison();
};

UCLASS()
class ACTIONPORTFOLIO_API UDebuffEffect_Stun : public UStateEffect
{
	GENERATED_BODY()

public:
	UDebuffEffect_Stun();
};