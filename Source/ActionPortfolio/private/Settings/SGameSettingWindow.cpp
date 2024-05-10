// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/SGameSettingWindow.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Types/NavigationMetaData.h"
#include "Widget/SInputKeyLabel.h"
#include "CustomInputSettingSubsystem.h"
#include "ActionPortfolio.h"

#define LOCTEXT_NAMESPACE "CustomGameSetting"

SLATE_IMPLEMENT_WIDGET(SGameSettingWindow);
void SGameSettingWindow::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{

}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGameSettingWindow::Construct(const FArguments& InArgs)
{}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGameSettingWindow::SetTextStyle(const FTextBlockStyle* InTextStyle)
{
	TextStyle = InTextStyle;

	for (TSharedPtr<SGameSettingNode> Node : SettingNodes)
	{
		Node->SetTextStyle(TextStyle);
	}
}

void SGameSettingWindow::SetFocusedFrame(const FSlateBrush* InBrush)
{
	if(FocusedFrameBrush == InBrush) return;

	FocusedFrameBrush = InBrush;

	for (TSharedPtr<SGameSettingNode> Node : SettingNodes)
	{
		Node->SetFocusedFrame(FocusedFrameBrush);
	}
}

void SGameSettingWindow::SetNameblockPadding(FMargin InPadding)
{
	if(NameBlockPadding == InPadding) return;

	NameBlockPadding = InPadding;

	for (TSharedPtr<SGameSettingNode> Node : SettingNodes)
	{
		Node->SetNameBlockPadding(NameBlockPadding);
	}
}

void SGameSettingWindow::SetDescTextStyle(const FTextBlockStyle* InStyle)
{
	DescTextStyle = InStyle;

	for (auto& TextBlock : DescTexts)
	{
		TextBlock->SetTextStyle(DescTextStyle);
	}
}

void SGameSettingWindow::SetDescKeyLabelSize(FVector2D InSize)
{
	if(DescKeyLabelSize == InSize) return;

	DescKeyLabelSize = InSize;

	for (auto& Label : DescKeyLabels)
	{
		Label->SetIconSize(DescKeyLabelSize);
	}
}

void SGameSettingWindow::RebuildNavigation()
{
	int Num = SettingNodes.Num();

	for (int i = 0; i < Num; i++)
	{
		TSharedPtr<FNavigationMetaData> TempNav = SettingNodes[i]->GetMetaData<FNavigationMetaData>();

		if (i - 1 < 0) 
		{
			TempNav->SetNavigationStop(EUINavigation::Up);
		}
		else
		{
			TempNav->SetNavigationExplicit(EUINavigation::Up, SettingNodes[i-1]);
		}

		if (Num <= i + 1)
		{
			TempNav->SetNavigationStop(EUINavigation::Down);
		}
		else
		{
			TempNav->SetNavigationExplicit(EUINavigation::Down, SettingNodes[i + 1]);
		}
	}
}

TSharedRef<SWidget> SGameSettingWindow::CreateDescriptionWidget()
{
	DescTexts.Empty();
	DescKeyLabels.Empty();

	TSharedPtr<SHorizontalBox> TempBox;
	SAssignNew(TempBox, SHorizontalBox)
	.Visibility(EVisibility::HitTestInvisible);

	TSharedPtr<STextBlock> TempText;
	TSharedPtr<SInputKeyLabel> TempKeyLabel;

	FKey KeyboardKey;
	FKey GamepadKey;

	UCustomInputSettingSubsystem* InputSystem = UCustomInputSettingSubsystem::GetInstance();

	//Move
	TempBox->AddSlot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	[
		SAssignNew(TempText, STextBlock)
		.Text(LOCTEXT("Move","이동 : "))
		.TextStyle(DescTextStyle)
	];
	DescTexts.Add(TempText);

	if (InputSystem != nullptr)
	{
		InputSystem->GetNavigationDirectionKey(EUINavigation::Up, KeyboardKey, GamepadKey);
	}

	TempBox->AddSlot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	[
		SAssignNew(TempKeyLabel, SInputKeyLabel)
		.KeyboardKey(KeyboardKey)
		.GamepadKey(GamepadKey)
		.IconSize(DescKeyLabelSize)
	];
	DescKeyLabels.Add(TempKeyLabel);

	if (InputSystem != nullptr)
	{
		InputSystem->GetNavigationDirectionKey(EUINavigation::Down, KeyboardKey, GamepadKey);
	}

	TempBox->AddSlot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	[
		SAssignNew(TempKeyLabel, SInputKeyLabel)
		.KeyboardKey(KeyboardKey)
		.GamepadKey(GamepadKey)
		.IconSize(DescKeyLabelSize)
	];
	DescKeyLabels.Add(TempKeyLabel);

	//옵션 전환
	TempBox->AddSlot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SAssignNew(TempText, STextBlock)
				.Text(LOCTEXT("Option Change", ", 변경 : "))
				.TextStyle(DescTextStyle)
		];
	DescTexts.Add(TempText);

	if (InputSystem != nullptr)
	{
		InputSystem->GetNavigationDirectionKey(EUINavigation::Left, KeyboardKey, GamepadKey);
	}

	TempBox->AddSlot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SAssignNew(TempKeyLabel, SInputKeyLabel)
				.KeyboardKey(KeyboardKey)
				.GamepadKey(GamepadKey)
				.IconSize(DescKeyLabelSize)
		];
	DescKeyLabels.Add(TempKeyLabel);

	if (InputSystem != nullptr)
	{
		InputSystem->GetNavigationDirectionKey(EUINavigation::Right, KeyboardKey, GamepadKey);
	}

	TempBox->AddSlot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SAssignNew(TempKeyLabel, SInputKeyLabel)
				.KeyboardKey(KeyboardKey)
				.GamepadKey(GamepadKey)
				.IconSize(DescKeyLabelSize)
		];
	DescKeyLabels.Add(TempKeyLabel);

	//Toggle 켜기/끄기
	TempBox->AddSlot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SAssignNew(TempText, STextBlock)
				.Text(LOCTEXT("Toggle OnOff", ", 켜기/끄기 : "))
				.TextStyle(DescTextStyle)
		];
	DescTexts.Add(TempText);

	if (InputSystem != nullptr)
	{
		InputSystem->GetNavigationActionKey(EUINavigationAction::Accept, KeyboardKey, GamepadKey);
	}

	TempBox->AddSlot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SAssignNew(TempKeyLabel, SInputKeyLabel)
				.KeyboardKey(KeyboardKey)
				.GamepadKey(GamepadKey)
				.IconSize(DescKeyLabelSize)
		];
	DescKeyLabels.Add(TempKeyLabel);

	return TempBox.ToSharedRef();
}

