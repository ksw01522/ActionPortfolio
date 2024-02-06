// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SCustomMapping.h"
#include "SlateOptMacros.h"
#include "CustomInputSettingSubsystem.h"
#include "CustomInputSettingRuntime.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Brushes/SlateImageBrush.h"
#include "CustomInputSettingSubsystem.h"
#include "Types/NavigationMetaData.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "PlayerMappableInputConfig.h"
#include "Widgets/Layout/SBackgroundBlur.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"

#define LOCTEXT_NAMESPACE "CustomInputSetting"

SLATE_IMPLEMENT_WIDGET(SCustomMappingSlate)
void SCustomMappingSlate::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{

}

SCustomMappingSlate::SCustomMappingSlate()
{
	bCanSupportFocus = false;

	SetCanTick(false);

	GuideText = LOCTEXT("Custom Input Guide Text", "키를 입력해 주십시오.");
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCustomMappingSlate::Construct(const FArguments& InArgs)
{
	const ETextJustify::Type TextJustification = ETextJustify::Center;
	TextColor = InArgs._TextColor;
	ErrorColor = InArgs._ErrorColor;

	ChildSlot
		[
			SNew(SBackgroundBlur)
				.Visibility(EVisibility::Visible)
				.bApplyAlphaToBlur(true)
				.BlurStrength(10)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SBox)
						.MinDesiredWidth(InArgs._BoxSize.X)
						.MinDesiredHeight(InArgs._BoxSize.Y)
						[
							SNew(SBorder)
								.Padding(InArgs._Padding)
								.BorderImage(InArgs._BorderImage)
								.BorderBackgroundColor(FLinearColor(1, 1, 1, 1))
								.ColorAndOpacity(FLinearColor(1, 1, 1, 1))
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								[
									SNew(SVerticalBox)
										.Visibility(EVisibility::HitTestInvisible)
										+ SVerticalBox::Slot()
										.HAlign(HAlign_Center)
										.VAlign(VAlign_Center)
										.FillHeight(6)
										[
											SAssignNew(NameTextBlock, STextBlock)
												.Justification(TextJustification)
												.TextStyle(InArgs._NameBlockStyle)
												.ColorAndOpacity(TextColor)

										]
										+ SVerticalBox::Slot()
										.HAlign(HAlign_Fill)
										.VAlign(VAlign_Fill)
										.FillHeight(4)
										[
											SAssignNew(GuideTextBlock, STextBlock)
												.Justification(TextJustification)
												.TextStyle(InArgs._GuideBlockStyle)
												.ColorAndOpacity(FLinearColor(1, 1, 1, 1))
										]
								]
						]
				]
		];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCustomMappingSlate::SetFont(const FSlateFontInfo& NewFont)
{
	NameTextBlock->SetFont(NewFont);
	GuideTextBlock->SetFont(NewFont);
}

void SCustomMappingSlate::SetAutoWrapText(bool NewState)
{
	NameTextBlock->SetAutoWrapText(NewState);
	GuideTextBlock->SetAutoWrapText(NewState);
}

void SCustomMappingSlate::SetWrapTextAt(float NewWrapTextAt)
{
	NameTextBlock->SetWrapTextAt(NewWrapTextAt);
	GuideTextBlock->SetWrapTextAt(NewWrapTextAt);
}

void SCustomMappingSlate::SetWrappingPolicy(ETextWrappingPolicy Policy)
{
	NameTextBlock->SetWrappingPolicy(Policy);
	GuideTextBlock->SetWrappingPolicy(Policy);
}


FReply SCustomMappingSlate::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingSlate::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{

	return FReply::Handled();
}

FReply SCustomMappingSlate::OnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& InAnalogInputEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingSlate::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingSlate::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingSlate::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingSlate::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingSlate::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingSlate::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingSlate::OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingSlate::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	return FReply::Handled();
}

void SCustomMappingSlate::ReadyToCustomMapping(const FText& DisplayName)
{
	NameTextBlock->SetText(DisplayName);

	GuideTextBlock->SetColorAndOpacity(TextColor);
	GuideTextBlock->SetText(GuideText);
	SetVisibility(EVisibility::Visible);
}

void SCustomMappingSlate::ShowErrorText(const FText& InText)
{
	GuideTextBlock->SetColorAndOpacity(ErrorColor);
	GuideTextBlock->SetText(InText);
}

#undef LOCTEXT_NAMESPACE