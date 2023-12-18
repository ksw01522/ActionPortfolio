// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class AActionPFPlayerController;
class SDialogueBox;
class SVerticalBox;

class ACTIONPORTFOLIO_API SActionPFMainSlate : public SCompoundWidget
{
	friend class AActionPFPlayerController;

public:
	SLATE_BEGIN_ARGS(SActionPFMainSlate)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SDialogueBox> MainDialogueBox;
	
	TSharedPtr<SVerticalBox> AnswersVerticalBox;
	
	TSharedPtr<SVerticalBox> NPCInteractBTNBox;
};
