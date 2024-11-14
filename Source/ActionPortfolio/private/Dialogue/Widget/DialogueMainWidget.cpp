// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/Widget/DialogueMainWidget.h"
#include "Dialogue/Slate/DialogueSlate.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Components/VerticalBox.h"


FReply UDialogueMainWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{


	return FReply::Unhandled();
}

FReply UDialogueMainWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FSlateApplication& SlateApp = FSlateApplication::Get();

	EUINavigationAction Action = SlateApp.GetNavigationActionFromKey(InKeyEvent);
	if (Action == EUINavigationAction::Accept)
	{

	}
	else if(Action == EUINavigationAction::Back)
	{
		
	}

	EUINavigation NaviDir = SlateApp.GetNavigationDirectionFromKey(InKeyEvent);
	if (EUINavigation::Left <= NaviDir && NaviDir <= EUINavigation::Previous && 0 < AnswerPanel->GetChildrenCount())
	{
		
	}


	return FReply::Handled();
}
