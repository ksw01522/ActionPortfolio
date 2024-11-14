// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphEditor/EdGraph/EdGraph_AbilityChain.h"
#include "AbilityChainGraph.h"

bool UEdGraph_AbilityChain::Modify(bool bAlwaysMarkDirty)
{
	bool FinalValue = Super::Modify(bAlwaysMarkDirty);

	GetAbilityChainGraph()->Modify();

	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		Nodes[i]->Modify();
	}

	return FinalValue;
}

void UEdGraph_AbilityChain::PostEditUndo()
{
	Super::PostEditUndo();

	NotifyGraphChanged();
}

void UEdGraph_AbilityChain::PostLoad()
{
	Super::PostLoad();
}

UAbilityChainGraph* UEdGraph_AbilityChain::GetAbilityChainGraph() const
{
	return CastChecked<UAbilityChainGraph>(GetOuter());
}
