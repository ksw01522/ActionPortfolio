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
#include "Widgets/Layout/SUniformGridPanel.h"

#include "Types/NavigationMetaData.h"
#include "Items/Widget/SItemSlot.h"
#include "Character/Player/InventoryComponent.h"
#include "Widgets/Layout/SScaleBox.h"

SLATE_IMPLEMENT_WIDGET(SInventoryWindow)
void SInventoryWindow::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{}

SInventoryWindow::SInventoryWindow()
{
	bInitialized = false;
	SetCanTick(false);
}

SInventoryWindow::~SInventoryWindow()
{


}


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SInventoryWindow::Construct(const FArguments& InArgs)
{
	FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();

	CountByRow = InArgs._CountByRow;

	SlotBackgroundBrush = InArgs._SlotBackgroundBrush;

	ChildSlot
	[
		SAssignNew(InvalidationPanel, SInvalidationPanel)
		[
			SAssignNew(ScrollBox, SScrollBox)
			+SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
					SAssignNew(SlotPanel, SUniformGridPanel)
			]
			
		]
	];

	TSharedPtr<FNavigationMetaData> WindowNaviMetaData = GetMetaData<FNavigationMetaData>();
	if (!WindowNaviMetaData.IsValid())
	{
		WindowNaviMetaData = MakeShared<FNavigationMetaData>();

		for(EUINavigation Dir : TEnumRange<EUINavigation>())
		{
			WindowNaviMetaData->SetNavigationStop(Dir);
		}

		AddMetadata(WindowNaviMetaData.ToSharedRef());
	}

	InitializeInventoryWindow(InArgs._Count, true);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SInventoryWindow::InitializeInventoryWindow(int Count, bool bReset)
{
	if (bInitialized && !bReset) {
		ensureMsgf(false, TEXT("Already Initialized Inventory Window"));
		return;
	}
	
	if(Count < 0) Count = 0;

	if(Slots.Num() == Count) return;

	for (int i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->SetVisibility(EVisibility::Collapsed);
		SlotPanel->RemoveSlot(Slots[i].ToSharedRef());
		Slots[i].Reset();
	}

	Slots.Empty();
	Slots.Reserve(Count);

	for (int i = 0; i < Count; ++i)
	{
		TSharedPtr<SItemSlot> TempSlot;
		SAssignNew(TempSlot, SItemSlot)
			.BackgroundBrush(SlotBackgroundBrush);

		TempSlot->GetOnFocusedItemSlot().AddSP(this, &SInventoryWindow::OnFocusedItemSlotFunction);
	
		TSharedPtr<SScaleBox> ScaleBox;

		SAssignNew(ScaleBox, SScaleBox)
		.Stretch(EStretch::ScaleToFit)
		.StretchDirection(EStretchDirection::UpOnly)
		[
			TempSlot.ToSharedRef()
		];

		SlotPanel->AddSlot(i % CountByRow, i / CountByRow)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				ScaleBox.ToSharedRef()
			];
		Slots.Add(MoveTemp(TempSlot));
	}

	SlotNavigationBuild();

	bInitialized = true;
}




void SInventoryWindow::SetSlotPadding(const FMargin& NewPadding)
{
	SlotPanel->SetSlotPadding(NewPadding);
}

void SInventoryWindow::SlotNavigationBuild()
{
	int SlotsNum = Slots.Num();

	TSharedPtr<FNavigationMetaData> WindowNaviMetaData = GetMetaData<FNavigationMetaData>();
	check(WindowNaviMetaData.IsValid());

	for (int i = 0; i < SlotsNum; i++)
	{
		//Navigation 제작
		TSharedPtr<FNavigationMetaData> NaviMetaData = Slots[i]->GetMetaData<FNavigationMetaData>();
		if (!NaviMetaData.IsValid())
		{
			NaviMetaData = MakeShared<FNavigationMetaData>();
			Slots[i]->AddMetadata(NaviMetaData.ToSharedRef());
		}


		NaviMetaData->SetNavigationStop(EUINavigation::Previous);
		NaviMetaData->SetNavigationStop(EUINavigation::Next);
		
		if (i % CountByRow == 0)	{ NaviMetaData->SetNavigationStop(EUINavigation::Left); }
		else						{ NaviMetaData->SetNavigationExplicit(EUINavigation::Left, Slots[i - 1]); }

		if (i % CountByRow == CountByRow - 1 || SlotsNum - 1 <= i)	{ NaviMetaData->SetNavigationStop(EUINavigation::Right); }
		else										{ NaviMetaData->SetNavigationExplicit(EUINavigation::Right, Slots[i + 1]); }
		
		if(i - CountByRow < 0)		{ NaviMetaData->SetNavigationStop(EUINavigation::Up); }
		else						{ NaviMetaData->SetNavigationExplicit(EUINavigation::Up, Slots[i - CountByRow]); }

		if(SlotsNum <= i + CountByRow)	{ NaviMetaData->SetNavigationStop(EUINavigation::Down); }
		else							{ NaviMetaData->SetNavigationExplicit(EUINavigation::Down, Slots[i + CountByRow]); }
	}
}

