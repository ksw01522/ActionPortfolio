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

#define LOCTEXT_NAMESPACE "NPCInteactWidget"

template< class ObjectType >
struct FSRichTextBlockDeferredDeletor : public FDeferredCleanupInterface
{
public:
	FSRichTextBlockDeferredDeletor(ObjectType* InInnerObjectToDelete)
		: InnerObjectToDelete(InInnerObjectToDelete)
	{
	}

	virtual ~FSRichTextBlockDeferredDeletor()
	{
		delete InnerObjectToDelete;
	}

private:
	ObjectType* InnerObjectToDelete;
};

template< class ObjectType >
FORCEINLINE TSharedPtr< ObjectType > MakeSRichTextBlockShareableDeferredCleanup(ObjectType* InObject)
{
	return MakeShareable(InObject, [](ObjectType* ObjectToDelete) { BeginCleanup(new FSRichTextBlockDeferredDeletor<ObjectType>(ObjectToDelete)); });
}



FReply SNPCInteractWidget::MouseButtonDownInDialogueBox(const FGeometry& Geometry, const FPointerEvent& PointerEvent)
{
	ensure(IsValid(OwnerPlayer));
	OwnerPlayer->OnMouseButtonDownInDialogueBox_NPC();
	return FReply::Unhandled();
}

FReply SNPCInteractWidget::ExitInteractNPC()
{
	ensure(IsValid(OwnerPlayer));
	OwnerPlayer->ExitInteractNPC();
	return FReply::Unhandled();
}

void SNPCInteractWidget::SetDialoguerName(const FString& NewName)
{
	NPCDialogueTextBox->SetDialoguerName(NewName);
}

void SNPCInteractWidget::SetDialogueText(const FString& NewText)
{
	NPCDialogueTextBox->SetDialogueText(NewText);
}

void SNPCInteractWidget::SetDialogueStyleSet(UDataTable* StyleSet)
{
	NPCDialogueTextBox->SetTextStyleSet(StyleSet);
}

void SNPCInteractWidget::SetDialogueDecorator(const TArray<TSubclassOf<class USRichTextBlockDecorator>>& NewDecoClasses)
{
	NPCDialogueTextBox->SetDecorators(NewDecoClasses);
}

bool SNPCInteractWidget::IsDialogueAnimating() const
{
	return NPCDialogueTextBox->IsDialogueAnimating();
}

void SNPCInteractWidget::SetShowInteractButton(bool bShow)
{
	if(bShow)	{ BTNBoxBorder->SetVisibility(EVisibility::Visible); }
	else		{ BTNBoxBorder->SetVisibility(EVisibility::Hidden); }
}



void SNPCInteractWidget::AnimDialogueText()
{
	NPCDialogueTextBox->AnimText();
}

void SNPCInteractWidget::ForceDialogueAnimComplete()
{
	NPCDialogueTextBox->CompleteAnimText();
}


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNPCInteractWidget::Construct(const FArguments& InArgs)
{
	ensure(IsValid(InArgs._OwnerPlayer) && IsValid(InArgs._NPCInteractionSystem));

	OwnerPlayer = InArgs._OwnerPlayer;
	NPCInteractionSystem = InArgs._NPCInteractionSystem;

	// 블러강도 0~100
	float BlurStrength = 20;

	SAssignNew(NPCInteractBTNBox, SVerticalBox);

	const TArray<UNPCInteract*>& NPCInteracts = NPCInteractionSystem->GetAbleNPCInteractions(OwnerPlayer);

	for (auto NPCInteract : NPCInteracts)
	{
		if(NPCInteract == nullptr) continue;

		NPCInteractBTNBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SNPCInteractButton)
			.NPCInteract(NPCInteract)
			.OwnerPlayer(InArgs._OwnerPlayer)
		];
		
	}

	NPCInteractBTNBox->AddSlot()
	.HAlign(HAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SNew(SButton)
		.Text(LOCTEXT("NPC Interact Exit BTN", "대화 끝내기"))
		.OnClicked(this, &SNPCInteractWidget::ExitInteractNPC)
	];

	ChildSlot
	[
		SNew(SBackgroundBlur)
		.bApplyAlphaToBlur(true)
		.BlurStrength(BlurStrength)
		[
			SNew(SConstraintCanvas)
			
			+SConstraintCanvas::Slot()
			.Anchors(FAnchors(0.01,0.7,0.99,0.985))
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.OnMouseButtonDown(this, &SNPCInteractWidget::MouseButtonDownInDialogueBox)
				.Padding(10)
				[
					SAssignNew(NPCDialogueTextBox, SNPCDialogueBox)
					.Visibility(EVisibility::HitTestInvisible)
				]
			]

			+ SConstraintCanvas::Slot()
			.Anchors(FAnchors(0.5))
			.AutoSize(true)
			[
				SAssignNew(BTNBoxBorder, SBorder)
				[
					NPCInteractBTNBox.ToSharedRef()
				]
			]
		]
	];

	SetShowInteractButton(false);

}

// Interact Butoon Construct
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
		.TextShapingMethod(ETextShapingMethod::FullShaping)
	];
}

