// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SActionPFMainSlate.h"
#include "SlateOptMacros.h"

#include "Character/Player/ActionPFPlayerController.h"

#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/SBoxPanel.h"
#include "UI/DialogueSlate.h"
#include "WidgetStyle/ActionPortfolioWidgetStyle.h"

#define LOCTEXT_NAMESPACE "ActionPF Main Slate"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SActionPFMainSlate::Construct(const FArguments& InArgs)
{
	FSlateStyleSet* ActionPFSlateStyleSet = FActionPortfolioWidgetStyle::Get();

	const FButtonStyle* InteractBTNStyle = &ActionPFSlateStyleSet->GetWidgetStyle<FButtonStyle>(ActionPFStyle::ButtonStyle::Default);
	const FTextBlockStyle* DefaultTextStyle = &ActionPFSlateStyleSet->GetWidgetStyle<FTextBlockStyle>(DialogueStyle::TextStyle::Default);

	//Anchors 모음
	const FAnchors MainDialogueBoxAnchors(0.01f, 0.7f, 0.99f, 0.985f);
	const FAnchors AnswersVerticalBoxAnchors(0.3f, 0.45f, 0.7f, 0.45f); 
	const FAnchors NPCInteractBTNsAnchors(0.35f, 0.5f, 0.65f, 0.5f);
	
	//Text Size RelativePath
	const FString TextSizePath = "ActionPF.TextSize.";

	const FText NPCInteractExitBTNText = LOCTEXT("Exit NPC Interact", "그만두기");

	EVisibility TempVisibility = EVisibility::Collapsed;

	ChildSlot
	[
		SNew(SConstraintCanvas)
		.Visibility(EVisibility::SelfHitTestInvisible)
		//Main Dialogue Box 생성
		+ SConstraintCanvas::Slot()
		.Anchors(MainDialogueBoxAnchors)
			[
				SAssignNew(MainDialogueBox, SDialogueBox)
				.Visibility(TempVisibility)
			]
		//Answers Vertical Box 생성
		+ SConstraintCanvas::Slot()
		.Anchors(AnswersVerticalBoxAnchors)
		.Alignment(FVector2D(0.5f))
			[
				SAssignNew(AnswersVerticalBox, SVerticalBox)
				.Visibility(TempVisibility)
			]
		//NPCInteract Button Vertical Box 생성
		+ SConstraintCanvas::Slot()
		.Anchors(NPCInteractBTNsAnchors)
		.Alignment(FVector2D(0.5f))
			[
				SAssignNew(NPCInteractBTNBox, SVerticalBox)
				.Visibility(TempVisibility)
				+SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Fill)
					[
						SNew(SButton)
						.Visibility(EVisibility::Visible)
						.ButtonStyle(InteractBTNStyle)
						.TextStyle(DefaultTextStyle)
						.Text(NPCInteractExitBTNText)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.ContentPadding(FMargin(10,10))
					]
			]

	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION



#undef LOCTEXT_NAMESPACE