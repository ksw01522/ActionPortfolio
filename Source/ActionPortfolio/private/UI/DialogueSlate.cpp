// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueSlate.h"
#include "Character/Player/ActionPFPlayerController.h"
#include "DialogueNode.h"
#include "Settings/GameSettingSubsystem.h"
#include "ActionPortfolioInstance.h"

#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Input/SButton.h"
#include "SRichTextBlockDecorator.h"
#include "Components/RichTextBlock.h"
#include "Components/RichTextBlockDecorator.h"

#include "Widgets/Layout/SScaleBox.h"

#include "ActionPortfolio.h"

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



BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDialogueBox::Construct(const FArguments& InArgs)
{
	TextStyleSet = nullptr;
	DecoClasses.Empty();
	InstanceDecorators.Empty();

	FSlateStyleSet* ActionPFSlateStyleSet = FActionPortfolioWidgetStyle::Get();
	const FSlateBrush* NewBrush = ActionPFSlateStyleSet->GetBrush(DialogueStyle::BlockStyle::Default);

	const FTextBlockStyle* NameTextStyle = &ActionPFSlateStyleSet->GetWidgetStyle<FTextBlockStyle>(DialogueStyle::TextStyle::Default);

	ChildSlot[
		SNew(SBorder)
			.BorderImage(NewBrush)
			.OnMouseButtonDown(this, &SDialogueBox::MouseButtonDownInDialogueBox)
			[
				SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Center)
					.FillHeight(0.2f)
					.Padding(15, 4)
					[
						SAssignNew(DialoguerNameBlock, STextBlock)
							.TextStyle(NameTextStyle)
							.AutoWrapText(true)
					]

					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.FillHeight(0.8f)
					.Padding(15, 0)
					[
						SAssignNew(DialogueTextBlock, SRichTextBlock)
							.AutoWrapText(true)
					]
			]
	];

}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION



void SDialogueBox::MakeStyleInstance()
{
	StyleInstance.Reset();
	StyleInstance = MakeSRichTextBlockShareableDeferredCleanup(new FSlateStyleSet(TEXT("RichTextStyle")));

	if (TextStyleSet != nullptr && TextStyleSet->GetRowStruct()->IsChildOf(FRichTextStyleRow::StaticStruct()))
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
}

void SDialogueBox::MakeDecoInstance(TArray<TSharedRef<ITextDecorator>>& OutDecorators)
{
	for (USRichTextBlockDecorator* Decorator : InstanceDecorators)
	{
		Decorator->MarkAsGarbage();
	}
	InstanceDecorators.Empty();

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
}

void SDialogueBox::UpdateDialogueTextBlock()
{
	//StyleSet 업데이트
	MakeStyleInstance();
	DialogueTextBlock->SetTextStyle(TextBlockStyle);
	DialogueTextBlock->SetDecoratorStyleSet(StyleInstance.Get());

	//Decorator 업데이트
	TArray<TSharedRef<ITextDecorator>> Decorators;
	MakeDecoInstance(Decorators);
	DialogueTextBlock->SetDecorators(Decorators);
}

FReply SDialogueBox::MouseButtonDownInDialogueBox(const FGeometry& Geometry, const FPointerEvent& PointerEvent)
{
	if (OnButtonDown.IsBound())
	{
		OnButtonDown.Broadcast();
	}

	return FReply::Unhandled();
}


void SDialogueBox::SetDialogue(const FDialogueElement& DialogueElement)
{
	
	//NameBlock
	DialoguerNameBlock->SetVisibility(DialogueElement.Name.IsEmpty() ? EVisibility::Collapsed : EVisibility::HitTestInvisible);
	DialoguerNameBlock->SetText(FText::FromString(DialogueElement.Name));

	//String Block
	TargetText = DialogueElement.DialogueString;
	CurrentText.Empty();
	DialogueTextBlock->SetText(FText::GetEmpty());
	bIsDecorating = false;
	bIsAnimating = true;

	//RichText StyleSet이랑 Deco
	DecoClasses.Empty();
	TextStyleSet = DialogueElement.DialogueStyleSet;
	DecoClasses = DialogueElement.DialogueSlateDecorators;

	UpdateDialogueTextBlock();
}

void SDialogueBox::AnimText()
{
	int NextIdx = CurrentText.Len();
	if (!TargetText.IsValidIndex(NextIdx)) {
		CompleteAnimText();
		return;
	}

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

	DialogueTextBlock->SetText(FText::FromString(TempString));

	if (CurrentText == TargetText) {
		CompleteAnimText();
	}
}

void SDialogueBox::CompleteAnimText()
{
	bIsDecorating = false;
	CurrentText = TargetText;
	DialogueTextBlock->SetText(FText::FromString(CurrentText));
	bIsAnimating = false;
}

void SDialogueBox::ClearDialogueText()
{
	TargetText.Empty();
	CurrentText.Empty();
	DialogueTextBlock->SetText(FText::FromString(CurrentText));
	bIsAnimating = false;
	bIsDecorating = false;

	DialoguerNameBlock->SetText(FText::GetEmpty());
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAnswerButton::Construct(const FArguments& InArgs)
{
	FSlateStyleSet* ActionPFSlateStyleSet = FActionPortfolioWidgetStyle::Get();
	const FButtonStyle* AnswerBTNStyle = &ActionPFSlateStyleSet->GetWidgetStyle<FButtonStyle>(DialogueStyle::ButtonStyle::AnswerButton);
	const FTextBlockStyle* DefaultTextStyle = &ActionPFSlateStyleSet->GetWidgetStyle<FTextBlockStyle>(DialogueStyle::TextStyle::Default);

	SetCanTick(false);

	SButton::Construct(SButton::FArguments()
		.ButtonStyle(AnswerBTNStyle)
		.ContentPadding(FMargin(30, 10))
		.OnClicked(InArgs._OnClicked)
		.IsFocusable(true)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SAssignNew(AnswerTextBlock, STextBlock)
				.TextStyle(DefaultTextStyle)
				.AutoWrapText(true)
				.Visibility(EVisibility::SelfHitTestInvisible)
		]
	);
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAnswerButton::SetAnswer(const FDialogueElement& DialogueElement)
{
	FString AnswerString = DialogueElement.DialogueString;
	AnswerTextBlock->SetText(FText::FromString(AnswerString));
}