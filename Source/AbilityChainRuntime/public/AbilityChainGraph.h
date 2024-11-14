// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilityChainGraph.generated.h"


class UAbilityChainNode;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ABILITYCHAINRUNTIME_API UAbilityChainGraph : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TObjectPtr<UAbilityChainNode> StartNode;

	UPROPERTY()
	TArray<TObjectPtr<UAbilityChainNode>> AllNodes;

public:

	UAbilityChainNode* GetStartNode() const {return StartNode;}








#if WITH_EDITORONLY_DATA
//어빌리티 체인 에디터를 위한 EdGraph
public:
	TObjectPtr<class UEdGraph> EdGraph = nullptr;
#endif
};
