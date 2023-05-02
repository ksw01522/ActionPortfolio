// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeAssetEditor/SEdNode_SkillTreeEdge.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "Widgets/SToolTip.h"
#include "SGraphPanel.h"
#include "EdGraphSchema_K2.h"
#include "SkillTreeAssetEditor/EdGraphNode_SkillNode.h"
#include "SkillTreeAssetEditor/EdGraphNode_SkillTreeEdge.h"
#include "SkillTreeAssetEditor/ConnectionDrawingPolicy_SkillTree.h"

#define LOCTEXT_NAMESPACE "SSkillTreeEdge"

void SEdNode_SkillTreeEdge::Construct(const FArguments& InArgs, UEdGraphNode_SkillTreeEdge* InNode)
{
	this->GraphNode = InNode;
	this->UpdateGraphNode();
}

bool SEdNode_SkillTreeEdge::RequiresSecondPassLayout() const
{
	return true;
}

void SEdNode_SkillTreeEdge::PerformSecondPassLayout(const TMap<UObject*, TSharedRef<SNode>>& NodeToWidgetLookup) const
{
	UEdGraphNode_SkillTreeEdge* EdgeNode = CastChecked<UEdGraphNode_SkillTreeEdge>(GraphNode);

	FGeometry StartGeom;
	FGeometry EndGeom;

	UEdGraphNode_SkillNode* Start = EdgeNode->GetStartNode();
	UEdGraphNode_SkillNode* End = EdgeNode->GetEndNode();
	if (Start != nullptr && End != nullptr)
	{
		const TSharedRef<SNode>* pFromWidget = NodeToWidgetLookup.Find(Start);
		const TSharedRef<SNode>* pToWidget = NodeToWidgetLookup.Find(End);
		if (pFromWidget != nullptr && pToWidget != nullptr)
		{
			const TSharedRef<SNode>& FromWidget = *pFromWidget;
			const TSharedRef<SNode>& ToWidget = *pToWidget;

			StartGeom = FGeometry(FVector2D(Start->NodePosX, Start->NodePosY), FVector2D::ZeroVector, FromWidget->GetDesiredSize(), 1.0f);
			EndGeom = FGeometry(FVector2D(End->NodePosX, End->NodePosY), FVector2D::ZeroVector, ToWidget->GetDesiredSize(), 1.0f);
		}
	}

	PositionBetweenTwoNodesWithOffset(StartGeom, EndGeom, 0, 1);
}

void SEdNode_SkillTreeEdge::OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo)
{
	SGraphNode::OnNameTextCommited(InText, CommitInfo);

	UEdGraphNode_SkillTreeEdge* MyNode = CastChecked<UEdGraphNode_SkillTreeEdge>(GraphNode);

	if (MyNode != nullptr && MyNode->SkillTreeEdge != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("GenericGraphEditorRenameEdge", "Generic Graph Editor: Rename Edge"));
		MyNode->Modify();
		//->SkillTreeEdge->SetNodeTitle(InText);
		UpdateGraphNode();
	}
}

void SEdNode_SkillTreeEdge::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		[
			SNew(SImage)
			.Image(FAppStyle::GetBrush("Graph.TransitionNode.ColorSpill"))
		.ColorAndOpacity(this, &SEdNode_SkillTreeEdge::GetEdgeColor)
		]
	+ SOverlay::Slot()
		[
			SNew(SImage)
			.Image(this, &SEdNode_SkillTreeEdge::GetEdgeImage)
		.Visibility(this, &SEdNode_SkillTreeEdge::GetEdgeImageVisibility)
		]

	+ SOverlay::Slot()
		.Padding(FMargin(4.0f, 4.0f, 4.0f, 4.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.AutoHeight()
		[
			SAssignNew(InlineEditableText, SInlineEditableTextBlock)
			.ColorAndOpacity(FLinearColor::Black)
		.Visibility(this, &SEdNode_SkillTreeEdge::GetEdgeTitleVisbility)
		.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
		.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
		.OnTextCommitted(this, &SEdNode_SkillTreeEdge::OnNameTextCommited)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			NodeTitle.ToSharedRef()
		]

		]
		];
}

void SEdNode_SkillTreeEdge::PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const
{
	// Get a reasonable seed point (halfway between the boxes)
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;

	// Find the (approximate) closest points between the two boxes
	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	// Position ourselves halfway along the connecting line between the nodes, elevated away perpendicular to the direction of the line
	const float Height = 30.0f;

	const FVector2D DesiredNodeSize = GetDesiredSize();

	FVector2D DeltaPos(EndAnchorPoint - StartAnchorPoint);

	if (DeltaPos.IsNearlyZero())
	{
		DeltaPos = FVector2D(10.0f, 0.0f);
	}

	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();

	const FVector2D NewCenter = StartAnchorPoint + (0.5f * DeltaPos) + (Height * Normal);

	FVector2D DeltaNormal = DeltaPos.GetSafeNormal();

	// Calculate node offset in the case of multiple transitions between the same two nodes
	// MultiNodeOffset: the offset where 0 is the centre of the transition, -1 is 1 <size of node>
	// towards the PrevStateNode and +1 is 1 <size of node> towards the NextStateNode.

	const float MutliNodeSpace = 0.2f; // Space between multiple transition nodes (in units of <size of node> )
	const float MultiNodeStep = (1.f + MutliNodeSpace); //Step between node centres (Size of node + size of node spacer)

	const float MultiNodeStart = -((MaxNodes - 1) * MultiNodeStep) / 2.f;
	const float MultiNodeOffset = MultiNodeStart + (NodeIndex * MultiNodeStep);

	// Now we need to adjust the new center by the node size, zoom factor and multi node offset
	const FVector2D NewCorner = NewCenter - (0.5f * DesiredNodeSize) + (DeltaNormal * MultiNodeOffset * DesiredNodeSize.Size());

	GraphNode->NodePosX = NewCorner.X;
	GraphNode->NodePosY = NewCorner.Y;
}

FSlateColor SEdNode_SkillTreeEdge::GetEdgeColor() const
{
	UEdGraphNode_SkillTreeEdge* EdgeNode = CastChecked<UEdGraphNode_SkillTreeEdge>(GraphNode);
	if (EdgeNode != nullptr && EdgeNode->SkillTreeEdge != nullptr)
	{
		return EdgeNode->SkillTreeEdge->GetEdgeColor();
	}
	return FLinearColor(0.9f, 0.9f, 0.9f, 1.0f);
}

const FSlateBrush* SEdNode_SkillTreeEdge::GetEdgeImage() const
{
	return FAppStyle::GetBrush("Graph.TransitionNode.Icon");
}

EVisibility SEdNode_SkillTreeEdge::GetEdgeImageVisibility() const
{
	UEdGraphNode_SkillTreeEdge* EdgeNode = CastChecked<UEdGraphNode_SkillTreeEdge>(GraphNode);
	if (EdgeNode && EdgeNode->SkillTreeEdge && EdgeNode->SkillTreeEdge->bShouldDrawTitle)
		return EVisibility::Hidden;

	return EVisibility::Visible;
}

EVisibility SEdNode_SkillTreeEdge::GetEdgeTitleVisbility() const
{
	UEdGraphNode_SkillTreeEdge* EdgeNode = CastChecked<UEdGraphNode_SkillTreeEdge>(GraphNode);
	if (EdgeNode && EdgeNode->SkillTreeEdge && EdgeNode->SkillTreeEdge->bShouldDrawTitle)
		return EVisibility::Visible;

	return EVisibility::Collapsed;
}

#undef LOCTEXT_NAMESPACE