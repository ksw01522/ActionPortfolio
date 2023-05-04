// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractionType/SNPCInteractWidget.h"
#include "Interaction/InteractionType/InteractionSystemComponent_NPC.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SBackgroundBlur.h"
#include "ActionPFPlayerController.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNPCInteractWidget::Construct(const FArguments& InArgs)
{
	if(InArgs._OwnerPlayer == nullptr) return;

	// 블러강도 0~100
	float BlurStrength = 20;

	TSharedPtr<SVerticalBox> InteractsBox;
	SAssignNew(InteractsBox, SVerticalBox);

	for (auto NPCInteract : InArgs._NPCInteracts)
	{
		if(NPCInteract == nullptr) continue;

		InteractsBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SNPCInteractButton)
			.NPCInteract(NPCInteract)
			.OwnerPlayer(InArgs._OwnerPlayer)
		];
		
	}


	ChildSlot
	[
		SNew(SBackgroundBlur)
		.bApplyAlphaToBlur(true)
		.BlurStrength(BlurStrength)
		[
			SNew(SConstraintCanvas)

			+ SConstraintCanvas::Slot().Anchors(FAnchors(0.5, 0.5)).AutoSize(true)
			[
				SNew(SBorder)
				[
					InteractsBox.ToSharedRef()
				]
			]
		]
		
		
		
	];
}


void SNPCInteractButton::Construct(const FArguments& InArgs)
{
	if (!IsValid( InArgs._NPCInteract) || !IsValid(InArgs._OwnerPlayer)) {
		ChildSlot[
			SNew(STextBlock)
			.Text(FText::FromString("Can't Make NPCInteract Button."))
		];
		return;
	}

	NPCInteract = InArgs._NPCInteract;
	OwnerPlayer = InArgs._OwnerPlayer;

	ChildSlot[
		SNew(SButton)
		.OnClicked(this, &SNPCInteractButton::PlayerInteractWithNPC)
		.Text(NPCInteract->GetNPCInteractionName())
		.ContentPadding(FMargin(5,5))
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
	];
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SNPCInteractButton::PlayerInteractWithNPC()
{
	if(IsValid(OwnerPlayer) && IsValid(NPCInteract) )	NPCInteract->NPCInteract(OwnerPlayer);
	return FReply::Handled();
}
