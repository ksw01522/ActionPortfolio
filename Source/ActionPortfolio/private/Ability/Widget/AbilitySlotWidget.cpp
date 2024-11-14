// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Widget/AbilitySlotWidget.h"
#include "Ability/Widget/SAbilitySlot.h"
#include "Ability/AbilitySlot.h"

TSharedRef<SWidget> UAbilitySlotWidget::RebuildWidget()
{
	return 
	SAssignNew(SlotSlate, SAbilitySlot)
	.LinkSlot(LinkedSlot.Get())
	.BackgroundBrush(&BackgroundBrush)
	.SlotSize(SlotSize);
}

void UAbilitySlotWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	SlotSlate.Reset();

	Super::ReleaseSlateResources(bReleaseChildren);
}

void UAbilitySlotWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (SlotSlate.IsValid())
	{
		SlotSlate->SetSlotSize(SlotSize);
		SlotSlate->SetAbilityIcon(AbilityIcon);
	}
}

#if WITH_ACCESSIBILITY
TSharedPtr<SWidget> UAbilitySlotWidget::GetAccessibleWidget() const
{
	return SlotSlate;
}

#endif


void UAbilitySlotWidget::LinkAbilitySlot(UAbilitySlot* InSlot)
{
	if(InSlot == LinkedSlot.Get()) return;

	LinkedSlot = InSlot;

	if (SlotSlate.IsValid())
	{
		SlotSlate->LinkAbilitySlot(LinkedSlot.Get());
	}
}

void UAbilitySlotWidget::SetAbilityIcon(const TSharedPtr<SAbilityIcon>& InIcon)
{
	if(AbilityIcon == InIcon) return;

	AbilityIcon = InIcon;

	if (SlotSlate.IsValid())
	{
		SlotSlate->SetAbilityIcon(AbilityIcon);
	}
}

SAbilityIcon* UAbilitySlotWidget::GetAbilityIcon() const
{
	return AbilityIcon.Get();
}


TSharedRef<SWidget> UAbilityHotKeySlotWidget::RebuildWidget()
{
	return
		SAssignNew(SlotSlate, SAbilitySlot_HotKey)
		.BackgroundBrush(GetBackgroundBrush())
		.SlotSize(GetSlotSize())
		.HotKey_Keyboard(HotKey_Keyboard)
		.HotKey_Gamepad(HotKey_Gamepad);
}

void UAbilityHotKeySlotWidget::SetHotKey_Keyboard(const FKey& InKey)
{
	if(HotKey_Keyboard == InKey) return;

	HotKey_Keyboard = InKey;
	if (SlotSlate.IsValid())
	{
		StaticCastSharedPtr<SAbilitySlot_HotKey>(SlotSlate)->SetHotKeyKeyboard(HotKey_Keyboard);
	}
}

void UAbilityHotKeySlotWidget::SetHotKey_Gamepad(const FKey& InKey)
{
	if(HotKey_Gamepad == InKey) return;

	HotKey_Gamepad = InKey;
	if (SlotSlate.IsValid())
	{
		StaticCastSharedPtr<SAbilitySlot_HotKey>(SlotSlate)->SetHotKeyGamepad(HotKey_Gamepad);
	}
}
