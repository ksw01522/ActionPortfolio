// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Widget/InventoryWidget.h"
#include "Items/Widget/SInventorySlate.h"
#include "ActionPortfolio.h"
#include "Items/ItemManagerSubsystem.h"
#include "Items/ItemBase.h"
#include "Character/Player/ActionPFPlayerController.h"
#include "Character/Player/InventoryComponent.h"

UInventoryWidget::UInventoryWidget() : 
                    Preview_InventorySize(64), SlotCountByRow(8),
                    SlotPadding(2)
{}

TSharedRef<SWidget> UInventoryWidget::RebuildWidget()
{
    AActionPFPlayerController* Controller = GetOwningPlayer<AActionPFPlayerController>();

    int InvenCount = Controller != nullptr ? Controller->GetInventory()->GetSlotCount() : 0;
#if WITH_EDITOR
    if (Controller == nullptr) InvenCount = Preview_InventorySize;
#endif

    SAssignNew(InventorySlate, SInventoryWindow)
        .Count(InvenCount)
        .SlotPadding(SlotPadding)
        .CountByRow(SlotCountByRow)
        .SlotBackgroundBrush(&SlotBackgroundBrush);
        

    InventorySlate->GetOnFocusedItemSlot().AddUObject(this, &UInventoryWidget::OnNativeFocusedItemSlot);
    if(Controller) InventorySlate->BindInventorySlot(Controller->GetInventory()->GetSlotArray());

    return InventorySlate.ToSharedRef();
}

void UInventoryWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    InventorySlate.Reset();
}

void UInventoryWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    if (InventorySlate.IsValid())
    {
        AActionPFPlayerController* Controller = GetOwningPlayer<AActionPFPlayerController>();

        int InvenCount = Controller != nullptr ? Controller->GetInventory()->GetSlotCount() : 0;
#if WITH_EDITOR
        if (Controller == nullptr) InvenCount = Preview_InventorySize;
#endif

        InventorySlate->SetSlotPadding(SlotPadding);

        InventorySlate->SetSlotBackgroundBrush(&SlotBackgroundBrush);
        InventorySlate->SetSlotCountByRow(SlotCountByRow);
    }
}

#if WITH_EDITOR
const FText UInventoryWidget::GetPaletteCategory()
{
    return NSLOCTEXT("ActionPortfolio", "InventoryCategory", "Inventory");
}
#endif


#if WITH_ACCESSIBILITY

TSharedPtr<SWidget> UInventoryWidget::GetAccessibleWidget() const
{
    return InventorySlate;
}

#endif



void UInventoryWidget::OnNativeFocusedItemSlot(UItemSlot* InSlot)
{
    if(OnFocusedItemSlot.IsBound()) OnFocusedItemSlot.Broadcast(InSlot);
}

void UInventoryWidget::SetSlotCountByRow(int NewCount)
{
    if(SlotCountByRow == NewCount) return;
    SlotCountByRow = NewCount;
    if (InventorySlate.IsValid())
    {
        InventorySlate->SetSlotCountByRow(NewCount);
    }
}

void UInventoryWidget::BindInventorySlots(TArray<UInventorySlot*> InventorySlots)
{
    InSlots = InventorySlots;

    if (InventorySlate.IsValid())
    {
        InventorySlate->BindInventorySlot(InSlots);
    }
}
