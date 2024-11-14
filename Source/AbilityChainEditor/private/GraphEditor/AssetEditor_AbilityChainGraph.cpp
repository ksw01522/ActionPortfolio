// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphEditor/AssetEditor_AbilityChainGraph.h"
#include "AbilityChainGraph.h"
#include "Framework/Commands/GenericCommands.h"
#include "AbilityChainEditor.h"

#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "EdGraph/EdGraph_AbilityChain.h"
#include "EdGraph/EdGraphSchema_AbilityChain.h"

#include "IDetailsView.h"
#include "PropertyEditorModule.h"

#define LOCTEXT_NAMESPACE "AbilityChainEditor"

//추가할 탭들
struct FAbilityChainGraphEditorTabs
{
	static const FName NodePropertyID;
	static const FName GraphEditorID;
};

const FName FAbilityChainGraphEditorTabs::NodePropertyID(TEXT("NodeProperty"));
const FName FAbilityChainGraphEditorTabs::GraphEditorID(TEXT("GraphEditor"));

FAssetEditor_AbilityChainGraph::FAssetEditor_AbilityChainGraph()
{
	EditingGraph = nullptr;

	EdGraphSubclass = UEdGraph_AbilityChain::StaticClass();
	EdGraphSchemaSubclass = UEdGraphSchema_AbilityChain::StaticClass();

	OnPackageSavedDelegateHandle = UPackage::PackageSavedWithContextEvent.AddRaw(this, &FAssetEditor_AbilityChainGraph::OnPackageSavedWithContext);
}

FAssetEditor_AbilityChainGraph::~FAssetEditor_AbilityChainGraph()
{
	UPackage::PackageSavedWithContextEvent.Remove(OnPackageSavedDelegateHandle);
}

void FAssetEditor_AbilityChainGraph::InitAbilityChainGraphEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UAbilityChainGraph* Graph)
{
	EditingGraph = Graph;
	CreateEdGraph();

	FGenericCommands::Register();
	//FGraphEditorCommands::Register();
	//어빌리티 체인 전용 커맨드는 아직 보류

	/*if (!ToolbarBuilder.IsValid())
	{

	}*/

	CreateInternalWidgets();

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_AbilityChainEditor_Layout_v0")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewStack()->SetSizeCoefficient(0.6f)->SetHideTabWell(true)
					->AddTab(FAbilityChainGraphEditorTabs::GraphEditorID, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()->SetSizeCoefficient(0.4f)->SetHideTabWell(true)
					->AddTab(FAbilityChainGraphEditorTabs::NodePropertyID, ETabState::OpenedTab)
				)
			)
		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, FName(TEXT("AbilityChainEditorApp")), StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, EditingGraph, false);

	RegenerateMenusAndToolbars();
}

void FAssetEditor_AbilityChainGraph::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_AbilityChainEditor", "Custom Graph Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FAbilityChainGraphEditorTabs::GraphEditorID, FOnSpawnTab::CreateSP(this, &FAssetEditor_AbilityChainGraph::SpawnTab_GraphEditor))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "GraphEditor"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(FAbilityChainGraphEditorTabs::NodePropertyID, FOnSpawnTab::CreateSP(this, &FAssetEditor_AbilityChainGraph::SpawnTab_NodePropertyDetail))
		.SetDisplayName(LOCTEXT("DetailsTab", "Property"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FAssetEditor_AbilityChainGraph::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FAbilityChainGraphEditorTabs::GraphEditorID);
	InTabManager->UnregisterTabSpawner(FAbilityChainGraphEditorTabs::NodePropertyID);
}

FText FAssetEditor_AbilityChainGraph::GetBaseToolkitName() const
{
	return LOCTEXT("AbilityChain EditorAppLabel", "AbilityChain Graph Editor");
}

FText FAssetEditor_AbilityChainGraph::GetToolkitName() const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("AbilityChainName"), FText::FromString(EditingGraph->GetName()));
	return FText::Format(LOCTEXT("AbilityChainEditorToolkitName", "{AbilityChainName}"), Args);
}

FText FAssetEditor_AbilityChainGraph::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(EditingGraph);
}

