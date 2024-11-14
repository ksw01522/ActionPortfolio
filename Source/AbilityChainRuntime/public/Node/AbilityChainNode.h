// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilityChainNode.generated.h"

/**
 * 
 */
class UGameplayAbility;
class UAbilityChainEdge;
class UAbilitySystemComponent;
class IAbilitySystemInterface;

UCLASS()
class ABILITYCHAINRUNTIME_API UAbilityChainNode : public UObject
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY()
	TArray<TObjectPtr<UAbilityChainEdge>> From;

	UPROPERTY()
	TArray<TObjectPtr<UAbilityChainEdge>> To;

public:
	TSubclassOf<UGameplayAbility> GetAbilityClass() const {return AbilityClass;}

	bool TryActivateAbility(UAbilitySystemComponent* InASC) const;
	bool TryActivateAbility(IAbilitySystemInterface* InASI) const;
};
