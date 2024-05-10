// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Widget/SItemSlot.h"
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
#include "Items/Slot/ItemSlot.h"

SLATE_IMPLEMENT_WIDGET(SItemSlot)
void SItemSlot::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{


}

SItemSlot::SItemSlot()
{
	SetCanTick(false);

	BindedSlot = nullptr;
}

SItemSlot::~SItemSlot()
{	
	BindItemSlot(nullptr);
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SItemSlot::Construct(const FArguments& InArgs)
{
	BindedSlot = InArgs._ItemSlot;

	FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();
	const FSlateBrush* SlotBackgroundBrush = InArgs._BackgroundBrush;

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
					.ForceVolatile(true)
			]
			+ SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SAssignNew(FrameImage, SImage)
					.Visibility(EVisibility::Collapsed)
					.ForceVolatile(true)
			]
			+ SOverlay::Slot()
			.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Right)
			.Padding(FMargin(0, 0, 5, 5))
			[
				SAssignNew(CountBlock, STextBlock)
					.Visibility(EVisibility::Collapsed)
					.TextStyle(CountBlockStyle)
					.ForceVolatile(true)
			]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SItemSlot::BindItemSlot(UItemSlot* InSlot)
{
	if(InSlot == BindedSlot) return;

	if (BindedSlot.IsValid())
	{
		BindedSlot->GetOnChagnedItemInSlot().Remove(OnChangedSlotHandle);
		ClearSlotSlate();
	}

	BindedSlot = InSlot;

	if (BindedSlot.IsValid())
	{
		TWeakPtr<SItemSlot> WeakSlot = StaticCastWeakPtr<SItemSlot>(AsWeak());

		OnChangedSlotHandle = BindedSlot->GetOnChagnedItemInSlot().AddLambda([WeakSlot](UItemSlot* InSlot, UItemBase* PrevItem){
							if(!WeakSlot.IsValid()) return;
							WeakSlot.Pin()->UpdateSlotSlate(InSlot->GetItem());
						});
		
		UpdateSlotSlate(BindedSlot->GetItem());
	}
	else
	{
		ClearSlotSlate();
	}
}

void SItemSlot::SetBackgroundBrush(const FSlateBrush* InBrush)
{
	BackgroundImage->SetImage(InBrush);
}

void SItemSlot::SetCount(int NewCount)
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

void SItemSlot::SetItemIcon(TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade)
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

void SItemSlot::SetIconFrame(EItemGrade ItemGrade)
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

FReply SItemSlot::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	if(OnFocusedItemSlot.IsBound()) OnFocusedItemSlot.Broadcast(BindedSlot.Get());

	return FReply::Handled();
}

void SItemSlot::OnFocusLost(const FFocusEvent& InFocusEvent)
{

}

bool SItemSlot::SupportsKeyboardFocus() const
{
	return true;
}

FReply SItemSlot::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
#if WITH_EDITOR
	PFLOG(Warning, TEXT("On MouseButtonDown Item Slot"));
#endif

#if WITH_EDITOR
	if(!BindedSlot.IsValid()) {return FReply::Unhandled();}
#endif
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && BindedSlot.IsValid() && BindedSlot->GetItem() != nullptr)
	{
		TSharedRef<FItemSlotDragDropOperation> DDO = FItemSlotDragDropOperation::ItemSlotDrag(this);
		return FReply::Handled().BeginDragDrop(DDO);
	}

	return FReply::Handled();
}

FReply SItemSlot::OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Unhandled();
}

FReply SItemSlot::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply SItemSlot::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FSlateApplication& SlateApp = FSlateApplication::Get();

	if (SlateApp.GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept)
	{
		if(OnAcceptEvent.IsBound()) 
		{
			return OnAcceptEvent.Execute();
		}
		else 
		{
			return FReply::Handled();
		}
	}

	EUINavigation NavDirection = SlateApp.GetNavigationDirectionFromKey(InKeyEvent);
	if(NavDirection != EUINavigation::Invalid)
	{
		ENavigationGenesis Genesis = InKeyEvent.GetKey().IsGamepadKey() ? ENavigationGenesis::Controller : ENavigationGenesis::Keyboard;
		return FReply::Handled().SetNavigation(NavDirection, Genesis);
	}

	return FReply::Unhandled();
}


FReply SItemSlot::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

void SItemSlot::OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
}

void SItemSlot::OnDragLeave(const FDragDropEvent& DragDropEvent)
{

}

FReply SItemSlot::OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{

	return FReply::Handled();
}

FReply SItemSlot::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
#if WITH_EDITOR
	check(BindedSlot.IsValid());
	PFLOG(Warning, TEXT("On Drop Item Slot"));
#endif
	TSharedPtr<FItemSlotDragDropOperation> ISDDO = DragDropEvent.GetOperationAs<FItemSlotDragDropOperation>();
	if (ISDDO.IsValid())
	{
		ISDDO->SetToSlot(BindedSlot.Get());
	}

	return FReply::Handled();
}


//////////////////////////////

void SItemSlot::SetAcceptEvent(const TDelegate<FReply()>& NewEvent)
{
	OnAcceptEvent = NewEvent;
}

void SItemSlot::UpdateSlotSlate(const UItemBase* InItem)
{
	if (InItem != nullptr)
	{
		int Count = InItem->GetCount();
		TSoftObjectPtr<UMaterialInterface> IconMat = InItem->GetIconMaterial();
		EItemGrade Grade = InItem->GetItemGrade();

		UpdateSlotSlate(IconMat, Grade, Count);
	}
	else
	{
		ClearSlotSlate();
	}
}


void SItemSlot::UpdateSlotSlate(TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount)
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

void SItemSlot::ClearSlotSlate()
{
	if(IconMaterial.IsNull()) return;

	IconImage->SetVisibility(EVisibility::Collapsed);
	FrameImage->SetVisibility(EVisibility::Collapsed);
	CountBlock->SetVisibility(EVisibility::Collapsed);

	IconMaterial.Reset();
	IconBrush.SetResourceObject(nullptr);

	CountBlock->SetText(FText::GetEmpty());
}



FItemSlotDragDropOperation::FItemSlotDragDropOperation(UItemSlot* InFromSlot) : FromSlot(InFromSlot)
{
	IconBrush = FSlateImageBrush(InFromSlot->GetItem()->GetIconMaterial().Get(), FVector2D(32));
}

void FItemSlotDragDropOperation::Construct()
{
	FDragDropOperation::Construct();
}

void FItemSlotDragDropOperation::OnDragged(const FDragDropEvent& DragDropEvent)
{
	FDragDropOperation::OnDragged(DragDropEvent);
}

void FItemSlotDragDropOperation::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
	DestroyCursorDecoratorWindow();

	if (FromSlot != nullptr)
	{
		FromSlot->SlotDropTo(ToSlot.Get());
	}
}

TSharedPtr<SWidget> FItemSlotDragDropOperation::GetDefaultDecorator() const
{
	return SNew(SImage)
			.Image(&IconBrush)
			.Visibility(EVisibility::HitTestInvisible);
}

void FItemSlotDragDropOperation::SetFromSlot(UItemSlot* InSlot)
{
	FromSlot =InSlot;
}

void FItemSlotDragDropOperation::SetToSlot(UItemSlot* InSlot)
{
	ToSlot = InSlot;
}

TSharedRef<FItemSlotDragDropOperation> FItemSlotDragDropOperation::ItemSlotDrag(SItemSlot* InFromSlot)
{
	TSharedRef<FItemSlotDragDropOperation> ReturnDDO = MakeShared<FItemSlotDragDropOperation>(InFromSlot->BindedSlot.Get());	
	ReturnDDO->Construct();

	return MoveTemp(ReturnDDO);
}

