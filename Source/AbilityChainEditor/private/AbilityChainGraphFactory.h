// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "AbilityChainGraphFactory.generated.h"

/**
 * 
 */
UCLASS()
class UAbilityChainGraphFactory : public UFactory
{
	GENERATED_BODY()
public:
	UAbilityChainGraphFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual FText GetDisplayName() const override;
	virtual FString GetDefaultNewAssetName() const override;
};
