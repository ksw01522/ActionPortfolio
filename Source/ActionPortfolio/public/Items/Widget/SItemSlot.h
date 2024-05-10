// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
/**
 * 
 */
 class SImage;
 class SUniformGridPanel;
 class STextBlock;
 class AActionPFPlayerController;
 class UItemSlot;
 class UItemBase;

 struct FItemData_Base;

 enum class EItemGrade : uint8;
 enum class EItemType : uint8;

class FItemSlotDragDropOperation : public FDragDropOperation
{
public:
	DRAG_DROP_OPERATOR_TYPE(FItemSlotDragDropOperation, FDragDropOperation);

	FItemSlotDragDropOperation() : FromSlot(nullptr), ToSlot(nullptr)
	{ bCreateNewWindow = true; }

	FItemSlotDragDropOperation(UItemSlot* InFromSlot);

public:
	virtual void Construct() override;

protected:
	virtual void OnDragged(const class FDragDropEvent& DragDropEvent) override;
	virtual void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;

	virtual TSharedPtr<SWidget> GetDefaultDecorator() const override;

private:
	TWeakObjectPtr<UItemSlot> FromSlot;
	TWeakObjectPtr<UItemSlot> ToSlot;

	FSlateBrush IconBrush;

public:
	void SetFromSlot(UItemSlot* InSlot);
	void SetToSlot(UItemSlot* InSlot);

	static TSharedRef<FItemSlotDragDropOperation> ItemSlotDrag(class SItemSlot* InFromSlot);
};

DECLARE_DELEGATE_RetVal(FReply, FSlotSlateEvent);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFocusedItemSlot, UItemSlot*);

class ACTIONPORTFOLIO_API SItemSlot : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SItemSlot, SCompoundWidget)

	friend class FItemSlotDragDropOperation;

public:
	SLATE_BEGIN_ARGS(SItemSlot) :
		_BackgroundBrush(nullptr),
		_ItemSlot(nullptr)
	{}

	SLATE_ARGUMENT(const FSlateBrush*, BackgroundBrush)
	SLATE_ARGUMENT(class UItemSlot*, ItemSlot)

	SLATE_END_ARGS()

	SItemSlot();
	virtual ~SItemSlot();

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	TWeakObjectPtr<UItemSlot> BindedSlot;

	FDelegateHandle OnChangedSlotHandle;

public:
	void BindItemSlot(UItemSlot* InSlot);

	void SetBackgroundBrush(const FSlateBrush* InBrush);

private:
	TSharedPtr<SImage> BackgroundImage;

	TSharedPtr<SImage> IconImage;
	TSoftObjectPtr<UMaterialInterface> IconMaterial;
	FSlateBrush IconBrush;

	TSharedPtr<SImage> FrameImage;

	TSharedPtr<STextBlock> CountBlock;

	FOnFocusedItemSlot OnFocusedItemSlot;

public:
	//TSharedRef<SImage> GetIconImage() const { return IconImage.ToSharedRef(); }
	//TSharedRef<SImage> GetFrameImage() const { return FrameImage.ToSharedRef(); }

private:
	void UpdateSlotSlate(TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount);
	void ClearSlotSlate();

	void SetItemIcon(TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade);
	void SetIconFrame(EItemGrade ItemGrade);
	void SetCount(int NewCount);

private:
	FSlotSlateEvent OnAcceptEvent;

protected:
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;

	virtual bool SupportsKeyboardFocus() const;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	virtual FReply OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent);

	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;


	//virtual void OnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;


	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	virtual void OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent);

	virtual void OnDragLeave(const FDragDropEvent& DragDropEvent);

	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent);

	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent);

public:
	void SetAcceptEvent(const TDelegate<FReply()>& NewEvent);

	void UpdateSlotSlate(const UItemBase* InItem);

	FOnFocusedItemSlot& GetOnFocusedItemSlot() {return OnFocusedItemSlot;}
};
