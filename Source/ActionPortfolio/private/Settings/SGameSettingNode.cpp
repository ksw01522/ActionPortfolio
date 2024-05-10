// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/SGameSettingNode.h"
#include "SlateOptMacros.h"
#include "Types/NavigationMetaData.h"
#include "ActionPortfolio.h"
#include "Settings/SGameSettingWindow.h"

SLATE_IMPLEMENT_WIDGET(SGameSettingNode);
void SGameSettingNode::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{

}

SGameSettingNode::SGameSettingNode()
{
	SetEnabled(false);
	SetCanTick(false);
}

void SGameSettingNode::Construct(const FArguments& InArgs)
{
	
}

void SGameSettingNode::SetTextStyle(const FTextBlockStyle* InTextStyle)
{
	NameTextBlock->SetTextStyle(InTextStyle);
}

void SGameSettingNode::SetFocusedFrame(const FSlateBrush* InBrush)
{
	FocusedFrame->SetImage(InBrush);
}

void SGameSettingNode::SetNameBlockPadding(FMargin InPadding)
{
	NameBlockSlot->SetPadding(InPadding);
}

FReply SGameSettingNode::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	FocusedFrame->SetVisibility(EVisibility::SelfHitTestInvisible);
	
	OwnerWindow->OnSelectedSettingNodeType(SettingType);

	return FReply::Handled();
}

void SGameSettingNode::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	FocusedFrame->SetVisibility(EVisibility::Collapsed);
}

void SGameSettingNode::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FSlateApplication::Get().SetUserFocus(MouseEvent.GetUserIndex(), AsShared());
}


SLATE_IMPLEMENT_WIDGET(SGameSettingNode_Multiple);
void SGameSettingNode_Multiple::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{

}

SGameSettingNode_Multiple::SGameSettingNode_Multiple()
{
	SettingType = ESettingNodeType::Multiple;
}

SGameSettingNode_Multiple::~SGameSettingNode_Multiple()
{
	
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGameSettingNode_Multiple::Construct(const FArguments& InArgs)
{
	OwnerWindow = InArgs._OwnerWindow;

	SetVisibility(EVisibility::Visible);

	OnChangeOption = InArgs._OnChangeOption;
	DisplayOptionNames = InArgs._OptionNames;
	SelectedOptionIndex = InArgs._SelectedIndex;
	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.Expose(NameBlockSlot)
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(InArgs._NameBlockPadding)
			.FillWidth(1)
			[
				SAssignNew(NameTextBlock, STextBlock)
				.Visibility(EVisibility::SelfHitTestInvisible)
				.TextStyle(InArgs._TextStyle)
				.Text(InArgs._NameText)
			]
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SAssignNew(LeftArrowImage, SImage)
				.FlipForRightToLeftFlowDirection(true)
				.Image(InArgs._ArrowImage)
				.Visibility(EVisibility::Visible)
				.OnMouseButtonDown(TDelegate<FReply(const FGeometry&, const FPointerEvent&)>::CreateSP(this, &SGameSettingNode_Multiple::OnClickLeftArrow))
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.FillWidth(0.8)
			[
				SAssignNew(OptionTextBlock, STextBlock)
				.TextStyle(InArgs._TextStyle)
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SAssignNew(RightArrowImage, SImage)
				.Image(InArgs._ArrowImage)
				.Visibility(EVisibility::Visible)
				.OnMouseButtonDown(TDelegate<FReply(const FGeometry&, const FPointerEvent&)>::CreateSP(this, &SGameSettingNode_Multiple::OnClickRightArrow))
			]
		]

		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(FocusedFrame, SImage)
			.Visibility(EVisibility::Collapsed)
			.Image(InArgs._FocusedFrame)
		]
	];
	
	if (DisplayOptionNames.IsValidIndex(SelectedOptionIndex)) { OptionTextBlock->SetText(DisplayOptionNames[SelectedOptionIndex]); }

	TSharedPtr<FNavigationMetaData> NaviMetaData = GetMetaData<FNavigationMetaData>();
	if (!NaviMetaData.IsValid())
	{
		NaviMetaData = MakeShared<FNavigationMetaData>();
		AddMetadata(NaviMetaData.ToSharedRef());
	}

	NaviMetaData->SetNavigationStop(EUINavigation::Left);
	NaviMetaData->SetNavigationStop(EUINavigation::Right);
	NaviMetaData->SetNavigationStop(EUINavigation::Previous);
	NaviMetaData->SetNavigationStop(EUINavigation::Next);
	NaviMetaData->SetNavigationStop(EUINavigation::Up);
	NaviMetaData->SetNavigationStop(EUINavigation::Down);

	LeftArrowImage->SetFlowDirectionPreference(EFlowDirectionPreference::RightToLeft);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGameSettingNode_Multiple::SetTextStyle(const FTextBlockStyle* InTextStyle)
{
	NameTextBlock->SetTextStyle(InTextStyle);
	OptionTextBlock->SetTextStyle(InTextStyle);
}



void SGameSettingNode_Multiple::SelectOption(int Index)
{
	Index = FMath::Clamp(Index, 0, DisplayOptionNames.Num() - 1);

	if(Index == SelectedOptionIndex) return;

	SelectedOptionIndex = Index;

	OnChangeOption.ExecuteIfBound(SelectedOptionIndex);
	
	OptionTextBlock->SetText(DisplayOptionNames[SelectedOptionIndex]);
}

void SGameSettingNode_Multiple::SetOnChangeOptionEvent(const FOnChangeOption& InEvent)
{
	OnChangeOption = InEvent;
}

