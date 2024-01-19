// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Widget/SInventorySlate.h"
#include "SlateOptMacros.h"
#include "WidgetStyle/ActionPortfolioWidgetStyle.h"

#include "Widgets/Images/SImage.h"
#include "Widgets/SOverlay.h"

#include "Items/ItemBase.h"
#include "Widgets/Layout/SBorder.h"

#include "Widgets/SInvalidationPanel.h"

#include "ActionPortfolio.h"

#include "Widgets/Layout/SWidgetSwitcher.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/Layout/SScrollBox.h"

SInventorySlot::SInventorySlot()
{
	
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInventorySlot::Construct(const FArguments& InArgs)
{
	FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();
	const FSlateBrush* SlotBackgroundBrush = InArgs._BackgroundBrush == nullptr ? InArgs._BackgroundBrush : StyleSet->GetBrush(InventoryStyle::SlotBackground);

	SlotIdx = InArgs._SlotIdx;

	SetCanTick(false);
	IconBrush.DrawAs = ESlateBrushDrawType::Image;

	const FTextBlockStyle* CountBlockStyle = &StyleSet->GetWidgetStyle<FTextBlockStyle>(InventoryStyle::CountTextStyle);

	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SAssignNew(BackgroundImage, SImage)
			.Image(SlotBackgroundBrush)
		]
		+SOverlay::Slot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SAssignNew(IconImage, SImage)
			.Visibility(EVisibility::Collapsed)
			.Image(&IconBrush)
		]
		+ SOverlay::Slot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SAssignNew(FrameImage, SImage)
			.Visibility(EVisibility::Collapsed)
		]
		+ SOverlay::Slot()
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Right)
		.Padding(FMargin(0,0,5,5))
		[
			SAssignNew(CountBlock, STextBlock)
			.Visibility(EVisibility::Collapsed)
			.TextStyle(CountBlockStyle)
		]
	];

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInventorySlot::SetCount(int NewCount)
{
	if (NewCount <= 1)
	{
		CountBlock->SetVisibility(EVisibility::Collapsed);
	}
	else
	{
		CountBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewCount)));
		CountBlock->SetVisibility(EVisibility::SelfHitTestInvisible);
	}
}

void SInventorySlot::SetItemIcon(TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade)
{
	if (IconMaterial == NewImage) return;

	IconMaterial = NewImage;

	// 아이템 아이콘 프레임 설정
	SetIconFrame(ItemGrade);
	FrameImage->SetVisibility(EVisibility::SelfHitTestInvisible);

	if (!NewImage.IsValid())
	{
		IconImage->SetVisibility(EVisibility::Collapsed);
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		Streamable.RequestAsyncLoad(NewImage.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([&, NewImage]() {
			if (IconMaterial != NewImage) return;

			IconBrush.SetResourceObject(NewImage.Get());
			IconImage->SetVisibility(EVisibility::SelfHitTestInvisible);
			}));
	}
	else
	{
		IconBrush.SetResourceObject(NewImage.Get());
		IconImage->SetVisibility(EVisibility::SelfHitTestInvisible);
	}
}

void SInventorySlot::SetIconFrame(EItemGrade ItemGrade)
{
	FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();

	FName FrameBrushPath;

	switch (ItemGrade)
	{
	case EItemGrade::Common:
		FrameBrushPath = InventoryStyle::SlotFrameGrade::Common;
		break;

	case EItemGrade::Uncommon:
		FrameBrushPath = InventoryStyle::SlotFrameGrade::Uncommon;
		break;

	case EItemGrade::Rare:
		FrameBrushPath = InventoryStyle::SlotFrameGrade::Rare;
		break;

	case EItemGrade::Unique:
		FrameBrushPath = InventoryStyle::SlotFrameGrade::Unique;
		break;

	case EItemGrade::Legendary:
		FrameBrushPath = InventoryStyle::SlotFrameGrade::Legendary;
		break;

	case EItemGrade::Epic:
		FrameBrushPath = InventoryStyle::SlotFrameGrade::Epic;
		break;
	}

	const FSlateBrush* NewFrameBrush = StyleSet->GetBrush(FrameBrushPath);
	FrameImage->SetImage(NewFrameBrush);
}

