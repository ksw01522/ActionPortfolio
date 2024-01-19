// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Widget/InventoryWidget.h"
#include "Items/Widget/SInventorySlate.h"
#include "ActionPortfolio.h"
#include "Items/ItemManagerSubsystem.h"
#include "Items/ItemBase.h"

UInventoryWidget::UInventoryWidget()
{}

TSharedRef<SWidget> UInventoryWidget::RebuildWidget()
{
    InventorySlate = SNew(SInventory);

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
        UItemManagerSubsystem* ManagerInstance = UItemManagerSubsystem::GetManagerInstance();
        int InventorySize = 0;

        InventorySlate->SetInnerSlotPadding(InnerSlotPadding);

        InventorySlate->SetWindowPadding(WindowPadding);

        InventorySlate->SetSlotBackgroundImage(EItemType::Equipment , SlotBackgroundMaterial_Equip);
        InventorySlate->SetSlotBackgroundImage(EItemType::Consumption , SlotBackgroundMaterial_Consum);
        InventorySlate->SetSlotBackgroundImage(EItemType::Material , SlotBackgroundMaterial_Mat);

#if WITH_EDITOR
        if (ManagerInstance)
        {
            InventorySize = ManagerInstance->GetInventorySize();
        }
        else
        {
            InventorySize = Preview_InventorySize;

        }
#else
        InventorySize = ManagerInstance->GetInventorySize();
#endif

        InventorySlate->InitializeInventoryWindow(InventorySize, true);
    }
}

#if WITH_EDITOR
void UInventoryWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

}

const FText UInventoryWidget::GetPaletteCategory()
{
    return NSLOCTEXT("ActionPortfolio", "ActionPFWidgetCategory", "ActionPF");
}
#endif


#if WITH_ACCESSIBILITY

TSharedPtr<SWidget> UInventoryWidget::GetAccessibleWidget() const
{
    return InventorySlate;
}

#endif

void UInventoryWidget::UpdateSlotWidget(EItemType InventoryType, int idx, TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount)
{
    if (InventorySlate.IsValid())
    {
        InventorySlate->UpdateSlotWidget(InventoryType, idx, NewImage, ItemGrade, NewCount);
    }
}