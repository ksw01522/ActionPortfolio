// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/DialogueNode_Basic.h"
#include "DialogueRuntime.h"
#include "DialogueSession.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DialogueBFL.h"
#include "DialogueManager.h"

#if WITH_EDITOR
#include "DialogueDeveloperSettings.h"
#endif


#define LOCTEXT_NAMESPACE "DialogueNode"

UDialogueNode_Basic::UDialogueNode_Basic()
{
#if WITH_EDITORONLY_DATA
	ContextMenuName = LOCTEXT("Dialogue Node Basic Title", "Basic Dialogue");
	ChildrenLimitType = ECustomNodeLimit::Limited;
	ChildrenLimit = 1;

	bSlateDeco = true;
#endif
	
	DialogueNodeType = EDialogueNodeType::Basic;
	DialogueUMGDecoratorClasses.Empty();
	DialogueSlateDecoratorClasses.Empty();
	DialoguerAnimations.Empty();
}


const TArray<TSubclassOf<URichTextBlockDecorator>>& UDialogueNode_Basic::GetUMGDecoClasses() const
{
	return DialogueUMGDecoratorClasses;
}

const TArray<TSubclassOf<USRichTextBlockDecorator>>& UDialogueNode_Basic::GetSlateDecoClasses() const
{
	return DialogueSlateDecoratorClasses;
}

FString UDialogueNode_Basic::GetDialoguerName() const
{
	FString ReturnName = DialogueSession->GetDialoguerName(*this);

	return ReturnName;
}

FString UDialogueNode_Basic::GetDialogueString() const
{
	FString ReturnString = DialogueSession->GetDialogueString(*this);

	return ReturnString;
}


#if WITH_EDITOR
FString UDialogueNode_Basic::GetDialoguerName_InEditor() const
{
	return DialoguerName;
}


FString UDialogueNode_Basic::GetDialogueString_InEditor() const
{
	return DialogueString;
}


FString UDialogueNode_Basic::GetDialoguerName_Original_InEditor() const
{
	if (DialoguerNameCode.IsNone()) {
		return FString("Narration");
	}
	return Original_DialoguerName;
}

FString UDialogueNode_Basic::GetDialogueString_Original_InEditor() const
{
	return Original_DialogueString;
}

void UDialogueNode_Basic::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if		(PropertyChangedEvent.GetPropertyName() == "DialoguerNameCode")		{ OnChanged_DialoguerNameCode(); }
	else if (PropertyChangedEvent.GetPropertyName() == "DialogueStringCode")	{ OnChanged_DialogueStringCode(); }
	else if (PropertyChangedEvent.GetPropertyName() == "DialogueTextStyleSet" || PropertyChangedEvent.GetPropertyName() == "DialogueDecoratorClasses") { OnChangedDialogueTextStyle(); }

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UDialogueNode_Basic::ImportDialogue()
{
	check(DialogueSession);

	DialogueSession->UpdateDialogueTableByNode(*this);
}

void UDialogueNode_Basic::RebuildNode()
{
	OnChanged_DialoguerNameCode();
	OnChanged_DialogueStringCode();
}


void UDialogueNode_Basic::OnChanged_DialoguerNameCode()
{
	check(DialogueSession);

	Original_DialoguerName = DialogueSession->GetDialoguerName_Original(*this);
	DialoguerName = DialogueSession->GetDialoguerName_InEditor(*this);
}

void UDialogueNode_Basic::OnChanged_DialogueStringCode()
{
	check(DialogueSession);

	Original_DialogueString = DialogueSession->GetDialogueString_InEditor(*this);
	DialogueString = DialogueSession->GetDialoguerName_Original(*this);
}

void UDialogueNode_Basic::OnChanged_PreviewLanguage()
{
	check(DialogueSession);

	OnChanged_DialoguerNameCode();
	OnChanged_DialogueStringCode();
}

#endif

#undef LOCTEXT_NAMESPACE

