// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/PlayerDialogueMCComponent.h"
#include "Dialogue/Slate/DialogueSlate.h"
#include "DialogueNode.h"
#include "ActionPortfolioInstance.h"
#include "ActionPortfolio.h"
#include "Interaction/InteractionType/SNPCInteractWidget.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Input/SButton.h"
#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "Nodes/DialogueNode_Basic.h"
#include "Character/Player/ActionPFPlayerController.h"

UPlayerDialogueMCComponent::UPlayerDialogueMCComponent()
{
	
}

void UPlayerDialogueMCComponent::BeginPlay()
{
	Super::BeginPlay();

	SAssignNew(DefaultDialogueSlate, SDialogueMainSlate)
	.OnActNextDel_UObject(this, &UPlayerDialogueMCComponent::OnActNextDialogue)
	.OnDialogueAnimComplete_UObject(this, &UPlayerDialogueMCComponent::OnDialogueBoxAnimComplete);
}

void UPlayerDialogueMCComponent::OnSuccedEnterNextNode(const class UDialogueNode* InNode)
{
	EDialogueNodeType CurrentNodeType = InNode->GetDialogueNodeType();
	if (CurrentNodeType == EDialogueNodeType::Basic || CurrentNodeType == EDialogueNodeType::Question)
	{
		const UDialogueNode_Basic* BasicNode = Cast<UDialogueNode_Basic>(InNode);
		FString DialoguerName = BasicNode->GetDialoguerName();
		FString DialogueString = BasicNode->GetDialogueString();

		#if WITH_EDITOR
		PFLOG(Warning, TEXT("Name : %s,  String : %s"), *DialoguerName, *DialogueString);
		#endif

		const TArray<TSubclassOf<USRichTextBlockDecorator>>& Decos = BasicNode->GetSlateDecoClasses();

		DefaultDialogueSlate->GetDialogueBox()->SetDialogueBox(DialoguerName, DialogueString, Decos);
	}
	else if (CurrentNodeType == EDialogueNodeType::Answer)
	{
		DefaultDialogueSlate->ClearAnswerBox();

		TryEnterNextNode(InNode->GetNextDialogueNodes()[0]);
	}
	else if (CurrentNodeType == EDialogueNodeType::End)
	{
		TryExitDialogue(false);
	}
}

FReply UPlayerDialogueMCComponent::ClickAnswerButton(const UDialogueNode* InAnswerNode)
{
	TryEnterNextNode(InAnswerNode);

	return FReply::Handled();
}

void UPlayerDialogueMCComponent::OnEnterDialogue()
{
	GEngine->GameViewport->AddViewportWidgetContent(DefaultDialogueSlate.ToSharedRef(), 110);

	GetOwner<AActionPFPlayerController>()->AddCustomFocuseWidget(*DefaultDialogueSlate.Get());

	TryEnterNextNode(GetCurrentNode()->GetNextDialogueNodes()[0]);
}

void UPlayerDialogueMCComponent::OnExitDialogue(bool bIsCancelled)
{
	GetOwner<AActionPFPlayerController>()->RemoveCustomFocuseWidgetStack();

	GEngine->GameViewport->RemoveViewportWidgetContent(DefaultDialogueSlate.ToSharedRef());
}


//////////////////////////////////////////////////////////////////////


void UPlayerDialogueMCComponent::OnActNextDialogue()
{
	check(GetCurrentNode());
	const UDialogueNode* TempCurrentNode = GetCurrentNode();

	EDialogueNodeType CurrentNodeType = TempCurrentNode->GetDialogueNodeType();
	if(CurrentNodeType == EDialogueNodeType::Question) return;

	TryEnterNextNode(TempCurrentNode->GetNextDialogueNodes()[0]);
}

void UPlayerDialogueMCComponent::OnDialogueBoxAnimComplete(bool bForced)
{
	const UDialogueNode* TempCurrentNode = GetCurrentNode();
	if(TempCurrentNode == nullptr) return;

	EDialogueNodeType NodeType = TempCurrentNode->GetDialogueNodeType();
	if (NodeType == EDialogueNodeType::Question)
	{
		TArray<const UDialogueNode*> AnswerNodes = TempCurrentNode->GetNextDialogueNodes();
		for (auto& Node : AnswerNodes)
		{
			const UDialogueNode_Basic* AnswerNode = Cast<UDialogueNode_Basic>(Node);
			check(AnswerNode->GetDialogueNodeType() == EDialogueNodeType::Answer);

			const UDialogueNode* NextNode = AnswerNode->GetNextDialogueNodes()[0];
			
			DefaultDialogueSlate->CreateAnswerButton(AnswerNode->GetDialogueString(), FOnClicked::CreateUObject(this, &UPlayerDialogueMCComponent::ClickAnswerButton, NextNode));
		}
	}
}

