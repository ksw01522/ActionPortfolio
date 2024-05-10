// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Widget/CharacterStatusWidget.h"
#include "Character/Player/Widget/SCharacterStatusSlate.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Character/CharacterStatusComponent.h"

UCharacterStatusWidget::UCharacterStatusWidget() : BindedStatus(nullptr)
{
}

TSharedRef<SWidget> UCharacterStatusWidget::RebuildWidget()
{
	SAssignNew(Slate, SCharacterStatusSlate)
		.SlotBackgroundBrush(&SlotBackgroundBrush)
		.SlotPadding(SlotPadding)
		.TextStyle(&TextStyle)
		.TextBlockPadding(TextBlockPadding)
		.BindStatus(BindedStatus.Get());

	Slate->GetOnFocusedEquipmentSlot().AddUObject(this, &UCharacterStatusWidget::NativeOnFocusedEuipmentSlot);

	return Slate.ToSharedRef();
}

void UCharacterStatusWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Slate.Reset();
}

void UCharacterStatusWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (Slate.IsValid())
	{
		Slate->SetSlotBackgroundBrush(&SlotBackgroundBrush);
		Slate->SetSlotPadding(SlotPadding);
		Slate->SetTextStyle(&TextStyle);
		Slate->SetTextBlockPadding(TextBlockPadding);
	}
}


#if WITH_EDITOR
const FText UCharacterStatusWidget::GetPaletteCategory()
{
	return NSLOCTEXT("ActionPortfolio", "CharacterStatus", "CharacterStatus");
}

#endif

#if WITH_ACCESSIBILITY
TSharedPtr<SWidget> UCharacterStatusWidget::GetAccessibleWidget() const
{
	return Slate;
}

#endif

void UCharacterStatusWidget::NativeOnFocusedEuipmentSlot(UItemSlot* FocusedSlot)
{
	if(OnFocusedEqipmentSlot.IsBound()) OnFocusedEqipmentSlot.Broadcast(FocusedSlot);
}

void UCharacterStatusWidget::BindStatusComponent(UCharacterStatusComponent* TargetStatus)
{
	BindedStatus = TargetStatus;
	if (Slate.IsValid())
	{
		Slate->BindStatusComponent(BindedStatus.Get());
	}
}

