// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SCustomMappingSlate : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SCustomMappingSlate, SCompoundWidget)

public:

	SCustomMappingSlate();

public:
	SLATE_BEGIN_ARGS(SCustomMappingSlate) :
		_Padding(5),
		_BorderImage(nullptr),
		_BoxSize(720, 400),
		_NameBlockStyle(nullptr),
		_GuideBlockStyle(nullptr),
		_TextColor(FLinearColor(0, 0, 0, 1)),
		_ErrorColor(FLinearColor(1, 0, 0, 1))
		{
			_Clipping = EWidgetClipping::Inherit;
		}

		SLATE_ARGUMENT(FMargin, Padding)
		SLATE_ARGUMENT(const FSlateBrush*, BorderImage)

		SLATE_ARGUMENT(FVector2D, BoxSize)

		//텍스트 블록
		SLATE_ARGUMENT(const FTextBlockStyle*, NameBlockStyle)
		SLATE_ARGUMENT(const FTextBlockStyle*, GuideBlockStyle)
		SLATE_ARGUMENT(FSlateColor, TextColor)
		SLATE_ARGUMENT(FSlateColor, ErrorColor)


	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	FSlateColor TextColor;
	FSlateColor ErrorColor;

	FText GuideText;

public:

	//텍스트 블록 관련
	void SetFont(const FSlateFontInfo& NewFont);
	void SetAutoWrapText(bool NewState);
	void SetWrapTextAt(float NewWrapTextAt);
	void SetWrappingPolicy(ETextWrappingPolicy Policy);

	//기존 SWidget 오버라이드
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	virtual FReply OnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& InAnalogInputEvent) override;

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

private:

	TSharedPtr<STextBlock> NameTextBlock;
	TSharedPtr<STextBlock> GuideTextBlock;

public:
	void ReadyToCustomMapping(const FText& DisplayName);

	void ShowErrorText(const FText& InText);

};