void SInventoryWindow::UpdateOverBoundNavi()
{
	int SlotCount = Slots.Num();
	if(SlotCount == 0 || CountByRow <= 0) return;

	TSharedPtr<FNavigationMetaData> WindowMeta = GetMetaData<FNavigationMetaData>();

	{
		EUINavigationRule Rule = WindowMeta->GetBoundaryRule(EUINavigation::Up);

		for(int i = 0; i < CountByRow ; i++)
		{
			TSharedPtr<FNavigationMetaData> SlotMeta = GetMetaData<FNavigationMetaData>();
			TWeakPtr<SWidget> ExplicitWidget = WindowMeta->GetFocusRecipient(EUINavigation::Up);

			switch (Rule)
			{
			case EUINavigationRule::Escape:
				SlotMeta->SetNavigationEscape(EUINavigation::Up);
				break;
			case EUINavigationRule::Explicit:
				SlotMeta->SetNavigationExplicit(EUINavigation::Up, ExplicitWidget.Pin());
				break;
			case EUINavigationRule::Wrap:
				SlotMeta->SetNavigationWrap(EUINavigation::Up);
				break;
			case EUINavigationRule::Stop:
				SlotMeta->SetNavigationStop(EUINavigation::Up);
				break;
			default:
				break;
			}
		}
	}

	{
		EUINavigationRule Rule = WindowMeta->GetBoundaryRule(EUINavigation::Down);

		int Start = (SlotCount - 1) / CountByRow;

		for (int i = Start * CountByRow; i < SlotCount; i++)
		{
			TSharedPtr<FNavigationMetaData> SlotMeta = GetMetaData<FNavigationMetaData>();
			TWeakPtr<SWidget> ExplicitWidget = WindowMeta->GetFocusRecipient(EUINavigation::Down);

			switch (Rule)
			{
			case EUINavigationRule::Escape:
				SlotMeta->SetNavigationEscape(EUINavigation::Down);
				break;
			case EUINavigationRule::Explicit:
				SlotMeta->SetNavigationExplicit(EUINavigation::Down, ExplicitWidget.Pin());
				break;
			case EUINavigationRule::Wrap:
				SlotMeta->SetNavigationWrap(EUINavigation::Down);
				break;
			case EUINavigationRule::Stop:
				SlotMeta->SetNavigationStop(EUINavigation::Down);
				break;
			default:
				break;
			}
		}

	}

	{
		EUINavigationRule Rule = WindowMeta->GetBoundaryRule(EUINavigation::Left);

		for (int i = 0; i < SlotCount; i+=CountByRow)
		{
			TSharedPtr<FNavigationMetaData> SlotMeta = GetMetaData<FNavigationMetaData>();
			TWeakPtr<SWidget> ExplicitWidget = WindowMeta->GetFocusRecipient(EUINavigation::Left);

			switch (Rule)
			{
			case EUINavigationRule::Escape:
				SlotMeta->SetNavigationEscape(EUINavigation::Left);
				break;
			case EUINavigationRule::Explicit:
				SlotMeta->SetNavigationExplicit(EUINavigation::Left, ExplicitWidget.Pin());
				break;
			case EUINavigationRule::Wrap:
				SlotMeta->SetNavigationWrap(EUINavigation::Left);
				break;
			case EUINavigationRule::Stop:
				SlotMeta->SetNavigationStop(EUINavigation::Left);
				break;
			default:
				break;
			}
		}
	}

	{
		EUINavigationRule Rule = WindowMeta->GetBoundaryRule(EUINavigation::Right);

		for (int i = CountByRow - 1; i < SlotCount; i+=CountByRow)
		{
			TSharedPtr<FNavigationMetaData> SlotMeta = GetMetaData<FNavigationMetaData>();
			TWeakPtr<SWidget> ExplicitWidget = WindowMeta->GetFocusRecipient(EUINavigation::Right);

			switch (Rule)
			{
			case EUINavigationRule::Escape:
				SlotMeta->SetNavigationEscape(EUINavigation::Right);
				break;
			case EUINavigationRule::Explicit:
				SlotMeta->SetNavigationExplicit(EUINavigation::Right, ExplicitWidget.Pin());
				break;
			case EUINavigationRule::Wrap:
				SlotMeta->SetNavigationWrap(EUINavigation::Right);
				break;
			case EUINavigationRule::Stop:
				SlotMeta->SetNavigationStop(EUINavigation::Right);
				break;
			default:
				break;
			}
		}
	}
}