void SInventorySlot::UpdateSlotWidget(TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount)
{
#if WITH_EDITOR
	if (NewImage.IsNull())
	{
		PFLOG(Error, TEXT("if u need Clear Slot, Call ClearInventorySlotWidget."));
		return;
	}
	if (ItemGrade == EItemGrade::None)
	{
		PFLOG(Error, TEXT("U Call Set Inventory Slot Widget by None Grade."));
		return;
	}
	if (NewCount == 0)
	{
		PFLOG(Error, TEXT("U use Zero Count , if u need Clear Slot, Call ClearInventorySlotWidget."));
		return;
	}
#endif

	SetCount(NewCount);
	SetItemIcon(NewImage, ItemGrade);
}

void SInventorySlot::ClearInventorySlotWidget()
{
	if(IconMaterial.IsNull()) return;

	IconImage->SetVisibility(EVisibility::Collapsed);
	FrameImage->SetVisibility(EVisibility::Collapsed);
	CountBlock->SetVisibility(EVisibility::Collapsed);

	IconMaterial.Reset();
	IconBrush.SetResourceObject(nullptr);

	CountBlock->SetText(FText::GetEmpty());
}


////////////////////////////////////////////////////// SInventoryWindow //////////////////////////////////////

SInventoryWindow::SInventoryWindow()
{
	bInitialized = false;
}


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SInventoryWindow::Construct(const FArguments& InArgs)
{
	FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();
	const FSlateBrush* WindowBackgroundBrush = StyleSet->GetBrush(InventoryStyle::WindowBackground);

	SlotBackgroundBrush = *StyleSet->GetBrush(InventoryStyle::SlotBackground);
	

	SetCanTick(false);

	ChildSlot
	[
		SNew(SInvalidationPanel)
		[
			SAssignNew(WindowBoder, SBorder)
			.BorderImage(WindowBackgroundBrush)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(ScrollBox, SScrollBox)
				+SScrollBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
						SAssignNew(SlotWrapBox, SWrapBox)
						.Orientation(EOrientation::Orient_Horizontal)
						.UseAllottedSize(true)
				]
			]
		]
	];

	if (0 < InArgs._SlotCount){ InitializeInventoryWindow(InArgs._SlotCount, true); }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInventoryWindow::InitializeInventoryWindow(int Count, bool bReset)
{
	if (bInitialized && !bReset) {
		ensureMsgf(false, TEXT("Already Initialized Inventory Window"));
		return;
	}
	
	if(Count < 0) Count = 0;

	int BeforeCount = Slots.Num();

	if (BeforeCount == Count){ return; }
	
	if (BeforeCount < Count)
	{
		Slots.Reserve(Count);

		for (int i = BeforeCount; i < Count; ++i)
		{
			TSharedPtr<SInventorySlot> TempSlot;
			SAssignNew(TempSlot, SInventorySlot)
			.BackgroundBrush(&SlotBackgroundBrush);

			SlotWrapBox->AddSlot() [TempSlot.ToSharedRef()];
			Slots.Add(TempSlot);
		}
	}
	else
	{
	
		for (int i = 0; i < BeforeCount; i++)
		{
			Slots[i]->SetVisibility(EVisibility::Collapsed);
			Slots[i].Reset();
		}
		
		Slots.Empty();
		Slots.Reserve(Count);

		for (int i = 0; i < Count; ++i)
		{
			TSharedPtr<SInventorySlot> TempSlot;
			SAssignNew(TempSlot, SInventorySlot);

			SlotWrapBox->AddSlot()[TempSlot.ToSharedRef()];
			Slots.Add(TempSlot);
		}
	}


	bInitialized = true;
}

void SInventoryWindow::UpdateSlotWidget(int idx, TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount)
{
	if (!Slots.IsValidIndex(idx))
	{
		ensureMsgf(false, TEXT("Out Slots Index Range : %d"), idx);
		return;
	}

#if WITH_EDITOR
	if (!Slots[idx].IsValid())
	{
		ensureMsgf(false, TEXT("Invalidate Slot : %d"), idx);
		return;
	}
#endif
	
	Slots[idx]->UpdateSlotWidget(NewImage, ItemGrade, NewCount);
}



void SInventoryWindow::SetInnerSlotPadding(FVector2D NewPadding)
{
	SlotWrapBox->SetInnerSlotPadding(NewPadding);
}

void SInventoryWindow::SetWindowPadding(const FMargin& NewPadding)
{
	WindowBoder->SetPadding(NewPadding);
}

