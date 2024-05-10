// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SItemSlot.h"
/**
 * 
 */
 class SImage;
 class SUniformGridPanel;
 class STextBlock;


class ACTIONPORTFOLIO_API SInventoryWindow : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SInventoryWindow, SCompoundWidget);

public:
	SLATE_BEGIN_ARGS(SInventoryWindow) : _Count(0), _CountByRow(1), _SlotPadding(FMargin(0)), _SlotBackgroundBrush(nullptr)
		{} 

	SLATE_ARGUMENT(int, Count)
	SLATE_ARGUMENT(int, CountByRow)
	SLATE_ARGUMENT(FMargin, SlotPadding)
	SLATE_ARGUMENT(const FSlateBrush*, SlotBackgroundBrush)

	SLATE_END_ARGS()

	SInventoryWindow();
	virtual ~SInventoryWindow();

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<class SInvalidationPanel> InvalidationPanel;

	TSharedPtr<SUniformGridPanel> SlotPanel;
	int CountByRow;

	TSharedPtr<class SScrollBox> ScrollBox;

	TArray<TSharedPtr<class SItemSlot>> Slots;

	const FSlateBrush* SlotBackgroundBrush;

	bool bInitialized;

public:
	//������ ������ ���� �����ϱ�
	void InitializeInventoryWindow(int Count, bool bReset = false);

private:
	//���� �׺���̼� ����� Ȥ�� ���� : ���Ը���ų�, ��� ����ī��Ʈ�� ��ȭ�� ����� �׺���̼� ����
	void SlotNavigationBuild();

	//OverBound �׺���̼� ������Ʈ
	void UpdateOverBoundNavi();

	virtual bool SupportsKeyboardFocus() const {return true;}

public:
	void SetOverBoundNaviStop(EUINavigation Dir);
	void SetOverBoundNaviEscape(EUINavigation Dir);
	void SetOverBoundNaviWrap(EUINavigation Dir);
	void SetOverBoundNaviExplicit(EUINavigation Dir, TSharedPtr<SWidget> InWidget);

	SItemSlot* GetInventorySlot(int Idx) const;

public:	
	//������ Padding ����
	void SetSlotPadding(const FMargin& NewPadding);

	//��� ī��Ʈ �ٲٱ�
	void SetSlotCountByRow(int NewCount);

	void SetSlotBackgroundBrush(const FSlateBrush* NewBrush);

	void OnSelectedWindow();

	void BindInventorySlot(TArray<class UInventorySlot*> InSlots);

private:
	mutable FOnFocusedItemSlot OnFocusedItemSlot;

	void OnFocusedItemSlotFunction(UItemSlot* InSlot) const;

public:
	FOnFocusedItemSlot& GetOnFocusedItemSlot() const {return OnFocusedItemSlot;}
};

/*
class ACTIONPORTFOLIO_API SInventory : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SInventory, SCompoundWidget)

public:
	SLATE_BEGIN_ARGS(SInventory) : _InventorySize(0)
		{}

	SLATE_ARGUMENT(int, InventorySize);

	SLATE_END_ARGS()

	TMap<EItemType, TSharedPtr<SInventoryWindow>> InventoryWindows;
	TSharedPtr<SWidgetSwitcher> InventoryWindowSwitcher;

	SInventory();

	void Construct(const FArguments& InArgs);

	void UpdateSlotWidget(EItemType InventoryType, int idx, TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount);

	void InitializeInventoryWindow(int SlotCount, bool bReset = false);

	void SetSlotPadding(const FMargin& NewPadding);

	void SetSlotSize(FVector2D NewSize);

	void SelectInventoryWindow(EItemType InventoryType);

	void SetSlotCountByRow(int NewCount);

	void SetSlotBackgroundImage(EItemType InventoryType, TSoftObjectPtr<UMaterialInterface> NewImage);

	EItemType GetSlectedInventoryType() const;

	void ShowInventoryWindow(EItemType InventoryType);
};
*/