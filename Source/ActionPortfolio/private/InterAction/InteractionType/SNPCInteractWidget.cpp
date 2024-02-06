// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractionType/SNPCInteractWidget.h"
#include "Interaction/InteractionType/InteractionSystemComponent_NPC.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SBackgroundBlur.h"
#include "Character/Player/ActionPFPlayerController.h"
#include "RenderingThread.h"
#include "Components/RichTextBlockDecorator.h"
#include "Styling/SlateStyle.h"
#include "Components/RichTextBlock.h"
#include "Components/RichTextBlockDecorator.h"
#include "Engine/DataTable.h"
#include "SRichTextBlockDecorator.h"
#include "ActionPortfolio.h"
#include "Styling/SlateStyleRegistry.h"
#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "Styling/SlateStyle.h"
#include "Widgets/Layout/SSpacer.h"
#include "UI/DialogueSlate.h"
#include "Widgets/Layout/SScaleBox.h"

#define LOCTEXT_NAMESPACE "NPCInteactWidget"



void SNPCInteractButton::Construct(const FArguments& InArgs)
{
	FSlateStyleSet* ActionPFSlateStyleSet = FActionPortfolioWidgetStyle::Get();

	const FButtonStyle* InteractBTNStyle = &ActionPFSlateStyleSet->GetWidgetStyle<FButtonStyle>(ActionPFStyle::ButtonStyle::Default);
	const FTextBlockStyle* DefaultTextStyle = &ActionPFSlateStyleSet->GetWidgetStyle<FTextBlockStyle>(DialogueStyle::TextStyle::Default);

	ChildSlot[
		SAssignNew(InteractButton, SButton)
		.ContentPadding(FMargin(10, 10))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.ButtonStyle(InteractBTNStyle)
			[
				SNew(SScaleBox)
				.Stretch(EStretch::ScaleToFit)
				.Visibility(EVisibility::SelfHitTestInvisible)
					[
						SAssignNew(InteractNameBlock, STextBlock)
						.TextStyle(DefaultTextStyle)
						.AutoWrapText(true)
						.Visibility(EVisibility::SelfHitTestInvisible)
					]
			]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNPCInteractButton::BindNPCInteract(AActionPFPlayerController* Player, UNPCInteract* Interact)
{
	ensureMsgf(Player != nullptr && Interact != nullptr, TEXT("Try Bind NPC Interact BTN By null Interact or Player"));

	InteractButton->SetOnClicked( TDelegate<FReply()>::CreateLambda(
									[TargetInteract = Interact, TargetPlayer = Player]() -> FReply
								{ TargetInteract->NPCInteract(TargetPlayer); return FReply::Unhandled(); }
								) );

	InteractNameBlock->SetText(Interact->GetNPCInteractionName());
}

#undef LOCTEXT_NAMESPACE


