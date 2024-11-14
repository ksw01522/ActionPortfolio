// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilityChainEditor.h"
//#include "AbilityChainNodeFactory.h"
//#include "AssetTypeActions_AbilityChainSession.h"
//#include "AbilityChainAssetEditor/AbilityChainEditorStyle.h"
//#include "AssetTypeActions_AbilityChainSession.h"
#include "AssetTypeActions_ACGraph.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "K2Node_Event.h"

//#include "FAbilityChainEditorUtilities.h"



DEFINE_LOG_CATEGORY(AbilityChainEditor)
IMPLEMENT_MODULE(FAbilityChainEditor, AbilityChainEditor)

#define LOCTEXT_NAMESPACE "AbilityChainEditor"

void FAbilityChainEditor::StartupModule()
{
	//FAbilityChainEditorStyle::Initialize();

	//RegisterNodeFactory();
	RegisterCategory();

	//RegisterBlueprintEvents();
}

void FAbilityChainEditor::ShutdownModule()
{	
// Unregister all the asset types that we registered
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
		}
	}

	//UnRegisterNodeFactory();

	//FAbilityChainEditorStyle::Shutdown();
}


void FAbilityChainEditor::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

void FAbilityChainEditor::RegisterCategory()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_ACGraph()));
}

void FAbilityChainEditor::RegisterNodeFactory()
{
	/*GraphPanelNodeFactory_AbilityChain = MakeShareable(new FGraphPanelNodeFactory_AbilityChain());
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_AbilityChain);*/
}

void FAbilityChainEditor::UnRegisterNodeFactory()
{
	/*if (GraphPanelNodeFactory_AbilityChain.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactory_AbilityChain);
		GraphPanelNodeFactory_AbilityChain.Reset();
	}*/
}

void FAbilityChainEditor::RegisterBlueprintEvents()
{
	/*FKismetEditorUtilities::RegisterOnBlueprintCreatedCallback(
		this,
		UAbilityChainEvent::StaticClass(),
		FKismetEditorUtilities::FOnBlueprintCreated::CreateRaw(this, &FAbilityChainEditor::HandleNewAbilityChainEventBlueprintCreated)
	);*/

}

void FAbilityChainEditor::HandleNewAbilityChainEventBlueprintCreated(UBlueprint* Blueprint)
{

	//if (!Blueprint || Blueprint->BlueprintType != BPTYPE_Normal)
	//{
	//	return;
	//}

	//Blueprint->bForceFullEditor = true;
	//UK2Node_Event* EventNode = FAbilityChainEditorUtilities::BlueprintGetOrAddEvent(
	//	Blueprint,
	//	GET_FUNCTION_NAME_CHECKED(UAbilityChainEvent, OnCalledEvent),
	//	UAbilityChainEvent::StaticClass()
	//);
	//if (EventNode)
	//{
	//	Blueprint->LastEditedDocuments.Add(EventNode->GetGraph());
	//}
	//
}


#undef LOCTEXT_NAMESPACE
