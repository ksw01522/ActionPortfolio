// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/DialogueNode_Answer.h"
#include "Nodes/DialogueNode_Question.h"
#include "DialogueManager.h"

#define LOCTEXT_NAMESPACE "DialogueNode"

UDialogueNode_Answer::UDialogueNode_Answer()
{
#if WITH_EDITORONLY_DATA
	ContextMenuName = LOCTEXT("Dialogue Node Answer Title", "Answer");
	BackgroundColor = FLinearColor(0.25, 1, 0);
#endif

	DialogueNodeType = EDialogueNodeType::Answer;
}
#if WITH_EDITOR
bool UDialogueNode_Answer::CanCreateConnectionFrom(UDialogueNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage)
{
	if (Cast<UDialogueNode_Question>(Other))
	{
		return true;
	}
	else
	{
		return false;
	}
}

FText UDialogueNode_Answer::GetNodeTitle()
{
	return GetDialogueString_InEditor().IsEmpty() ? LOCTEXT("Empty Answer Title", "Empty Answer String") : FText::FromString(GetDialogueString_InEditor());
}
void UDialogueNode_Answer::GetDialogueElementContainer(FDialogueElementContainer& OutElement) const
{
	EDialogueLanguage Language = UDialogueManager::ManagerInstance->GetCurrentLanguage();

	FString ReturnName = GetDialoguerName(Language);
	FString ReturnDialogueString = GetDialogueString(Language);

	OutElement.Elements.Emplace(ReturnName, ReturnDialogueString, DialogueTextStyleSet, DialogueSlateDecoratorClasses, DialogueUMGDecoratorClasses);

}
#endif
#undef LOCTEXT_NAMESPACE


