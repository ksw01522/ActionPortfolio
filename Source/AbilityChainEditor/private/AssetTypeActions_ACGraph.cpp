// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetTypeActions_ACGraph.h"
#include "AbilityChainGraph.h"
#include "AbilityChainEditor.h"
#include "GraphEditor/AssetEditor_AbilityChainGraph.h"


FText FAssetTypeActions_ACGraph::GetName() const
{
	return NSLOCTEXT("AbilityChainEditor", "ATAACGraphName", "AbilityChain Graph");
}

FColor FAssetTypeActions_ACGraph::GetTypeColor() const
{
	return FColor(129, 196, 115);
}

UClass* FAssetTypeActions_ACGraph::GetSupportedClass() const
{
	return UAbilityChainGraph::StaticClass();
}

void FAssetTypeActions_ACGraph::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	LOG_WARNING(TEXT("Open Asset Editor AbilityChain Graph Mode : {%s}"), 
				*(Mode == EToolkitMode::WorldCentric ? FString("WorldCentric") : FString("Standalone")));
	
	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UAbilityChainGraph* Graph = Cast<UAbilityChainGraph>(*ObjIt))
		{
			TSharedRef<FAssetEditor_AbilityChainGraph> NewGraphEditor(new FAssetEditor_AbilityChainGraph());
			NewGraphEditor->InitAbilityChainGraphEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}