void SGameSettingWindow::SetArrowImage(const FSlateBrush* InImage)
{
	if(ArrowImageBrush == InImage) return;

	ArrowImageBrush = InImage;

	for (TSharedPtr<SGameSettingNode> Node : SettingNodes)
	{
		Node->SetArrowImage(ArrowImageBrush);
	}
}

void SGameSettingWindow::SetCheckedImage(const FSlateBrush* InImage)
{
	if(CheckedImageBrush == InImage) return;

	CheckedImageBrush = InImage;

	for (TSharedPtr<SGameSettingNode> Node : SettingNodes)
	{
		Node->SetCheckedImage(CheckedImageBrush);
	}
}

void SGameSettingWindow::SetUncheckedImage(const FSlateBrush* InImage)
{
	if (UncheckedImageBrush == InImage) return;

	UncheckedImageBrush = InImage;

	for (TSharedPtr<SGameSettingNode> Node : SettingNodes)
	{
		Node->SetUncheckedImage(UncheckedImageBrush);
	}
}

void SGameSettingWindow::CreateMultipleNode(const FText& NameText, const TArray<FText>& OptionNames, int SelectedIdx, const SGameSettingNode_Multiple::FOnChangeOption& Event)
{
	TSharedPtr<SGameSettingNode> Temp;

	ScrollBox->AddSlot()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SAssignNew(Temp, SGameSettingNode_Multiple)
		.TextStyle(TextStyle)
		.FocusedFrame(FocusedFrameBrush)
		.NameBlockPadding(NameBlockPadding)
		.ArrowImage(ArrowImageBrush)
		.NameText(NameText)
		.OptionNames(OptionNames)
		.SelectedIndex(SelectedIdx)
		.OnChangeOption(Event)
	];

	SettingNodes.Add(Temp);
}

void SGameSettingWindow::CreateToggleNode(const FText& NameText, bool CurrentState, const SGameSettingNode_Toggle::FOnChangeState& Event)
{
	TSharedPtr<SGameSettingNode> Temp;

	ScrollBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(Temp, SGameSettingNode_Toggle)
				.TextStyle(TextStyle)
				.FocusedFrame(FocusedFrameBrush)
				.NameBlockPadding(NameBlockPadding)
				.NameText(NameText)
				.CurrentState(CurrentState)
				.CheckedImage(CheckedImageBrush)
				.UncheckedImage(UncheckedImageBrush)
				.OnChangeState(Event)
		];

	SettingNodes.Add(Temp);
}

FReply SGameSettingWindow::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	if (!SettingNodes.IsEmpty())
	{
		for (auto& SettingNode : SettingNodes)
		{
			SettingNode->SetEnabled(true);
		}

		FSlateApplication::Get().SetUserFocus(InFocusEvent.GetUser(), SettingNodes[0]);
	}

	return FReply::Handled();
}

FReply SGameSettingWindow::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FSlateApplication& SlateApp = FSlateApplication::Get();
	
	EUINavigationAction NavAction = SlateApp.GetNavigationActionFromKey(InKeyEvent);
	if (NavAction == EUINavigationAction::Back)
	{
		for (auto& SettingNode : SettingNodes)
		{
			SettingNode->SetEnabled(false);
		}

		if(OnBackKeyDown.IsBound()) return OnBackKeyDown.Execute(MyGeometry, InKeyEvent);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void SGameSettingWindow::OnSelectedSettingNodeType(ESettingNodeType NodeType)
{
	if (NodeType == ESettingNodeType::Multiple)
	{
	
	}
	else if(NodeType == ESettingNodeType::Toggle)
	{
	}
}

#undef LOCTEXT_NAMESPACE