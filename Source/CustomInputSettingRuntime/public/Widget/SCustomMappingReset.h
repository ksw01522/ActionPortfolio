// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SCustomMappingReset : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SCustomMappingReset, SCompoundWidget)
public:
	SCustomMappingReset();

public:
	SLATE_BEGIN_ARGS(SCustomMappingReset) :
		_Padding(5),
		_BorderImage(nullptr),
		_BoxSize(720, 400),
		_GuideBlockStyle(nullptr),
		_SupportBlockStyle(nullptr),
		_TextColor(FLinearColor(0, 0, 0, 1))
		{
			_Clipping = EWidgetClipping::Inherit;
		}

		SLATE_ARGUMENT(FMargin, Padding)
		SLATE_ARGUMENT(const FSlateBrush*, BorderImage)

		SLATE_ARGUMENT(FVector2D, BoxSize)

		//텍스트 블록
		SLATE_ARGUMENT(const FTextBlockStyle*, GuideBlockStyle)
		SLATE_ARGUMENT(const FTextBlockStyle*, SupportBlockStyle)
		SLATE_ARGUMENT(FSlateColor, TextColor)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	//키 설정
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	//키 설정
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

	virtual bool SupportsKeyboardFocus() const { return false; }
};
