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


 struct FItemData_Base;

 enum class EItemGrade : uint8;
 enum class EItemType : uint8;

DECLARE_DELEGATE_RetVal(FReply, FInventorySlotEvent);

class ACTIONPORTFOLIO_API SInventorySlot : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SInventorySlot, SCompoundWidget)

public:
	SLATE_BEGIN_ARGS(SInventorySlot) :
		_BackgroundBrush(nullptr), _SlotIdx(-1)
	{}

	SLATE_ARGUMENT(const FSlateBrush*, BackgroundBrush);
	SLATE_ARGUMENT(int, SlotIdx);

	SLATE_END_ARGS()

	SInventorySlot();
	virtual ~SInventorySlot();

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

private:
	FInventorySlotEvent OnAcceptEvent;
	FInventorySlotEvent OnBackEvent;

protected:
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;

	virtual bool SupportsKeyboardFocus() const;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	virtual FReply OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent);

	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;


	virtual void OnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;

public:
	void SetAcceptEvent(const TDelegate<FReply()>& NewEvent);
	void SetBackEvent(const TDelegate<FReply()>& NewEvent);

public:
	void UpdateSlotWidget(TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount);
	void ClearInventorySlotWidget();
	int GetSlotIdx() const {return SlotIdx;}
};

class ACTIONPORTFOLIO_API SInventoryWindow : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SInventoryWindow, SCompoundWidget);

public:
	SLATE_BEGIN_ARGS(SInventoryWindow) : _SlotCount(0) , _CountByRow(1)
		{} 

	SLATE_ARGUMENT(int, SlotCount);
	SLATE_ARGUMENT(int, CountByRow)

	SLATE_END_ARGS()

	SInventoryWindow();
	~SInventoryWindow();

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SUniformGridPanel> SlotPanel;
	int CountByRow;

	TSharedPtr<class SScrollBox> ScrollBox;

	TArray<TSharedPtr<SInventorySlot>> Slots;

	FSlateBrush SlotBackgroundBrush;
	TSoftObjectPtr<UMaterialInterface> SlotBackgroundMaterial;

	bool bInitialized;

	TSharedPtr<struct FStreamableHandle> StreamingHandle;
	FVector2D SlotSize;



public:
	//������ ������ ���� �����ϱ�
	void InitializeInventoryWindow(int SlotCount, bool bReset = false);

private:
	//���� �׺���̼� ����� Ȥ�� ���� : ���Ը���ų�, ��� ����ī��Ʈ�� ��ȭ�� ����� �׺���̼� ����
	void SlotNavigationBuild();

public:	
	//���Կ� �ش�Ǵ� �κ��丮ĭ�� ��ȭ�� ����� ������Ʈ ���ֱ�
	void UpdateSlotWidget(int idx, TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount);

	//������ Padding ����
	void SetSlotPadding(const FMargin& NewPadding);

	//��� ī��Ʈ �ٲٱ�
	void SetSlotCountByRow(int NewCount);

	//���� ������
	void SetSlotSize(FVector2D NewSize);

	void SetSlotBackgroundImage(TSoftObjectPtr<UMaterialInterface> NewImage);

	void OnSelectedWindow();
};

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