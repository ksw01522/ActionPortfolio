// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "DamageEffect.generated.h"

/**
 * 
 */




USTRUCT(BlueprintType)
struct FDamageDataStruct
{
	GENERATED_BODY()
public:
	FDamageDataStruct();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UDamageEffect> DamageClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0"))
	float Damage_Absolute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0"))
	float Damage_Relative;
};

UCLASS(Abstract)
class ACTIONPORTFOLIO_API UDamageEffect : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UDamageEffect();

public:
	static FGameplayEffectSpecHandle CreateDamageEffectSpec(const FDamageDataStruct& InDamage, const FGameplayEffectContextHandle& ContextHandle);
};

UCLASS()
class ACTIONPORTFOLIO_API UDamageEffect_Meelee : public UDamageEffect
{
	GENERATED_BODY()
public:
	UDamageEffect_Meelee();
};

UCLASS()
class ACTIONPORTFOLIO_API UDamageEffect_Fire : public UDamageEffect
{
	GENERATED_BODY()
public:
	UDamageEffect_Fire();
};

UCLASS()
class ACTIONPORTFOLIO_API UDamageEffect_Ice : public UDamageEffect
{
	GENERATED_BODY()
public:
	UDamageEffect_Ice();
};

UCLASS()
class ACTIONPORTFOLIO_API UDamageEffect_Electric : public UDamageEffect
{
	GENERATED_BODY()
public:
	UDamageEffect_Electric();
};




