// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "AttributeEffect.generated.h"

/**
 * 
*/


UCLASS()
class ACTIONPORTFOLIO_API UAttributeEffect_Base : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UAttributeEffect_Base();
};
UCLASS()
class ACTIONPORTFOLIO_API UAttributeEffect_Enemy : public UAttributeEffect_Base
{
	GENERATED_BODY()

	UAttributeEffect_Enemy();
};

UCLASS()
class ACTIONPORTFOLIO_API UAttributeEffect_Player : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UAttributeEffect_Player();
};
