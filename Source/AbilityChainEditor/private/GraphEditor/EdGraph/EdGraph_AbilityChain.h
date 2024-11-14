// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph_AbilityChain.generated.h"

class UAbilityChainGraph;
class UAbilityChainNode;
class UEdGraphNode_AbilityChainNode;
class UAbilityChainEdge;
class UEdGraphNode_AbilityChainEdge;
/**
 * 
 */
UCLASS()
class UEdGraph_AbilityChain : public UEdGraph
{
	GENERATED_BODY()
	
public:
	UEdGraph_AbilityChain(){}
	virtual ~UEdGraph_AbilityChain(){}

private:
	UPROPERTY(Transient)
	TMap<UAbilityChainNode*, UEdGraphNode_AbilityChainNode*> NodeMap;

	UPROPERTY(Transient)
	TMap<UAbilityChainEdge*, UEdGraphNode_AbilityChainEdge*> EdgeMap;

public:
	virtual bool Modify(bool bAlwaysMarkDirty = true) override;
	virtual void PostEditUndo() override;

	virtual void PostLoad() override;

	//void RebuildAbilityChainGraph();

	UAbilityChainGraph* GetAbilityChainGraph() const;
};
