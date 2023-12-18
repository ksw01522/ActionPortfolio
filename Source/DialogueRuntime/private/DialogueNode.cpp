// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueNode.h"
#include "Events/DialogueEvent.h"
#include "DialogueSession.h"
#include "DialogueRuntime.h"
#include "DialogueStructs.h"
#include "DialogueManager.h"
#include "DialogueEdge.h"
#include "Engine/CompositeDataTable.h"

#define LOCTEXT_NAMESPACE "DialogueNode"

FString FDialogueLocalization::GetStringByLanguage(EDialogueLanguage Language) const
{
	FString ReturnString;

	switch (Language)
	{
	case EDialogueLanguage::Korean:
		ReturnString = Korean;
		break;

	case EDialogueLanguage::English:
		ReturnString = English;
		break;
	}

	return ReturnString;
}



UDialogueNode::UDialogueNode()
{
#if WITH_EDITORONLY_DATA
	ContextMenuName = LOCTEXT("Dialogue Node Title", "Dialogue Node");
#endif
	DialogueNodeType = EDialogueNodeType::None;
}



UDialogueSession* UDialogueNode::GetDialogueSession() const
{
	return DialogueSession;
}

void UDialogueNode::GetDialogueElementContainer(FDialogueElementContainer& OutElement) const
{
	OutElement.ContainerType = GetDialogueNodeType();
}

const UDialogueNode* UDialogueNode::GetNextDialogueNode(FNextDialogueNodeOptionalStruct* OptionalStruct) const
{
	const UDialogueNode* NextNode = nullptr;

	for (const auto* Child : ChildrenNodes)
	{
		const UDialogueEdge* Edge = GetEdge(Child);
		if (Edge->CanEnterNextNode()) {
			NextNode = Child;
			break;
		}
	}

	return NextNode;
}



const UDialogueEdge* UDialogueNode::GetEdge(const UDialogueNode* ChildNode) const
{
	if(!Edges.Contains(ChildNode)) return nullptr;

	return Edges[ChildNode];
}

bool UDialogueNode::IsLeafNode() const
{
	return ChildrenNodes.Num() == 0;
}
 

#if WITH_EDITOR

FText UDialogueNode::GetDescription_Implementation() const
{
	return FText::GetEmpty();
}

bool UDialogueNode::IsNameEditable() const
{
	return false;
}

FLinearColor UDialogueNode::GetBackgroundColor() const
{
	return BackgroundColor;
}

FText UDialogueNode::GetNodeTitle()
{
	return FText::GetEmpty();
}


bool UDialogueNode::CanCreateConnection(UDialogueNode* Other, FText& ErrorMessage)
{
	return true;
}

bool UDialogueNode::CanCreateConnectionTo(UDialogueNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage)
{
	if(!CanCreateConnection(Other, ErrorMessage)) return false;
	if (Other->GetDialogueNodeType() == EDialogueNodeType::Start) return false;

	if (ChildrenLimitType == ECustomNodeLimit::Limited && NumberOfChildrenNodes >= ChildrenLimit)
	{
		ErrorMessage = FText::FromString("Children limit exceeded");
		return false;
	}

	return true;
}

bool UDialogueNode::CanCreateConnectionFrom(UDialogueNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage)
{
	if (!CanCreateConnection(Other, ErrorMessage)) return false;
	if (Other->GetDialogueNodeType() == EDialogueNodeType::End) return false;

	if (ParentLimitType == ECustomNodeLimit::Limited && NumberOfParentNodes >= ParentLimit)
	{
		ErrorMessage = FText::FromString("Parent limit exceeded");
		return false;
	}

	return true;
}



#endif

void FDialogueElementContainer::Clear()
{
	ContainerType = EDialogueNodeType::None;
	Elements.Empty();
}


#undef LOCTEXT_NAMESPACE