FReply SGameSettingNode_Multiple::OnClickLeftArrow(const FGeometry& Geometry, const FPointerEvent& PointerEvent)
{
	if (IsEnabled())
	{
		SelectOption(SelectedOptionIndex - 1);
	}

	return FReply::Handled();
}

FReply SGameSettingNode_Multiple::OnClickRightArrow(const FGeometry& Geometry, const FPointerEvent& PointerEvent)
{
	if (IsEnabled())
	{
		SelectOption(SelectedOptionIndex + 1);
	}

	return FReply::Handled();
}

void SGameSettingNode_Multiple::SetArrowImage(const FSlateBrush* InImage)
{
	LeftArrowImage->SetImage(InImage);
	RightArrowImage->SetImage(InImage);
}



FReply SGameSettingNode_Multiple::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (!IsEnabled()) return FReply::Unhandled();

	EUINavigation NavDir = FSlateApplication::Get().GetNavigationDirectionFromKey(InKeyEvent);
	if (NavDir == EUINavigation::Up || NavDir == EUINavigation::Down)
	{
		const ENavigationGenesis Genesis = InKeyEvent.GetKey().IsGamepadKey() ? ENavigationGenesis::Controller : ENavigationGenesis::Keyboard;
		return FReply::Handled().SetNavigation(NavDir, Genesis);
	}

	if (NavDir == EUINavigation::Left)
	{
		SelectOption(SelectedOptionIndex - 1);
	}
	else if (NavDir == EUINavigation::Right)
	{
		SelectOption(SelectedOptionIndex + 1);
	}

	return FReply::Unhandled();
}



SLATE_IMPLEMENT_WIDGET(SGameSettingNode_Toggle);
void SGameSettingNode_Toggle::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{

}

SGameSettingNode_Toggle::SGameSettingNode_Toggle()
{
	SettingType = ESettingNodeType::Toggle;
}

SGameSettingNode_Toggle::~SGameSettingNode_Toggle()
{
	
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGameSettingNode_Toggle::Construct(const FArguments& InArgs)
{
	OwnerWindow = InArgs._OwnerWindow;

	SetVisibility(EVisibility::Visible);

	CurrentState = InArgs._CurrentState;
	OnChangeState = InArgs._OnChangeState;
	CheckedImage = InArgs._CheckedImage;
	UncheckedImage = InArgs._UncheckedImage;

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Expose(NameBlockSlot)
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(InArgs._NameBlockPadding)
			.FillWidth(1)
			[
				SAssignNew(NameTextBlock, STextBlock)
				.Visibility(EVisibility::SelfHitTestInvisible)
				.TextStyle(InArgs._TextStyle)
				.Text(InArgs._NameText)
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.FillWidth(0.8)
			[
				SAssignNew(CheckImage, SImage)
				.Visibility(EVisibility::Visible)
			]
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(FocusedFrame, SImage)
			.Visibility(EVisibility::Collapsed)
			.Image(InArgs._FocusedFrame)
		]
	];


	if (CurrentState)	{ CheckImage->SetImage(CheckedImage); }
	else				{ CheckImage->SetImage(UncheckedImage); }

	TSharedPtr<FNavigationMetaData> NaviMetaData = GetMetaData<FNavigationMetaData>();
	if (!NaviMetaData.IsValid())
	{
		NaviMetaData = MakeShared<FNavigationMetaData>();
		AddMetadata(NaviMetaData.ToSharedRef());
	}

	NaviMetaData->SetNavigationStop(EUINavigation::Left);
	NaviMetaData->SetNavigationStop(EUINavigation::Right);
	NaviMetaData->SetNavigationStop(EUINavigation::Previous);
	NaviMetaData->SetNavigationStop(EUINavigation::Next);
	NaviMetaData->SetNavigationStop(EUINavigation::Up);
	NaviMetaData->SetNavigationStop(EUINavigation::Down);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGameSettingNode_Toggle::SetState(bool InState)
{
	if(CurrentState == InState) return;

	CurrentState = InState;

	OnChangeState.ExecuteIfBound(CurrentState);
}

void SGameSettingNode_Toggle::SetOnChangeStateEvent(const FOnChangeState& InEvent)
{
	OnChangeState = InEvent;
}


void SGameSettingNode_Toggle::SetCheckedImage(const FSlateBrush* InImage)
{
	CheckedImage = InImage;
	if(CurrentState) CheckImage->SetImage(CheckedImage);
}

void SGameSettingNode_Toggle::SetUncheckedImage(const FSlateBrush* InImage)
{
	UncheckedImage = InImage;
	if (!CurrentState) CheckImage->SetImage(UncheckedImage);
}


FReply SGameSettingNode_Toggle::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (!IsEnabled()) return FReply::Unhandled();

	FSlateApplication& SlateApp = FSlateApplication::Get();

	EUINavigation NavDir = SlateApp.GetNavigationDirectionFromKey(InKeyEvent);
	if (NavDir == EUINavigation::Up || NavDir == EUINavigation::Down)
	{
		const ENavigationGenesis Genesis = InKeyEvent.GetKey().IsGamepadKey() ? ENavigationGenesis::Controller : ENavigationGenesis::Keyboard;
		return FReply::Handled().SetNavigation(NavDir, Genesis);
	}

	EUINavigationAction NavAction = SlateApp.GetNavigationActionFromKey(InKeyEvent);
	if (NavAction == EUINavigationAction::Accept)
	{
		SetState(!CurrentState);
		return FReply::Handled();
	}


	return FReply::Unhandled();
}
