// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilitySlot.h"
#include "Ability/Widget/SAbilitySlot.h"
#include "Ability/Widget/SAbilityIcon.h"
#include "ActionPortfolio.h"

void UAbilitySlot::ChangeSlotWithOther(UAbilitySlot* Other)
{
	TempAbilityClass = Other->GetAbilityClass();
}

bool UAbilitySlot::CanChangeSlotFrom(UAbilitySlot* From) const
{
	if(From == nullptr) return false;

	return true;
}

bool UAbilitySlot::CanChangeSlotTo(UAbilitySlot* To) const
{
	if (To == nullptr) return false;

	return true;
}

void UAbilitySlot::ApplyChangeSlot()
{
	SetAbilityClass(TempAbilityClass);
	TempAbilityClass = nullptr;
	
}

void UAbilitySlot::LinkAbilitySlotSlate(const TSharedPtr<SAbilitySlot>& InSlot)
{
	if(SlotSlate == InSlot) return;
	if (SlotSlate.IsValid())
	{
		SlotSlate.Pin()->UnlinkOwnerSlot(this);
	}

	SlotSlate = InSlot;
	if (SlotSlate.IsValid())
	{
		SlotSlate.Pin()->LinkOwnerSlot(this);
		if (AbilityClass.GetDefaultObject() != nullptr)
		{
			BringAbilityIcon();
		}
		else
		{
			SlotSlate.Pin()->SetAbilityIcon(nullptr);
		}
	}
}

void UAbilitySlot::ClearSlot()
{
	TempAbilityClass = nullptr;

	ApplyChangeSlot();
}



bool UAbilitySlot::PreSetAbilityClass(TSubclassOf<class UActionPFGameplayAbility> InClass)
{
	return true;
}

void UAbilitySlot::SetAbilityClass(TSubclassOf<class UActionPFGameplayAbility> InClass)
{
	if(!PreSetAbilityClass(InClass)) return;

	AbilityClass = InClass;

	if (SlotSlate.IsValid())
	{
		if (AbilityClass.GetDefaultObject() != nullptr)
		{
			BringAbilityIcon();
		}
		else
		{
			SlotSlate.Pin()->SetAbilityIcon(nullptr);
		}
	}

	PostSetAbilityClass();
}


void UAbilitySlot::BringAbilityIcon()
{
	
}