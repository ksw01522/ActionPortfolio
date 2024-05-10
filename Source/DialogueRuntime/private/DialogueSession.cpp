// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSession.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "DialogueManager.h"
#include "Nodes/DialogueNode_Basic.h"
#include "DialogueRuntime.h"

#define LOCTEXT_NAMESPACE "DialogueSession"

UDialogueSession::UDialogueSession()
{
	bEdgeEnabled = true;
	bHideNodeType = ECustomNodeTypeHidden::Hidden;

#if WITH_EDITORONLY_DATA
	PreviewLanguage = EDialogueLanguage::Korean;
	EdGraph = nullptr;
	bCanRenameNode = true;

#endif
}

#if WITH_EDITOR

void UDialogueSession::ClearGraph()
{
	for (int i = 0; i < AllNodes.Num(); ++i)
	{
		UDialogueNode* Node = AllNodes[i];
		if (Node)
		{
			Node->ParentNodes.Empty();
			Node->ChildrenNodes.Empty();
			Node->Edges.Empty();
		}
	}

	AllNodes.Empty();
}

#endif

UDialogueNode_Start* UDialogueSession::GetStartNode() const
{
	return StartNode;
}

FString UDialogueSession::GetDialoguerName(const UDialogueNode_Basic& InNode) const
{
	check(DialoguerNameTable != nullptr);
	
	UDialogueManager* Manager = UDialogueManager::GetManagerInstance();

	FString Context("Find Dialoguer Name");
	if (FDialogueLocalization* Local = DialoguerNameTable->FindRow<FDialogueLocalization>(InNode.GetDialoguerNameCode(), Context))
	{
		EDialogueLanguage Language = Manager->GetCurrentLanguage();
		return Local->GetStringByLanguage(Language);
	}
	
	LOG_ERROR(TEXT("Can't find Name By : %s"), *InNode.GetDialoguerNameCode().ToString());
	return FString("Can't Find");
}

FString UDialogueSession::GetDialogueString(const UDialogueNode_Basic& InNode) const
{
	check(DialogueStringTable != nullptr);

	UDialogueManager* Manager = UDialogueManager::GetManagerInstance();

	FString Context("Find Dialogue String");
	if (FDialogueLocalization* Local = DialogueStringTable->FindRow<FDialogueLocalization>(InNode.GetDialogueStringCode(), Context))
	{
		EDialogueLanguage Language = Manager->GetCurrentLanguage();
		return Local->GetStringByLanguage(Language);
	}

	LOG_ERROR(TEXT("Can't find Name By : %s"), *InNode.GetDialogueStringCode().ToString());
	return FString("Can't Find");
}

#if WITH_EDITOR
FString UDialogueSession::GetDialoguerName_InEditor(const UDialogueNode_Basic& InNode) const
{
	check(DialoguerNameTable != nullptr);

	UDialogueManager* Manager = UDialogueManager::GetManagerInstance();

	FString Context("Find Dialoguer Name");
	if (FDialogueLocalization* Local = DialoguerNameTable->FindRow<FDialogueLocalization>(InNode.GetDialoguerNameCode(), Context))
	{
		return Local->GetStringByLanguage(PreviewLanguage);
	}

	LOG_ERROR(TEXT("Can't find Name By : %s"), *InNode.GetDialoguerNameCode().ToString());
	return FString("Can't Find");
}


FString UDialogueSession::GetDialogueString_InEditor(const UDialogueNode_Basic& InNode) const
{
	check(DialogueStringTable != nullptr);

	UDialogueManager* Manager = UDialogueManager::GetManagerInstance();

	FString Context("Find Dialogue String");
	if (FDialogueLocalization* Local = DialogueStringTable->FindRow<FDialogueLocalization>(InNode.GetDialogueStringCode(), Context))
	{
		return Local->GetStringByLanguage(PreviewLanguage);
	}

	LOG_ERROR(TEXT("Can't find Name By : %s"), *InNode.GetDialogueStringCode().ToString());
	return FString("Can't Find");
}

FString UDialogueSession::GetDialoguerName_Original(const UDialogueNode_Basic& InNode) const
{
	check(DialoguerNameTable != nullptr);

	UDialogueManager* Manager = UDialogueManager::GetManagerInstance();

	FString Context("Find Dialoguer Name");
	if (FDialogueLocalization* Local = DialoguerNameTable->FindRow<FDialogueLocalization>(InNode.GetDialoguerNameCode(), Context))
	{
		return Local->GetStringByLanguage(EDialogueLanguage::Korean);
	}

	LOG_ERROR(TEXT("Can't find Name By : %s"), *InNode.GetDialoguerNameCode().ToString());
	return FString("Can't Find");
}

FString UDialogueSession::GetDialogueString_Original(const UDialogueNode_Basic& InNode) const
{
	check(DialogueStringTable != nullptr);

	UDialogueManager* Manager = UDialogueManager::GetManagerInstance();

	FString Context("Find Dialogue String");
	if (FDialogueLocalization* Local = DialogueStringTable->FindRow<FDialogueLocalization>(InNode.GetDialogueStringCode(), Context))
	{
		return Local->GetStringByLanguage(EDialogueLanguage::Korean);
	}

	LOG_ERROR(TEXT("Can't find Name By : %s"), *InNode.GetDialogueStringCode().ToString());
	return FString("Can't Find");
}

#endif



#if WITH_EDITOR
void UDialogueSession::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == "PreviewLanguage") { OnChaged_PreviewLanguage(); }
	else if (PropertyChangedEvent.GetPropertyName() == "DialogueTextStyleSet" || PropertyChangedEvent.GetPropertyName() == "DecoratorClasses") {
		for (auto Node : AllNodes)
		{
			Cast<UDialogueNode>(Node)->OnChangedDialogueTextStyle();
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UDialogueSession::UpdateDialogueTableByNode(UDialogueNode_Basic& InNode)
{
	check(DialoguerNameTable != nullptr && DialogueStringTable != nullptr);

	FString FindStringContext(TEXT("Find Dialogue String"));
	FName DialogueCode = InNode.GetDialogueStringCode();
	FString DialogueString = InNode.GetDialogueString_InEditor();
	FDialogueLocalization* FindedRow = DialogueStringTable->FindRow<FDialogueLocalization>(DialogueCode, FindStringContext);
	check(FindedRow);
	
	switch (PreviewLanguage)
	{
	case EDialogueLanguage::Korean:
		FindedRow->Korean = DialogueString;
		break;
	case EDialogueLanguage::English:
		FindedRow->English = DialogueString;
		break;
	default:
		break;
	}
}




void UDialogueSession::OnChaged_PreviewLanguage()
{
	for (auto Node : AllNodes)
	{
		Cast<UDialogueNode>(Node)->OnChanged_PreviewLanguage();
	}
}
#endif

#undef LOCTEXT_NAMESPACE


