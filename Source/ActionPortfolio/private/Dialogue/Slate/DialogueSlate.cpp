// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/Slate/DialogueSlate.h"
#include "Character/Player/ActionPFPlayerController.h"
#include "DialogueNode.h"
#include "ActionPortfolioInstance.h"

#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Input/SButton.h"
#include "SRichTextBlockDecorator.h"
#include "Components/RichTextBlock.h"
#include "Components/RichTextBlockDecorator.h"

#include "Widgets/Layout/SScaleBox.h"

#include "ActionPortfolio.h"

#include "Widgets/Layout/SBackgroundBlur.h"
#include "Widgets/SOverlay.h"
#include "DialogueManager.h"


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

void SDialogueMainSlate::Construct(const FArguments& InArgs)
{
	const float BlurStr = 30;
	FSlateBrush* SceneCaptureBrush = nullptr;

	AnswerButtonPadding = InArgs._AnswerButtonPadding;

	OnActNextDel = InArgs._OnActNextDel;

	ChildSlot[
		SNew(SBackgroundBlur)
			.BlurStrength(BlurStr)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SOverlay)
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SImage)
							.Image(SceneCaptureBrush)
					]
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.FillHeight(6)
						[
							SAssignNew(AnswerBox, SVerticalBox)
							.Visibility(EVisibility::Hidden)
						]
						+ SVerticalBox::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.FillHeight(4)
						.Padding(10,5)
						[
							SAssignNew(DialogueBox, SDialogueBox)
							.Visibility(EVisibility::HitTestInvisible)
							.OnAnimComplete(InArgs._OnDialogueAnimComplete)
						]
					]
			]

	];


	TSharedPtr<FNavigationMetaData> NaviMetaData = GetMetaData<FNavigationMetaData>();
	if (!NaviMetaData.IsValid())
	{
		NaviMetaData = MakeShared<FNavigationMetaData>();
		AddMetadata(NaviMetaData.ToSharedRef());
	}

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SDialogueMainSlate::ClearAnswerBox()
{
	TSharedPtr<FNavigationMetaData> SlateNaviMetaData = GetMetaData<FNavigationMetaData>();

	for (EUINavigation UINavi : TEnumRange<EUINavigation>())
	{
		SlateNaviMetaData->SetNavigationStop(UINavi);
	}

	while (0 < AnswerBox->NumSlots())
	{
		FChildren* Children = AnswerBox->GetChildren();

		AnswerBox->RemoveSlot(Children->GetChildAt(0));
	}

	AnswerBox->SetVisibility(EVisibility::Hidden);
}


FReply SDialogueMainSlate::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	ActNextDialogue();

	return FReply::Handled();
}

FReply SDialogueMainSlate::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FSlateApplication& SlateApp = FSlateApplication::Get();

	EUINavigationAction NavAction = SlateApp.GetNavigationActionFromKey(InKeyEvent);

	if (NavAction == EUINavigationAction::Accept)
	{
		ActNextDialogue();

		return FReply::Handled();
	}

	return FReply::Handled();
}

void SDialogueMainSlate::SetOnActNextDelegate(const FSimpleDelegate& OnDel)
{
	OnActNextDel = OnDel;
}

void SDialogueMainSlate::ClearOnActNextDelegate()
{
	OnActNextDel.Unbind();
}

void SDialogueMainSlate::CreateAnswerButton(const FString InAnswerString, const FOnClicked& ClickMethod)
{
	FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();
	
	const FTextBlockStyle& TextStyle = StyleSet->GetWidgetStyle<FTextBlockStyle>(DialogueStyle::TextStyle::Default);
	const FButtonStyle& ButtonStyle = StyleSet->GetWidgetStyle<FButtonStyle>(DialogueStyle::ButtonStyle::AnswerButton);
	FVector2D ButtonSize = StyleSet->GetVector(ActionPFStyle::ButtonStyle::DefaultSize);

	TSharedRef<SButton> TempButton = 
		SNew(SButton)
		.Text(FText::FromString(InAnswerString))
		.TextStyle(&TextStyle)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.ButtonStyle(&ButtonStyle)
		.OnClicked(ClickMethod);

	TSharedPtr<FNavigationMetaData> ButtonNaviMetaData = TempButton->GetMetaData<FNavigationMetaData>();
	if (!ButtonNaviMetaData.IsValid())
	{
		ButtonNaviMetaData = MakeShared<FNavigationMetaData>();
		TempButton->AddMetadata(ButtonNaviMetaData.ToSharedRef());
	}

	for (EUINavigation UINavi : TEnumRange<EUINavigation>())
	{
		ButtonNaviMetaData->SetNavigationStop(UINavi);
	}

	if (AnswerBox->NumSlots() == 0)
	{
		TSharedPtr<FNavigationMetaData> SlateNaviMetaData = GetMetaData<FNavigationMetaData>();
		SlateNaviMetaData->SetNavigationExplicit(EUINavigation::Up, TempButton->AsShared());
		SlateNaviMetaData->SetNavigationExplicit(EUINavigation::Down, TempButton->AsShared());
		SlateNaviMetaData->SetNavigationExplicit(EUINavigation::Left, TempButton->AsShared());
		SlateNaviMetaData->SetNavigationExplicit(EUINavigation::Right, TempButton->AsShared());
	}
	else
	{
		FChildren* Child = AnswerBox->GetChildren();
		TSharedRef<SWidget> LastChild = Child->GetChildAt(Child->Num() - 1);

		ButtonNaviMetaData->SetNavigationExplicit(EUINavigation::Up, LastChild);

		TSharedPtr<FNavigationMetaData> LastChildNaviMetaData = LastChild->GetMetaData<FNavigationMetaData>();
		LastChildNaviMetaData->SetNavigationExplicit(EUINavigation::Down, TempButton->AsShared());
	}

	AnswerBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(AnswerButtonPadding)
		[
			SNew(SBox)
			.WidthOverride(ButtonSize.X)
			.HeightOverride(ButtonSize.Y)
			.Visibility(EVisibility::SelfHitTestInvisible)
			[
				TempButton->AsShared()
			]
		];

	AnswerBox->SetVisibility(EVisibility::SelfHitTestInvisible);
}

