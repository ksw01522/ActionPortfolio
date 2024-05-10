// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Widget/AbilitySlotWidget.h"
#include "Ability/Widget/SAbilitySlot.h"

TSharedRef<SWidget> UAbilitySlotWidget::RebuildWidget()
{
	return 
	SAssignNew(AbilitySlot, SAbilitySlot)
	.BackgroundBrush(&BackgroundBrush)
	.SlotSize(SlotSize);
}

void UAbilitySlotWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	AbilitySlot.Reset();
}

void UAbilitySlotWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (AbilitySlot.IsValid())
	{
		AbilitySlot->SetSlotSize(SlotSize);
	}
}

#if WITH_ACCESSIBILITY
TSharedPtr<SWidget> UAbilitySlotWidget::GetAccessibleWidget() const
{
	return AbilitySlot;
}

#endif

void UAbilitySlotWidget::SetAbilityIcon(const TSharedPtr<class SAbilityIcon>& InIcon)
{
	if (AbilitySlot.IsValid())
	{
		AbilitySlot->SetAbilityIcon(InIcon);
	}
}

TSharedRef<SWidget> UAbilityHotKeySlotWidget::RebuildWidget()
{
	return 
	SAssignNew(AbilitySlot, SAbilitySlot_HotKey)
	.BackgroundBrush(GetBackgroundBrush())
	.SlotSize(GetSlotSize());
}