void FAssetEditor_AbilityChainGraph::SaveAsset_Execute()
{
	if (EditingGraph != nullptr)
	{
		//RebuildGraph();
	}

	FAssetEditorToolkit::SaveAsset_Execute();
}

void FAssetEditor_AbilityChainGraph::InitToolMenuContext(FToolMenuContext& MenuContext)
{
	LOG_ERROR(TEXT("Test"));
}

void FAssetEditor_AbilityChainGraph::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditingGraph);
	Collector.AddReferencedObject(EditingGraph->EdGraph);
}

FGraphPanelSelectionSet FAssetEditor_AbilityChainGraph::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelection;
	
	if (GraphEditorWidget.IsValid())
	{
		CurrentSelection = GraphEditorWidget->GetSelectedNodes();
	}

	return CurrentSelection;
}


void FAssetEditor_AbilityChainGraph::OnPackageSavedWithContext(const FString& PackageFileName, UPackage* Package, FObjectPostSaveContext ObjectSaveContext)
{
	//RebuildGraph();
}

void FAssetEditor_AbilityChainGraph::CreateInternalWidgets()
{

	//그래프 에디터 창
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_AbilityChain", "Ability Chain");

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FAssetEditor_AbilityChainGraph::OnSelectedNodesChanged);
	//InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FAssetEditor_AbilityChainGraph::OnNodeDoubleClicked);

	SAssignNew(GraphEditorWidget, SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(EditingGraph->EdGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);

	LOG_ERROR(TEXT("%s"), GraphEditorWidget.IsValid() ? *FString("trrrrrr") : *FString("Fgagf"));

	//노드 프로퍼티 디테일 창
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;

	NodePropertyWidget = PropertyModule.CreateDetailView(Args);
	NodePropertyWidget->SetObject(EditingGraph);
	NodePropertyWidget->OnFinishedChangingProperties().AddSP(this, &FAssetEditor_AbilityChainGraph::OnFinishedChangingProperties);
}

void FAssetEditor_AbilityChainGraph::CreateEdGraph()
{
	if(EditingGraph->EdGraph != nullptr) return;

	EditingGraph->EdGraph = CastChecked<UEdGraph_AbilityChain>(FBlueprintEditorUtils::CreateNewGraph(EditingGraph, NAME_None, EdGraphSubclass, EdGraphSchemaSubclass));

	EditingGraph->EdGraph->bAllowDeletion = false;

	// Give the schema a chance to fill out any required nodes (like the results node)
	const UEdGraphSchema* Schema = EditingGraph->EdGraph->GetSchema();
	Schema->CreateDefaultNodesForGraph(*EditingGraph->EdGraph);
}

TSharedRef<SDockTab> FAssetEditor_AbilityChainGraph::SpawnTab_GraphEditor(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FAbilityChainGraphEditorTabs::GraphEditorID);

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
				.Label(LOCTEXT("GraphEditorTab_Title", "GraphEditor"))
				[
					GraphEditorWidget.ToSharedRef()
				];
	
	return SpawnedTab;
}

void FAssetEditor_AbilityChainGraph::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelectedNodes)
{
	TArray<UObject*> SelectedNodeArray;
	SelectedNodeArray.Reserve(NewSelectedNodes.Num());
	for (UObject* SelectedNode : NewSelectedNodes)
	{
		SelectedNodeArray.Add(SelectedNode);
	}

	SelectedNodeArray.IsEmpty() ?	NodePropertyWidget->SetObject(EditingGraph) : 
									NodePropertyWidget->SetObjects(SelectedNodeArray);
}

TSharedRef<SDockTab> FAssetEditor_AbilityChainGraph::SpawnTab_NodePropertyDetail(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FAbilityChainGraphEditorTabs::NodePropertyID);

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(LOCTEXT("NodePropertyTab_Title", "NodeProperty"))
		[
			NodePropertyWidget.ToSharedRef()
		];

	return SpawnedTab;
}

void FAssetEditor_AbilityChainGraph::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (EditingGraph == nullptr)
		return;

	EditingGraph->EdGraph->GetSchema()->ForceVisualizationCacheClear();
}

#undef LOCTEXT_NAMESPACE