void SInventoryWindow::SetSlotCountByRow(int NewCount)
{
	if(NewCount < 1) NewCount = 1;

	if(CountByRow == NewCount) return;

	CountByRow = NewCount;

	FChildren* PanelChildren = SlotPanel->GetChildren();
	int SlotNum = Slots.Num();

	for(int i = 0; i < SlotNum; i++)
	{
		 SUniformGridPanel::FSlot& PanelSlot = const_cast<SUniformGridPanel::FSlot&>(static_cast<const SUniformGridPanel::FSlot&>(PanelChildren->GetSlotAt(i)));
		 PanelSlot.SetColumn(i % CountByRow);
		 PanelSlot.SetRow(i / CountByRow);
	}

	SlotNavigationBuild();
}

void SInventoryWindow::SetSlotBackgroundBrush(const FSlateBrush* NewBrush)
{
	SlotBackgroundBrush = NewBrush;

	for (int i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->SetBackgroundBrush(SlotBackgroundBrush);
	}
}

void SInventoryWindow::OnSelectedWindow()
{
	uint32 Test = FSlateApplication::Get().GetUserIndexForKeyboard();

	if(!Slots.IsEmpty()) FSlateApplication::Get().SetUserFocus(Test, Slots[0], EFocusCause::SetDirectly);
}

void SInventoryWindow::BindInventorySlot(TArray<UInventorySlot*> InSlots)
{
	checkf(InSlots.Num() == Slots.Num(), TEXT("SlotCount %d != Slate Slot Count %d"), InSlots.Num(), Slots.Num());
	for (int i = 0; i < InSlots.Num(); i++)
	{
		GetInventorySlot(i)->BindItemSlot(InSlots[i]);
	}
}

void SInventoryWindow::OnFocusedItemSlotFunction(UItemSlot* InSlot) const
{
	if(OnFocusedItemSlot.IsBound()) OnFocusedItemSlot.Broadcast(InSlot);
}

void SInventoryWindow::SetOverBoundNaviStop(EUINavigation Dir)
{
	TSharedPtr<FNavigationMetaData> WindowNaviMeta = GetMetaData<FNavigationMetaData>();

	WindowNaviMeta->SetNavigationStop(Dir);

	UpdateOverBoundNavi();
}

void SInventoryWindow::SetOverBoundNaviEscape(EUINavigation Dir)
{
	TSharedPtr<FNavigationMetaData> WindowNaviMeta = GetMetaData<FNavigationMetaData>();

	WindowNaviMeta->SetNavigationEscape(Dir);

	UpdateOverBoundNavi();

}

void SInventoryWindow::SetOverBoundNaviWrap(EUINavigation Dir)
{
	TSharedPtr<FNavigationMetaData> WindowNaviMeta = GetMetaData<FNavigationMetaData>();

	WindowNaviMeta->SetNavigationWrap(Dir);

	UpdateOverBoundNavi();

}

void SInventoryWindow::SetOverBoundNaviExplicit(EUINavigation Dir, TSharedPtr<SWidget> InWidget)
{
	TSharedPtr<FNavigationMetaData> WindowNaviMeta = GetMetaData<FNavigationMetaData>();

	WindowNaviMeta->SetNavigationExplicit(Dir, InWidget);

	UpdateOverBoundNavi();
}

SItemSlot* SInventoryWindow::GetInventorySlot(int Idx) const
{
	if(!Slots.IsValidIndex(Idx)) return nullptr;
	return Slots[Idx].Get();
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*
SLATE_IMPLEMENT_WIDGET(SInventory)
void SInventory::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SInventory::SInventory()
{
	SetCanTick(false);
}

void SInventory::Construct(const FArguments& InArgs)
{
	TSharedPtr<SHorizontalBox> SwitcherBTNBox;

	
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
				.OnClicked(FOnClicked::CreateLambda([&, ItemType]() -> FReply {
					this->SelectInventoryWindow(ItemType);

					return FReply::Handled();
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

void SInventory::SetSlotPadding(const FMargin& NewPadding)
{
	for (EItemType Type : TEnumRange<EItemType>())
	{
		InventoryWindows[Type]->SetSlotPadding(NewPadding);
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

void SInventory::SelectInventoryWindow(EItemType InventoryType)
{
	if (InventoryType == EItemType::None) {
		PFLOG(Error, TEXT("Select Window Inventory Type : None"));
		return;
	}

	InventoryWindowSwitcher->SetActiveWidget(InventoryWindows[InventoryType].ToSharedRef());
	InventoryWindows[InventoryType]->OnSelectedWindow();
}

void SInventory::SetSlotCountByRow(int NewCount)
{
	for (const auto& Type : TEnumRange<EItemType>())
	{
		InventoryWindows[Type]->SetSlotCountByRow(NewCount);
	}
}

void SInventory::ShowInventoryWindow(EItemType InventoryType)
{
	if(ensure(InventoryType != EItemType::None)){ return; }

	int InventoryIdx = (int)InventoryType - 1;

	InventoryWindowSwitcher->SetActiveWidgetIndex(InventoryIdx);
}

*/