// DialogueBox Construct
void SNPCDialogueBox::Construct(const FArguments& InArgs)
{
	FSlateStyleSet* NPCInteractSlateStyle = FActionPortfolioWidgetStyle::GetActionPFWidgetStyleSet();
	FSlateBrush* NewBrush = new FSlateBrush;

	NewBrush->TintColor = FLinearColor(1, 1, 1, 1);

	ChildSlot[
		SNew(SBorder)
		.BorderImage(NewBrush)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.FillHeight(0.2f)
			[
				SAssignNew(DialoguerNameBlock, STextBlock)
				.Font(NPCInteractSlateStyle->GetFontStyle("NPCInteract.DialoguerName.Default"))
			]

			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(0.8f)
			[
				SAssignNew(DialogueTextBlock, SRichTextBlock)
				.AutoWrapText(true)
			]
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SNPCInteractButton::PlayerInteractWithNPC()
{
	if(IsValid(OwnerPlayer) && IsValid(NPCInteract) )	NPCInteract->NPCInteract(OwnerPlayer);
	return FReply::Handled();
}




void SNPCDialogueBox::AnimText()
{
	if(!bIsAnimating) return;

	int NextIdx = CurrentText.Len();
	if (!TargetText.IsValidIndex(NextIdx)) {
		CompleteAnimText();
		return;
	}
	
	if (TargetText[NextIdx] == '<') {
		while (true) {
			if(!TargetText.IsValidIndex(NextIdx)) break;

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
	if(bIsDecorating) TempString.Append("</>");

	DialogueTextBlock->SetText(FText::FromString(TempString));

	if (CurrentText == TargetText) {
		CompleteAnimText();
	}
}

void SNPCDialogueBox::SetDialoguerName(const FString& NewName)
{
	DialoguerNameBlock->SetText(FText::FromString( NewName));
}



void SNPCDialogueBox::SetTextStyleSet(UDataTable* NewTextStyleSet)
{
	if(TextStyleSet == NewTextStyleSet) return;

	bNeedUpdateStyle = true;

	TextStyleSet = NewTextStyleSet;

	UpdateDialogueTextBlock();
}

void SNPCDialogueBox::SetDecorators(const TArray<TSubclassOf<class USRichTextBlockDecorator>>& NewDecoClasses)
{
	if(DecoClasses == NewDecoClasses) return;

	DecoClasses.Empty();
	bNeedUpdateDecos = true;

	DecoClasses = NewDecoClasses;

	UpdateDialogueTextBlock();
}


void SNPCDialogueBox::CompleteAnimText()
{
	DialogueTextBlock->SetText(FText::FromString(TargetText));
	bIsAnimating = false;
}

void SNPCDialogueBox::MakeStyleInstance()
{
	if(!bNeedUpdateStyle) return;

	StyleInstance.Reset();
	StyleInstance = MakeSRichTextBlockShareableDeferredCleanup(new FSlateStyleSet(TEXT("RichTextStyle")));

	if (TextStyleSet.IsValid() && TextStyleSet->GetRowStruct()->IsChildOf(FRichTextStyleRow::StaticStruct()))
	{
		for (const auto& Entry : TextStyleSet->GetRowMap())
		{
			FName SubStyleName = Entry.Key;
			FRichTextStyleRow* RichTextStyle = (FRichTextStyleRow*)Entry.Value;

			if (SubStyleName == FName(TEXT("Default")))
			{
				TextBlockStyle = RichTextStyle->TextStyle;
			}

			StyleInstance->Set(SubStyleName, RichTextStyle->TextStyle);
		}
	}

	bNeedUpdateStyle = false;
}


void SNPCDialogueBox::MakeDecoInstance(TArray<TSharedRef<class ITextDecorator>>& OutDecorators)
{
	if(!bNeedUpdateDecos) return;

	for (TSubclassOf<USRichTextBlockDecorator> DecoratorClass : DecoClasses)
	{
		if (UClass* ResolvedClass = DecoratorClass.Get())
		{
			if (!ResolvedClass->HasAnyClassFlags(CLASS_Abstract))
			{
				USRichTextBlockDecorator* Decorator = NewObject<USRichTextBlockDecorator>(nullptr, ResolvedClass);
				InstanceDecorators.Add(Decorator);
			}
		}
	}

	for (USRichTextBlockDecorator* Decorator : InstanceDecorators)
	{
		if (Decorator)
		{
			TSharedPtr<ITextDecorator> TextDecorator = Decorator->CreateDecorator(TextBlockStyle);
			if (TextDecorator.IsValid())
			{
				OutDecorators.Add(TextDecorator.ToSharedRef());
			}
		}
	}

	bNeedUpdateDecos = false;
}

void SNPCDialogueBox::UpdateDialogueTextBlock()
{
	if (bNeedUpdateStyle)
	{
		MakeStyleInstance();
		DialogueTextBlock->SetTextStyle(TextBlockStyle);
		DialogueTextBlock->SetDecoratorStyleSet(StyleInstance.Get());
	}

	if (bNeedUpdateDecos)
	{
		TArray<TSharedRef<ITextDecorator>> Decorators;
		MakeDecoInstance(Decorators);
		DialogueTextBlock->SetDecorators(Decorators);
	}
}

void SNPCDialogueBox::SetDialogueText(const FString& NewTarget)
{
	TargetText = NewTarget;
	CurrentText.Empty();
	DialogueTextBlock->SetText(FText::GetEmpty());
	bIsDecorating = false;
	bIsAnimating = true;
}


#undef LOCTEXT_NAMESPACE