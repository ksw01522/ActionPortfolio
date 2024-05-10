// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/DialogueNode_Question.h"

#define LOCTEXT_NAMESPACE "DialogueNode"

UDialogueNode_Question::UDialogueNode_Question()
{
#if WITH_EDITORONLY_DATA
	ContextMenuName = LOCTEXT("Dialogue Node Question Title", "Question");
	BackgroundColor = FLinearColor(0, 1, 0);
	ChildrenLimitType = ECustomNodeLimit::Unlimited;
#endif
	DialogueNodeType = EDialogueNodeType::Question;
}

#if WITH_EDITOR
bool UDialogueNode_Question::CanCreateConnectionTo(UDialogueNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage) const
{
	if (Other->GetDialogueNodeType() == EDialogueNodeType::Answer)
	{
		return true;
	}
	else
	{
		return false;
	}
}

FText UDialogueNode_Question::GetNodeTitle()
{
	return GetDialogueString_InEditor().IsEmpty() ? LOCTEXT("Empty Question Title", "Empty Question String") : FText::FromString(GetDialogueString_InEditor());
}
#endif

#undef LOCTEXT_NAMESPACE