void SDialogueMainSlate::ActNextDialogue()
{
	if (DialogueBox->IsTextAnimating())
	{
		DialogueBox->CompleteAnimText(true);
	}
	else
	{
		OnActNextDel.ExecuteIfBound();
	}
}


SDialogueBox::SDialogueBox()
{
	SetCanTick(false);
}


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SDialogueBox::Construct(const FArguments& InArgs)
{
	UDialogueManager* Manager = UDialogueManager::GetManagerInstance();

	TextStyleSet = Manager->GetDialogueTextStyleSet();
	DecoClasses.Empty();
	InstanceDecorators.Empty();

	FSlateStyleSet* ActionPFSlateStyleSet = FActionPortfolioWidgetStyle::Get();
	const FSlateBrush* NewBrush = ActionPFSlateStyleSet->GetBrush(DialogueStyle::BlockStyle::Default);

	const FTextBlockStyle* NameTextStyle = &ActionPFSlateStyleSet->GetWidgetStyle<FTextBlockStyle>(DialogueStyle::TextStyle::Default);

	OnAnimComplete = InArgs._OnAnimComplete;

	ChildSlot[
		SNew(SBorder)
			.BorderImage(NewBrush)
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

	MakeStyleInstance();
	DialogueTextBlock->SetTextStyle(TextBlockStyle);
	DialogueTextBlock->SetDecoratorStyleSet(StyleInstance.Get());
}

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

END_SLATE_FUNCTION_BUILD_OPTIMIZATION



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


void SDialogueBox::SetDialoguerName(const FString& InName)
{
	DialoguerNameBlock->SetVisibility(InName.IsEmpty() ? EVisibility::Hidden : EVisibility::HitTestInvisible);
	DialoguerNameBlock->SetText(FText::FromString(InName));

}

void SDialogueBox::SetDialogueString(const FString& InString)
{
	TargetText = InString;
	CurrentText.Empty();
	DialogueTextBlock->SetText(FText::GetEmpty());
	bIsDecorating = false;

	CurrentTime = 0;

	SetCanTick(true);
}

void SDialogueBox::SetSlateDecorators(const TArray<TSubclassOf<class USRichTextBlockDecorator>>& Decos)
{
	DecoClasses.Empty();
	DecoClasses = Decos;

	TArray<TSharedRef<ITextDecorator>> Decorators;
	MakeDecoInstance(Decorators);
	DialogueTextBlock->SetDecorators(Decorators);
}

void SDialogueBox::SetDialogueBox(const FString& InName, const FString& InString, const TArray<TSubclassOf<class USRichTextBlockDecorator>>& Decos)
{
	SetDialoguerName(InName);
	SetSlateDecorators(Decos);
	SetDialogueString(InString);
}

void SDialogueBox::ClearDialogueBox()
{
	DialoguerNameBlock->SetText(FText::GetEmpty());
	DialogueTextBlock->SetText(FText::GetEmpty());

	bIsDecorating = false;
	TargetText.Empty();
	CurrentText.Empty();

	SetCanTick(false);
}




void SDialogueBox::CompleteAnimText(bool bForced)
{
	bIsDecorating = false;
	CurrentText = TargetText;
	DialogueTextBlock->SetText(FText::FromString(CurrentText));
	
	SetCanTick(false);

	OnAnimComplete.ExecuteIfBound(bForced);
}


void SDialogueBox::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	//애니메이션 딜레이 확인
	CurrentTime += InDeltaTime;
	if (CurrentTime < AnimatingDelay) { return;	}
	CurrentTime = 0;

	//다음 순서가 옳은지 확인
	int NextIdx = CurrentText.Len();
	if (!TargetText.IsValidIndex(NextIdx)) {
		CompleteAnimText(false);
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

	DialogueTextBlock->SetText(FText::FromString(TempString));

	if (CurrentText == TargetText) {
		CompleteAnimText(false);
	}
}