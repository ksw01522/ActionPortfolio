// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

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

protected:
	virtual void Construct() override;

	virtual void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;

private:
	//virtual bool IsOfTypeImpl(const FString& Type) const override;
	virtual TSharedPtr<SWidget> GetDefaultDecorator() const override;

public:
	UAbilitySlot* GetFromSlot() const {return FromSlot.Get(); }

	static TSharedRef<FDragDropOperation_AbilitySlot> AbilitySlotDrag(UAbilitySlot* InFromSlot);
};


class ACTIONPORTFOLIO_API SAbilitySlot : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SAbilitySlot, SCompoundWidget)

public:
	SAbilitySlot();

	SLATE_BEGIN_ARGS(SAbilitySlot) :
			_BackgroundBrush(nullptr),
			_BackgroundColorAndOpacity(FLinearColor(1,1,1,1)),
			_SlotSize(36)
	{}

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

private:
	TWeakObjectPtr<class UAbilitySlot> OwnerSlot;

protected:
	virtual FVector2D ComputeDesiredSize(float) const override;

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

public:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;


public:
	UAbilitySlot* GetOwnerSlot() const;
	void LinkOwnerSlot(UAbilitySlot* InOwner);
	void UnlinkOwnerSlot(UAbilitySlot* InOwner);

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

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	FDelegateHandle OnChangedInputDeviceHandle;

	const FSlateBrush* CurrentHotKeyBrush;
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
