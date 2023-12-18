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
bool UDialogueNode_Question::CanCreateConnectionTo(UDialogueNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage)
{
	if (Cast<UDialogueNode_Answer>(Other))
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


void UDialogueNode_Question::GetDialogueElementContainer(FDialogueElementContainer& OutElement) const
{
	Super::GetDialogueElementContainer(OutElement);

	for (const auto& answer : ChildrenNodes)
	{
		answer->GetDialogueElementContainer(OutElement);
	}
}
	
const UDialogueNode* UDialogueNode_Question::GetNextDialogueNode(FNextDialogueNodeOptionalStruct* OptionalStruct) const
{
	FNextDialogueNodeOptionalStruct_Question* QuestionOptionalStruct = static_cast<FNextDialogueNodeOptionalStruct_Question*>(OptionalStruct);

	ensureMsgf(QuestionOptionalStruct != nullptr , TEXT("Call GetNextDialogueNode By nullptr Optional Strcut"));
	ensureMsgf(QuestionOptionalStruct->AnswerIndex < ChildrenNodes.Num(), TEXT("Try Get Range Out AnswerNode, AnswerNode Count : %d, Try Get Idx : %d."), ChildrenNodes.Num(), QuestionOptionalStruct->AnswerIndex);
	
	const UDialogueNode* NextDialogueNode = ChildrenNodes[QuestionOptionalStruct->AnswerIndex]->GetNextDialogueNode();

	return NextDialogueNode;
}


#undef LOCTEXT_NAMESPACE