// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SCustomMappingReset.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SBackgroundBlur.h"
#include "Widgets/Layout/SBox.h"
#include "Widget/SInputKeyLabel.h"
#include "CustomInputSettingSubsystem.h"

#define LOCTEXT_NAMESPACE "CustomInputSetting"

SLATE_IMPLEMENT_WIDGET(SCustomMappingReset)
void SCustomMappingReset::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{

}

SCustomMappingReset::SCustomMappingReset()
{
	bCanSupportFocus = false;
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCustomMappingReset::Construct(const FArguments& InArgs)
{
	UCustomInputSettingSubsystem* Subsystem = UCustomInputSettingSubsystem::GetInstance();

	FKey AcceptKeyboard;
	FKey AcceptGamepad;

	FKey BackKeyboard;
	FKey BackGamepad;

#if WITH_EDITOR
	
	if (Subsystem != nullptr)
	{
		Subsystem->GetNavigationActionKey(EUINavigationAction::Accept, AcceptKeyboard, AcceptGamepad);
		Subsystem->GetNavigationActionKey(EUINavigationAction::Back, BackKeyboard, BackGamepad);
	}

#else
	Subsystem->GetNavigationActionKey(EUINavigationAction::Accept, AcceptKeyboard, AcceptGamepad);
	Subsystem->GetNavigationActionKey(EUINavigationAction::Back, BackKeyboard, BackGamepad);
#endif


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
									+SVerticalBox::Slot()
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									.FillHeight(6)
									[
										SNew(STextBlock)
										.Visibility(EVisibility::SelfHitTestInvisible)
										.Justification(ETextJustify::Center)
										.TextStyle(InArgs._GuideBlockStyle)
										.Text(LOCTEXT("Reset Custom Mapping Guide", "키 세팅을 초기화 하시겠습니까?"))
										.ColorAndOpacity(InArgs._TextColor)
										.WrappingPolicy(ETextWrappingPolicy::DefaultWrapping)
										.AutoWrapText(false)
										//.WrapTextAt(30)
									]
									+ SVerticalBox::Slot()
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									.FillHeight(4)
									[
										SNew(SHorizontalBox)
										+SHorizontalBox::Slot()
										.HAlign(HAlign_Center)
										.VAlign(VAlign_Center)
										.Padding(FMargin(5,2))
										.AutoWidth()
										[
											SNew(SInputKeyLabel)
											.KeyboardKey(AcceptKeyboard)
											.GamepadKey(AcceptGamepad)
										]
										+ SHorizontalBox::Slot()
										.HAlign(HAlign_Center)
										.VAlign(VAlign_Center)
										.Padding(FMargin(5, 2))
										.AutoWidth()
										[
											SNew(STextBlock)
											.Text(LOCTEXT("Mapping Reset Accept", "초기화"))
											.Justification(ETextJustify::Center)
											.TextStyle(InArgs._SupportBlockStyle)
											
										]
										+ SHorizontalBox::Slot()
										.HAlign(HAlign_Center)
										.VAlign(VAlign_Center)
										.Padding(FMargin(5, 2))
										.AutoWidth()
										[
											SNew(SInputKeyLabel)
											.KeyboardKey(BackKeyboard)
											.GamepadKey(BackGamepad)
										]
										+ SHorizontalBox::Slot()
										.HAlign(HAlign_Center)
										.VAlign(VAlign_Center)
										.Padding(FMargin(5, 2))
										.AutoWidth()
										[
											SNew(STextBlock)
											.Text(LOCTEXT("Mapping Reset Back", "취소"))
											.Justification(ETextJustify::Center)
											.TextStyle(InArgs._SupportBlockStyle)
										]
									]
								]
						]
				]
		];


}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SCustomMappingReset::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingReset::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingReset::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingReset::OnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& InAnalogInputEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingReset::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingReset::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingReset::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingReset::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingReset::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingReset::OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	return FReply::Handled();
}

FReply SCustomMappingReset::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE