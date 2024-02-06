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

SLATE_IMPLEMENT_WIDGET(SInventorySlot)
void SInventorySlot::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{


}

SInventorySlot::SInventorySlot()
{
	SetCanTick(false);
}

SInventorySlot::~SInventorySlot()
{
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInventorySlot::Construct(const FArguments& InArgs)
{
	FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();
	const FSlateBrush* SlotBackgroundBrush = InArgs._BackgroundBrush;

	SlotIdx = InArgs._SlotIdx;

	IconBrush.DrawAs = ESlateBrushDrawType::Image;

#if WITH_EDITOR
	const FTextBlockStyle* CountBlockStyle = nullptr;

	if (StyleSet != nullptr)
	{
		CountBlockStyle = &StyleSet->GetWidgetStyle<FTextBlockStyle>(InventoryStyle::CountTextStyle);
	}
	else
	{
		FSlateFontInfo CountTextBlockFont(FPaths::ProjectContentDir() + TEXT("WidgetStyle/NPCInteract/Font/Default.ttf"), 12);
		FLinearColor CountTextBlockColor(0.7f, 0.4f, 0, 1);

		static FTextBlockStyle ForEditorCountBlockStyle;
		ForEditorCountBlockStyle.SetFont(CountTextBlockFont);
		ForEditorCountBlockStyle.SetColorAndOpacity(CountTextBlockColor);
		ForEditorCountBlockStyle.SetShadowOffset(FVector2D(0.1f, 0.1f));
		
		CountBlockStyle = &ForEditorCountBlockStyle;
	}
#else
	const FTextBlockStyle* CountBlockStyle = &StyleSet->GetWidgetStyle<FTextBlockStyle>(InventoryStyle::CountTextStyle);
#endif
	
	ChildSlot
	[
		SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SAssignNew(BackgroundImage, SImage)
					.Visibility(EVisibility::Visible)
					.Image(InArgs._BackgroundBrush)
			]
			+ SOverlay::Slot()
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
			.Padding(FMargin(0, 0, 5, 5))
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

FReply SInventorySlot::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
#if WITH_EDITOR
	PFLOG(Warning, TEXT("Focus Received Inventory Slot Idx : %d"), SlotIdx);
#endif

	return FReply::Unhandled();
}

void SInventorySlot::OnFocusLost(const FFocusEvent& InFocusEvent)
{
#if WITH_EDITOR
	PFLOG(Warning, TEXT("Focus Lost Inventory Slot Idx : %d"), SlotIdx);
#endif

}

bool SInventorySlot::SupportsKeyboardFocus() const
{
	return true;
}

FReply SInventorySlot::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

FReply SInventorySlot::OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Unhandled();
}

FReply SInventorySlot::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply SInventorySlot::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();
	if(!IsEnabled())
	{
		Reply = SWidget::OnKeyDown(MyGeometry, InKeyEvent);
		return MoveTemp(Reply);
	}

	FSlateApplication& SlateApp = FSlateApplication::Get();

	if (SlateApp.GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept)
	{
		if(OnAcceptEvent.IsBound()) 
		{
			Reply = OnAcceptEvent.Execute();
			ensure(Reply.IsEventHandled() == true);
		}
		else 
		{
			Reply = FReply::Handled();
		}
	}
	else if(SlateApp.GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Back)
	{
		if (OnAcceptEvent.IsBound())
		{
			Reply = OnBackEvent.Execute();
			ensure(Reply.IsEventHandled() == true);
		}
		else
		{
			Reply = FReply::Handled();
		}
	}
	else 
	{
		Reply = SWidget::OnKeyDown(MyGeometry, InKeyEvent);
	}

	return MoveTemp(Reply);
}

void SInventorySlot::OnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	PFLOG(Warning, TEXT("%d"), PreviousFocusPath.Widgets.Num());
}

void SInventorySlot::SetAcceptEvent(const TDelegate<FReply()>& NewEvent)
{
	OnAcceptEvent = NewEvent;
}

void SInventorySlot::SetBackEvent(const TDelegate<FReply()>& NewEvent)
{
	OnBackEvent = NewEvent;
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

SLATE_IMPLEMENT_WIDGET(SInventoryWindow)
void SInventoryWindow::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{}

SInventoryWindow::SInventoryWindow() : SlotSize(32)
{
	bInitialized = false;
	SetCanTick(false);
}

SInventoryWindow::~SInventoryWindow()
{
	if(StreamingHandle.IsValid())
	{
		StreamingHandle->CancelHandle();
	}

}


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SInventoryWindow::Construct(const FArguments& InArgs)
{
	FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();

	CountByRow = InArgs._CountByRow;

	SetSlotBackgroundImage(nullptr);
	SetSlotSize(SlotSize);

	ChildSlot
	[
		SNew(SInvalidationPanel)
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
		TSharedPtr<SInventorySlot> TempSlot;
		SAssignNew(TempSlot, SInventorySlot)
			.BackgroundBrush(&SlotBackgroundBrush)
			.SlotIdx(i);

		SlotPanel->AddSlot(i % CountByRow, i / CountByRow)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[TempSlot.ToSharedRef()];
		Slots.Add(TempSlot);
	}

	SlotNavigationBuild();

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



void SInventoryWindow::SetSlotPadding(const FMargin& NewPadding)
{
	SlotPanel->SetSlotPadding(NewPadding);
}

void SInventoryWindow::SlotNavigationBuild()
{
	int SlotsNum = Slots.Num();

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

void SInventoryWindow::SetSlotSize(FVector2D NewSize)
{
	SlotSize = NewSize;
	SlotBackgroundBrush.ImageSize = NewSize;
}

void SInventoryWindow::SetSlotBackgroundImage(TSoftObjectPtr<UMaterialInterface> NewImage)
{
	SlotBackgroundMaterial = NewImage;

	if (NewImage.IsNull())
	{
		FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();

#if WITH_EDITOR
		if (StyleSet != nullptr)
		{
			SlotBackgroundBrush = *StyleSet->GetBrush(InventoryStyle::SlotBackground);
			SetSlotSize(SlotSize);
		}
		else
		{
			FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
			UTexture2D* DefaultTexture = Streamable.LoadSynchronous<UTexture2D>(FSoftObjectPath(FString("/Game/WidgetStyle/Inventory/InventorySlot.InventorySlot")));
			SlotBackgroundBrush.SetResourceObject(DefaultTexture);
		}
#else
		SlotBackgroundBrush = *StyleSet->GetBrush(InventoryStyle::SlotBackground);
		SetSlotSize(SlotSize);
#endif

	}
	else if (NewImage.IsValid())
	{
		SlotBackgroundBrush.SetResourceObject(NewImage.Get());
	}
	else
	{

		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		StreamingHandle = Streamable.RequestAsyncLoad(NewImage.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([&, NewImage]() {
			if (SlotBackgroundMaterial != NewImage) return;

			SlotBackgroundBrush.SetResourceObject(NewImage.Get());
			}));

	}

}

void SInventoryWindow::OnSelectedWindow()
{
	uint32 Test = FSlateApplication::Get().GetUserIndexForKeyboard();

	if(!Slots.IsEmpty()) FSlateApplication::Get().SetUserFocus(Test, Slots[0], EFocusCause::SetDirectly);
}

////////////////////////////////////////////////////////////////////////////////////////////////

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




