// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/PlayerDialogueMCComponent.h"
#include "UI/DialogueSlate.h"
#include "DialogueNode.h"
#include "ActionPortfolioInstance.h"
#include "ActionPortfolio.h"
#include "Interaction/InteractionType/SNPCInteractWidget.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Input/SButton.h"
#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "Nodes/DialogueNode_Question.h"

UPlayerDialogueMCComponent::UPlayerDialogueMCComponent()
{
	
}

void UPlayerDialogueMCComponent::BeginPlay()
{
	Super::BeginPlay();
	
	bNeedShowAnswerBox = false;

	int AnswerButtonsReserveCount = 4;

	AnswerButtons.Empty();
	AnswerButtons.Reserve(AnswerButtonsReserveCount);
}

void UPlayerDialogueMCComponent::OnSuccedEnterNextNode(FDialogueElementContainer& Container)
{
	ensure(DefaultDialogueBox.Pin().IsValid());

	AnswersBox.Pin()->SetVisibility(EVisibility::Collapsed);

	float TextAnimTime = UActionPortfolioInstance::GetActionPFInstance()->GetDialogueAnimTime();

	switch (Container.ContainerType)
	{
		case EDialogueNodeType::Question:
		SetAnswerButtonText(Container);
		bNeedShowAnswerBox = true;
		case EDialogueNodeType::Basic:
		#if WITH_EDITOR
			PFLOG(Warning,TEXT("Check Dialogue Element Count : %d"), Container.Elements.Num());
		#endif	
			DefaultDialogueBox.Pin()->SetDialogue(Container.Elements[0]);

			GetWorld()->GetTimerManager().SetTimer(TextAnimHandle, this, &UPlayerDialogueMCComponent::AnimText, TextAnimTime, true);

			bCanEnterNextNodeInMC = false;

			break;

		case EDialogueNodeType::End:
			TryExitDialogue();
			break;

		#if WITH_EDITOR
			case EDialogueNodeType::None:
			case EDialogueNodeType::Start:
			case EDialogueNodeType::Answer:
				FString NodeTypeName = StaticEnum<EDialogueNodeType>()->GetNameStringByIndex((int32)Container.ContainerType);

				PFLOG(Error, TEXT("Get invalidate Dialogue Node Type : %s"), *NodeTypeName);
			break;
		#endif
	}
}

void UPlayerDialogueMCComponent::OnEnterDialogue()
{
	DefaultDialogueBox.Pin()->SetVisibility(EVisibility::SelfHitTestInvisible);

	Super::OnEnterDialogue();
}

void UPlayerDialogueMCComponent::OnExitDialogue(bool bIsCancelled)
{
	SetMainDialogueBoxVisible(false);

	if (IsTextAnimating())
	{
		GetWorld()->GetTimerManager().ClearTimer(TextAnimHandle);
	}

	Super::OnExitDialogue(bIsCancelled);
}

///////////////////////// Bind Slate ////////////////////////////////

void UPlayerDialogueMCComponent::BindDialogueBox(const TSharedPtr<SDialogueBox>& NewDialogueBox)
{
	ensure(NewDialogueBox.IsValid() && !DefaultDialogueBox.IsValid());

	DefaultDialogueBox = NewDialogueBox;
	DefaultDialogueBox.Pin()->OnButtonDown.AddUObject(this, &UPlayerDialogueMCComponent::OnMouseButtonDownDialogueBox);
}

void UPlayerDialogueMCComponent::BindAnswerBox(const TSharedPtr<SVerticalBox>& NewAnswersBox)
{
	ensure(NewAnswersBox.IsValid() && !AnswersBox.IsValid());

	AnswersBox = NewAnswersBox;
}

void UPlayerDialogueMCComponent::SetMainDialogueBoxVisible(bool NewState)
{
	EVisibility NewVisibility = NewState ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed;

	DefaultDialogueBox.Pin()->SetVisibility(NewVisibility);
	if(!NewState) AnswersBox.Pin()->SetVisibility(NewVisibility);
}

//////////////////////////////////////////////////////////////////////

FReply UPlayerDialogueMCComponent::AnswerForQuestion(int Index)
{
	FNextDialogueNodeOptionalStruct_Question AnswerStrcut(Index);

	TryEnterNextNode(&AnswerStrcut);
	
	return FReply::Unhandled();
}

void UPlayerDialogueMCComponent::SetAnswerButtonText(const FDialogueElementContainer& Container)
{
	int ElementCount = Container.Elements.Num();
	CheckAnswerButtonCount(ElementCount - 1);

	HideAndShowAnswerButtons(ElementCount - 1);

	for (int i = 1; i < ElementCount; ++i)
	{
		AnswerButtons[i - 1].Pin()->SetAnswer(Container.Elements[i]);
	}
}

void UPlayerDialogueMCComponent::CheckAnswerButtonCount(int Target)
{
	ensure(AnswersBox.IsValid());
	if (Target <= AnswerButtons.Num()) return;

	//Answer ButtonStyle
	FSlateStyleSet* ActionPFSlateStyleSet = FActionPortfolioWidgetStyle::Get();
	FButtonStyle AnswerButtonStyle = ActionPFSlateStyleSet->GetWidgetStyle<FButtonStyle>(DialogueStyle::ButtonStyle::AnswerButton);

	//부족한 버튼 생성
	for (int i = AnswerButtons.Num(); i < Target; i++)
	{
		AnswerButtons.Add(
			SNew(SAnswerButton)
			.OnClicked_UObject(this, &UPlayerDialogueMCComponent::AnswerForQuestion, i - 1)
		);

		AnswersBox.Pin()->AddSlot()
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.Padding(0, 0, 0, 40)
			[
				AnswerButtons.Last().Pin().ToSharedRef()
			];
	}
}

void UPlayerDialogueMCComponent::HideAndShowAnswerButtons(int Target)
{
	//범위 내의 Button들 보이게하게
	for(int i = 0; i < Target; i++)
	{
		AnswerButtons[i].Pin()->SetVisibility(EVisibility::Visible);
	}

	//범위 밖의 Button들 숨기기
	for (int i = Target; Target < AnswerButtons.Num(); i++)
	{
		AnswerButtons[i].Pin()->SetVisibility(EVisibility::Collapsed);
	}
	
}

void UPlayerDialogueMCComponent::OnMouseButtonDownDialogueBox()
{
	if (IsTextAnimating()) 
	{
		ForceTextAnimComplete();
	}
	else
	{
		TryEnterNextNode();
	}
}

////////////// Text Animation

bool UPlayerDialogueMCComponent::IsTextAnimating() const
{
	ensure(DefaultDialogueBox.Pin().IsValid());

	return DefaultDialogueBox.Pin()->IsTextAnimating();
}

void UPlayerDialogueMCComponent::AnimText()
{
	ensure(DefaultDialogueBox.Pin().IsValid());

	DefaultDialogueBox.Pin()->AnimText();

	if (!IsTextAnimating()) {
		OnTextAnimCompleted();
	}
}

void UPlayerDialogueMCComponent::ForceTextAnimComplete()
{
	ensure(DefaultDialogueBox.Pin().IsValid());

	DefaultDialogueBox.Pin()->CompleteAnimText();
	OnTextAnimCompleted();
}

void UPlayerDialogueMCComponent::OnTextAnimCompleted()
{
	GetWorld()->GetTimerManager().ClearTimer(TextAnimHandle);

	if (bNeedShowAnswerBox)
	{
		AnswersBox.Pin()->SetVisibility(EVisibility::SelfHitTestInvisible);
	}
	else {
		bCanEnterNextNodeInMC = true;
	}
}