// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

/**
 * 
 */

enum class EInputDeviceType : uint8;

class FDragDropOperation_AbilitySlot : public FDragDropOperation
{
public:
	DRAG_DROP_OPERATOR_TYPE(FDragDropOperation_AbilitySlot, FDragDropOperation)

	virtual ~FDragDropOperation_AbilitySlot(){}

protected:
	TWeakObjectPtr<class UAbilitySlot> FromSlot;
	TWeakObjectPtr<class UAbilitySlot> ToSlot;

protected:
	virtual void Construct() override;

	virtual void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;

private:
	//virtual bool IsOfTypeImpl(const FString& Type) const override;
	virtual TSharedPtr<SWidget> GetDefaultDecorator() const override;

public:
	static TSharedRef<FDragDropOperation_AbilitySlot> AbilitySlotDrag(UAbilitySlot* InFromSlot);

	void SetToSlot(UAbilitySlot* InSlot) { ToSlot = InSlot; }
};


class ACTIONPORTFOLIO_API SAbilitySlot : public SLeafWidget
{
	SLATE_DECLARE_WIDGET(SAbilitySlot, SLeafWidget)

public:
	SAbilitySlot();
	virtual ~SAbilitySlot();

	SLATE_BEGIN_ARGS(SAbilitySlot) :
			_LinkSlot(nullptr),
			_BackgroundBrush(nullptr),
			_BackgroundColorAndOpacity(FLinearColor(1,1,1,1)),
			_SlotSize(36)
	{}

	SLATE_ARGUMENT(class UAbilitySlot*, LinkSlot)
	SLATE_ARGUMENT(const FSlateBrush*, BackgroundBrush)
	SLATE_ARGUMENT(FSlateColor, BackgroundColorAndOpacity)
	SLATE_ARGUMENT(FVector2D, SlotSize)
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	const FSlateBrush* BackgroundBrush;
	FSlateColor BGColorAndOpacity;
	FVector2D SlotSize;

public:
	void SetBackgroundBrush(const FSlateBrush* InBrush);
	void SetSlotSize(FVector2D InSize) { SlotSize = InSize; }

protected:
	virtual FVector2D ComputeDesiredSize(float) const override;

public:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
	TWeakObjectPtr<class UAbilitySlot> LinkedSlot;

	TSharedPtr<class SAbilityIcon> AbilityIcon;

public:
	void LinkAbilitySlot(UAbilitySlot* InSlot);
	void ClearAbilitySlot();

	void SetAbilityIcon(const TSharedPtr<class SAbilityIcon>& InIcon);

protected:
	//virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

};

class ACTIONPORTFOLIO_API SAbilitySlot_HotKey : public SAbilitySlot
{
	SLATE_DECLARE_WIDGET(SAbilitySlot_HotKey, SAbilitySlot)

public:
	SAbilitySlot_HotKey();
	virtual ~SAbilitySlot_HotKey();

	SLATE_BEGIN_ARGS(SAbilitySlot_HotKey) :
		_BackgroundBrush(nullptr),
		_BackgroundColorAndOpacity(FLinearColor(1, 1, 1, 1)),
		_SlotSize(36)
		{}

		SLATE_ARGUMENT(const FSlateBrush*, BackgroundBrush)
		SLATE_ARGUMENT(FSlateColor, BackgroundColorAndOpacity)
		SLATE_ARGUMENT(FVector2D, SlotSize)
		SLATE_ARGUMENT(FKey, HotKey_Keyboard)
		SLATE_ARGUMENT(FKey, HotKey_Gamepad)


	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	FDelegateHandle OnChangedInputDeviceHandle;

	bool bIsGamepad;
	FSlateBrush HotKeyBrush_Keyboard;
	FSlateBrush HotKeyBrush_Gamepad;

private:
	void AddOnChangedDeviceEvent();
	void ClearOnChangedDeviceEvent();

	void OnCurrentDeviceChanged(EInputDeviceType InType);

public:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	void SetHotKeyKeyboard(const FKey& InKey);
	void SetHotKeyGamepad(const FKey& InKey);

	void SetHotKeys(const FKey& KeyboardKey, const FKey& GamepadKey);
	
};
