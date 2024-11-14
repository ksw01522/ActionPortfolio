// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/Widget/DialogueBoxWidget.h"
#include "Dialogue/Slate/DialogueSlate.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"


void UDialogueBoxWidget::SetDialoguerName(const FString& NewName)
{
	NameBlock->SetText(FText::FromString(NewName));
}

void UDialogueBoxWidget::SetDialogueString(const FString& NewString)
{
	TargetText = NewString;
	
	if (0 < TextAnimationDelay)
	{
		CurrentText.Reset(TargetText.GetAllocatedSize());
		StringBlock->SetText(FText::GetEmpty());

		bTextAnimating = true;
	}
	else
	{
		CurrentText = TargetText;
		StringBlock->SetText(FText::FromString(NewString));

		bTextAnimating = false;
	}


}

void UDialogueBoxWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TickTextAnimation(InDeltaTime);
}

void UDialogueBoxWidget::TickTextAnimation(float InDeltaTime)
{
	if (!bTextAnimating) return;

	//업데이트 주기 확인
	RemainTextAnimationDelay -= InDeltaTime;
	if(0 < RemainTextAnimationDelay) return;

	RemainTextAnimationDelay = TextAnimationDelay;

	//다음 순서가 옳은지 확인
	int NextIdx = CurrentText.Len();
	if (!TargetText.IsValidIndex(NextIdx)) {
		CompleteTextAnimation(false);
		return;
	}

	//Decorator 확인
	if (TargetText[NextIdx] == '<') {
		while (true) {
			if (!TargetText.IsValidIndex(NextIdx)) break;

			CurrentText.AppendChar(TargetText[NextIdx]);
			NextIdx++;

			if (CurrentText[CurrentText.Len() - 1] == '>') {
				bIsDecorating = !bIsDecorating;
				break;
			}
		}
	}

	CurrentText.AppendChar(TargetText[NextIdx]);

	FString TempString = CurrentText;
	if (bIsDecorating) TempString.Append("</>");

	StringBlock->SetText(FText::FromString(TempString));

	if (CurrentText == TargetText) {
		CompleteTextAnimation(false);
	}
}

void UDialogueBoxWidget::CompleteTextAnimation(bool bForced)
{
	CurrentText = TargetText;

	StringBlock->SetText(FText::FromString(CurrentText));

	bTextAnimating = true;
	bIsDecorating = false;
}

void UDialogueBoxWidget::ForceCompleteTextAnimation()
{
	CompleteTextAnimation(true);
}

void UDialogueBoxWidget::ClearDialogueBox()
{
	NameBlock->SetText(FText::GetEmpty());
	StringBlock->SetText(FText::GetEmpty());

	bIsDecorating = false;
	TargetText.Empty();
	CurrentText.Empty();

	bTextAnimating = false;
}
