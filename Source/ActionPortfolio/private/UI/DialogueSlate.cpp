// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueSlate.h"
#include "Interaction/InteractionType/SNPCInteractWidget.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Character/Player/ActionPFPlayerController.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDialogueSlate::Construct(const FArguments& InArgs)
{
	OwnerPlayer = InArgs._OwnerPlayer;
	FSlateBrush* NewBrush = new FSlateBrush;

	NewBrush->TintColor = FLinearColor(1,1,1,1);

	ChildSlot
		[
			
			SNew(SConstraintCanvas)

			+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.01, 0.7, 0.99, 0.985))
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill)
			.BorderImage(NewBrush)
		.VAlign(VAlign_Fill)
		.OnMouseButtonDown(this, &SDialogueSlate::MouseButtonDownInDialogueBox)
		.Padding(10)
		[
			SAssignNew(DialogueBox, SNPCDialogueBox)
			.Visibility(EVisibility::HitTestInvisible)
		]
		]
		];

}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SDialogueSlate::MouseButtonDownInDialogueBox(const FGeometry& Geometry, const FPointerEvent& PointerEvent)
{
	ensure(OwnerPlayer.IsValid());
	OwnerPlayer->OnMouseButtonDownInDialogueBox();

	return FReply::Unhandled();
}

void SDialogueSlate::SetDialoguerName(const FString& NewName)
{
	DialogueBox->SetDialoguerName(NewName);
}

void SDialogueSlate::SetDialogueText(const FString& NewText)
{
	DialogueBox->SetDialogueText(NewText);
}

void SDialogueSlate::SetDialogueStyleSet(UDataTable* StyleSet)
{
	DialogueBox->SetTextStyleSet(StyleSet);
}

void SDialogueSlate::SetDialogueDecorator(const TArray<TSubclassOf<class USRichTextBlockDecorator>>& NewDecoClasses)
{
	DialogueBox->SetDecorators(NewDecoClasses);
}

bool SDialogueSlate::IsDialogueAnimating() const
{
	return DialogueBox->IsDialogueAnimating();
}

void SDialogueSlate::AnimDialogueText()
{
	DialogueBox->AnimText();
}

void SDialogueSlate::ForceDialogueAnimComplete()
{
	DialogueBox->CompleteAnimText();
}
