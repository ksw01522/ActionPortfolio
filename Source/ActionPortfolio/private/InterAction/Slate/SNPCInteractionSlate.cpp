// Fill out your copyright notice in the Description page of Project Settings.


#include "InterAction/Slate/SNPCInteractionSlate.h"
#include "SlateOptMacros.h"
#include "ActionPortfolio.h"
#include "Types/NavigationMetaData.h"
#include "WidgetStyle/ActionPortfolioWidgetStyle.h"

SLATE_IMPLEMENT_WIDGET(SNPCInteractionSlate);
void SNPCInteractionSlate::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{

}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNPCInteractionSlate::Construct(const FArguments& InArgs)
{
	SetVisibility(EVisibility::Visible);

	ChildSlot
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SAssignNew(BTNBox, SVerticalBox)
	];
	
	TSharedPtr<FNavigationMetaData> NaviMetaData = GetMetaData<FNavigationMetaData>();
	if (!NaviMetaData.IsValid())
	{
		NaviMetaData = MakeShared<FNavigationMetaData>();
		AddMetadata(NaviMetaData.ToSharedRef());
	}

}



END_SLATE_FUNCTION_BUILD_OPTIMIZATION

SNPCInteractionSlate::~SNPCInteractionSlate()
{
}

FReply SNPCInteractionSlate::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
#if WITH_EDITOR
	PFLOG(Warning, TEXT("On FocusReceived NPCInteraction"));
#endif

	EFocusCause Cause = InFocusEvent.GetCause();

#if WITH_EDITOR
	UEnum* Enum = StaticEnum<EFocusCause>();
	PFLOG(Warning, TEXT("%s"), *Enum->GetDisplayNameTextByValue((int64)Cause).ToString());
#endif

	SetVisibility(EVisibility::Visible);

	return FReply::Handled();
}

void SNPCInteractionSlate::OnFocusLost(const FFocusEvent& InFocusEvent)
{

}

FReply SNPCInteractionSlate::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

FReply SNPCInteractionSlate::OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

void SNPCInteractionSlate::AddButton(const TSharedRef<SButton>& InButton)
{
	FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();
	FMargin ButtonPadding = StyleSet->GetMargin(InteractionStyle::ButtonStyle::Padding);

	TSharedPtr<FNavigationMetaData> ButtonNaviMetaData = InButton->GetMetaData<FNavigationMetaData>();
	if (!ButtonNaviMetaData.IsValid())
	{
		ButtonNaviMetaData = MakeShared<FNavigationMetaData>();
		InButton->AddMetadata(ButtonNaviMetaData.ToSharedRef());
	}

	for (EUINavigation UINavi : TEnumRange<EUINavigation>())
	{
		ButtonNaviMetaData->SetNavigationStop(UINavi);
	}

	if (BTNBox->NumSlots() == 0)
	{
		TSharedPtr<FNavigationMetaData> SlateNaviMetaData = GetMetaData<FNavigationMetaData>();
		SlateNaviMetaData->SetNavigationExplicit(EUINavigation::Up, InButton);
		SlateNaviMetaData->SetNavigationExplicit(EUINavigation::Down, InButton);
		SlateNaviMetaData->SetNavigationExplicit(EUINavigation::Left, InButton);
		SlateNaviMetaData->SetNavigationExplicit(EUINavigation::Right, InButton);
	}
	else
	{
		FChildren* Child = BTNBox->GetChildren();
		TSharedRef<SWidget> LastChild = Child->GetChildAt(Child->Num() - 1);

		ButtonNaviMetaData->SetNavigationExplicit(EUINavigation::Up, LastChild);

		TSharedPtr<FNavigationMetaData> LastChildNaviMetaData = LastChild->GetMetaData<FNavigationMetaData>();
		LastChildNaviMetaData->SetNavigationExplicit(EUINavigation::Down, InButton);
	}

	BTNBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(ButtonPadding)
			[
				InButton
			];
}

void SNPCInteractionSlate::ClearButtons()
{
	FChildren* BTNs = BTNBox->GetChildren();
	while (0 < BTNs->Num())
	{
		BTNBox->RemoveSlot(BTNs->GetChildAt(0));
	}

	TSharedPtr<FNavigationMetaData> NaviMetaData = GetMetaData<FNavigationMetaData>();
	for (EUINavigation UINavi : TEnumRange<EUINavigation>())
	{
		NaviMetaData->SetNavigationStop(UINavi);
	}
}
