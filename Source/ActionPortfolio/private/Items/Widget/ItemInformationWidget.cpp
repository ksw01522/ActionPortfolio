// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Widget/ItemInformationWidget.h"
#include "Items/Widget/SItemInformationSlate.h"
#include "Items/ItemWorldSubsystem.h"
#include "ActionPortfolio.h"

TSharedRef<SWidget> UItemInformationWidget::RebuildWidget()
{
    return SAssignNew(Slate, SItemInformationSlate)
                .MeshCaptureBrush(&MeshCaptureBrush)
                .NameStyle(&NameStyle)
                .DescriptionStyle(&DescriptionStyle);
}

void UItemInformationWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);

    Slate.Reset();
}

void UItemInformationWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    if (Slate.IsValid())
    {
        Slate->SetMeshCaptureBrush(&MeshCaptureBrush);
        Slate->SetNameStyle(&NameStyle);
        Slate->SetDescriptionStyle(&DescriptionStyle);
    }
}


#if WITH_EDITOR

const FText UItemInformationWidget::GetPaletteCategory()
{
    return NSLOCTEXT("ActionPortfolio", "InventoryCategory", "Inventory");
}

#endif

#if WITH_ACCESSIBILITY
TSharedPtr<SWidget> UItemInformationWidget::GetAccessibleWidget() const
{
    return Slate;
}

#endif

void UItemInformationWidget::UpdateItemInformation(const UItemBase* InItem)
{
    if (Slate.IsValid())
    {
        Slate->UpdateInformation(InItem);
    }
}