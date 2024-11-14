// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilityChainEdge.generated.h"


class UAbilityChainNode;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ABILITYCHAINRUNTIME_API UAbilityChainEdge : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Edge", meta = (AllowPrivateAccess = "true"))
	int Priority = 0;

	UPROPERTY()
	TWeakObjectPtr<UAbilityChainNode> From;

	UPROPERTY()
	TWeakObjectPtr<UAbilityChainNode> To;

public:
	int GetPriority() const {return Priority;}

	UFUNCTION(BlueprintPure)
	UAbilityChainNode* GetFromNode() const;

	UFUNCTION(BlueprintPure)
	UAbilityChainNode* GetToNode() const;
};
