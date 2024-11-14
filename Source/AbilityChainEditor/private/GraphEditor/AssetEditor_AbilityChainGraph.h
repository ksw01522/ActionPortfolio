// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectSaveContext.h"


class UAbilityChainGraph;

/**
 * 
 */
class FAssetEditor_AbilityChainGraph : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
public:
	FAssetEditor_AbilityChainGraph();
	virtual ~FAssetEditor_AbilityChainGraph();

	void InitAbilityChainGraphEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UAbilityChainGraph* Graph);

protected:
	/** IToolkit interface */
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	virtual FName GetToolkitFName() const override {return FName("FAbilityChainGraphEditor"); }
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::White;}
	virtual FString GetWorldCentricTabPrefix() const override {return TEXT("AbilityChainGraphEditor"); }
	virtual FString GetDocumentationLink() const override{return TEXT(""); }
	virtual void SaveAsset_Execute() override;

	virtual void InitToolMenuContext(FToolMenuContext& MenuContext) override;

	// FSerializableObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	// End of FSerializableObject interface

private:	
		//void BindCommands();

	   //void CreateCommandList();

	   //TSharedPtr<SGraphEditor> GetCurrGraphEditor() const;

	   FGraphPanelSelectionSet GetSelectedNodes() const;

	  // void RebuildAbilityChainGraph();

	   //// Delegates for graph editor commands
	   //void SelectAllNodes();
	   //bool CanSelectAllNodes();
	   //virtual void DeleteSelectedNodes();
	   //bool CanDeleteNodes();
	   //void DeleteSelectedDuplicatableNodes();
	   //void CutSelectedNodes();
	   //bool CanCutNodes();
	   //void CopySelectedNodes();
	   //bool CanCopyNodes();
	   //void PasteNodes();
	   //void PasteNodesHere(const FVector2D& Location);
	   //bool CanPasteNodes();
	   //void DuplicateNodes();
	   //bool CanDuplicateNodes();

	   //void GraphSettings();
	   //bool CanGraphSettings() const;

	   //void AutoArrange();
	   //bool CanAutoArrange() const;

	   //void OnRenameNode();
	   //bool CanRenameNodes() const;

private:
	void OnPackageSavedWithContext(const FString& PackageFileName, UPackage* Package, FObjectPostSaveContext ObjectSaveContext);

	FDelegateHandle OnPackageSavedDelegateHandle;

	void CreateInternalWidgets();

	//TSharedPtr<class FAssetEditorToolbar_Dialogue> ToolbarBuilder;

	//어빌리티 체인 타겟
	UAbilityChainGraph* EditingGraph;

	TSubclassOf<class UEdGraph_AbilityChain> EdGraphSubclass;
	TSubclassOf<class UEdGraphSchema_AbilityChain> EdGraphSchemaSubclass;

	void CreateEdGraph();

	TSharedPtr<SGraphEditor> GraphEditorWidget;
	TSharedRef<SDockTab> SpawnTab_GraphEditor(const FSpawnTabArgs& Args);

	TSharedPtr<FUICommandList> GraphEditorCommands;

	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelectedNodes);

	//노드 프로퍼티 디테일창
	TSharedPtr<class IDetailsView> NodePropertyWidget;
	TSharedRef<SDockTab> SpawnTab_NodePropertyDetail(const FSpawnTabArgs& Args);

	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);

};