void SInventoryWindow::SetSlotSize(FVector2D NewSize)
{
	SlotBackgroundBrush.ImageSize = NewSize;
}

void SInventoryWindow::SetSlotBackgroundImage(TSoftObjectPtr<UMaterialInterface> NewImage)
{
	SlotBackgroundMaterial = NewImage;

	if (NewImage.IsNull())
	{
		FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();

	 	SlotBackgroundBrush.SetResourceObject( StyleSet->GetBrush(InventoryStyle::SlotBackground)->GetResourceObject());
	}
	else if (NewImage.IsValid())
	{
		SlotBackgroundBrush.SetResourceObject(NewImage.Get());
	}
	else
	{
		
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		Streamable.RequestAsyncLoad(NewImage.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([&, NewImage]() {
			if (SlotBackgroundMaterial != NewImage) return;

			SlotBackgroundBrush.SetResourceObject(NewImage.Get());
			}));

	}

}

////////////////////////////////////////////////////////////////////////////////////////////////


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInventory::Construct(const FArguments& InArgs)
{
	TSharedPtr<SHorizontalBox> SwitcherBTNBox;

	SetCanTick(false);

	ChildSlot[
		SNew(SBorder)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			.AutoHeight()
			[
				SAssignNew(SwitcherBTNBox, SHorizontalBox)
			]
			+SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			.FillHeight(1)
			[
				SAssignNew(InventoryWindowSwitcher, SWidgetSwitcher)
			]
		]
	];
	
	int SwitchIndex = 0;

	for (EItemType ItemType : TEnumRange<EItemType>())
	{
		TSharedPtr<SInventoryWindow> TempIW;

		SAssignNew(TempIW, SInventoryWindow)
		.SlotCount(InArgs._InventorySize);

		InventoryWindows.Add(ItemType, TempIW);
		InventoryWindowSwitcher->AddSlot()
		[
			TempIW.ToSharedRef()
		];

		SwitcherBTNBox->AddSlot()
			[
				SNew(SButton)
				.Text(FText::FromString("Test"))
				.OnClicked(FOnClicked::CreateLambda([=]() -> FReply {
				
					InventoryWindowSwitcher->SetActiveWidgetIndex(SwitchIndex);
					return FReply::Unhandled();
				}))
			];

		++SwitchIndex;
	}

	InventoryWindowSwitcher->SetActiveWidgetIndex(0);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInventory::UpdateSlotWidget(EItemType InventoryType, int idx, TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount)
{
#if WITH_EDITOR
	if (InventoryType == EItemType::None)
	{
		ensureMsgf(false, TEXT("Try Access None ItemType."));
		return;
	}
#endif

	InventoryWindows[InventoryType]->UpdateSlotWidget(idx, NewImage, ItemGrade, NewCount);
}

void SInventory::InitializeInventoryWindow(int SlotCount, bool bReset)
{
	for (EItemType Type : TEnumRange<EItemType>())
	{
		InventoryWindows[Type]->InitializeInventoryWindow(SlotCount, bReset);
	}
}

void SInventory::SetInnerSlotPadding(FVector2D NewPadding)
{
	for (EItemType Type : TEnumRange<EItemType>())
	{
		InventoryWindows[Type]->SetInnerSlotPadding(NewPadding);
	}

}

void SInventory::SetWindowPadding(const FMargin& NewPadding)
{
	for (EItemType Type : TEnumRange<EItemType>())
	{
		InventoryWindows[Type]->SetWindowPadding(NewPadding);
	}
}

void SInventory::SetSlotSize(FVector2D NewSize)
{
	for (EItemType Type : TEnumRange<EItemType>())
	{
		InventoryWindows[Type]->SetSlotSize(NewSize);
	}
}

void SInventory::SetSlotBackgroundImage(EItemType InventoryType, TSoftObjectPtr<UMaterialInterface> NewImage)
{
	InventoryWindows[InventoryType]->SetSlotBackgroundImage(NewImage);
}

EItemType SInventory::GetSlectedInventoryType() const
{
	TSharedPtr<SWidget> ActiveWidget = InventoryWindowSwitcher->GetActiveWidget();
	for (const auto& Window : InventoryWindows)
	{
		if (Window.Value.Get() == ActiveWidget.Get())
		{
			return Window.Key;
		}
	}

	return EItemType::None;
}




