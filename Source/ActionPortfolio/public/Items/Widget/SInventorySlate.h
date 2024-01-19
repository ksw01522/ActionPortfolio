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

 struct FItemData_Base;

 enum class EItemGrade : uint8;
 enum class EItemType : uint8;

class ACTIONPORTFOLIO_API SInventorySlot : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInventorySlot) :
		_BackgroundBrush(nullptr), _SlotIdx(-1)
	{}

	SLATE_ARGUMENT(const FSlateBrush*, BackgroundBrush);
	SLATE_ARGUMENT(int, SlotIdx);

	SLATE_END_ARGS()

	SInventorySlot();

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SImage> BackgroundImage;

	TSharedPtr<SImage> IconImage;
	TSoftObjectPtr<UMaterialInterface> IconMaterial;
	FSlateBrush IconBrush;

	TSharedPtr<SImage> FrameImage;

	TSharedPtr<STextBlock> CountBlock;

	int SlotIdx = -1;

	void SetItemIcon(TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade);
	void SetIconFrame(EItemGrade ItemGrade);
	void SetCount(int NewCount);

public:
	void UpdateSlotWidget(TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount);
	void ClearInventorySlotWidget();
	int GetSlotIdx() const {return SlotIdx;}
};

class ACTIONPORTFOLIO_API SInventoryWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInventoryWindow) : _SlotCount(0)
		{} 

	SLATE_ARGUMENT(int, SlotCount);

	SLATE_END_ARGS()

	SInventoryWindow();

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<class SWrapBox> SlotWrapBox;
	TSharedPtr<class SBorder> WindowBoder;
	TSharedPtr<class SScrollBox> ScrollBox;

	TArray<TSharedPtr<SInventorySlot>> Slots;

	FSlateBrush SlotBackgroundBrush;
	TSoftObjectPtr<UMaterialInterface> SlotBackgroundMaterial;

	bool bInitialized;

public:
	void InitializeInventoryWindow(int SlotCount, bool bReset = false);
	
	void UpdateSlotWidget(int idx, TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount);

	void SetInnerSlotPadding(FVector2D NewPadding);

	void SetWindowPadding(const FMargin& NewPadding);

	void SetSlotSize(FVector2D NewSize);

	void SetSlotBackgroundImage(TSoftObjectPtr<UMaterialInterface> NewImage);
};

class ACTIONPORTFOLIO_API SInventory : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInventory) : _InventorySize(0)
		{}

	SLATE_ARGUMENT(int, InventorySize);

	SLATE_END_ARGS()

	TMap<EItemType, TSharedPtr<SInventoryWindow>> InventoryWindows;
	TSharedPtr<SWidgetSwitcher> InventoryWindowSwitcher;

	void Construct(const FArguments& InArgs);

	void UpdateSlotWidget(EItemType InventoryType, int idx, TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount);

	void InitializeInventoryWindow(int SlotCount, bool bReset = false);

	void SetInnerSlotPadding(FVector2D NewPadding);

	void SetWindowPadding(const FMargin& NewPadding);

	void SetSlotSize(FVector2D NewSize);

	void SetSlotBackgroundImage(EItemType InventoryType, TSoftObjectPtr<UMaterialInterface> NewImage);

	EItemType GetSlectedInventoryType() const;
};