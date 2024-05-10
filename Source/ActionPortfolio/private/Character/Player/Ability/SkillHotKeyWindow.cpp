// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Ability/SkillHotKeyWindow.h"
#include "Ability/Widget/AbilitySlotWidget.h"
#include "ActionPortfolio.h"
#include "Character/Player/PlayerCharacter.h"

void USkillHotKeyWindow::NativeConstruct()
{
	Super::NativeConstruct();

	if(OwnerCharacter.IsValid()) OwnerCharacter->LinkSkillHotKeyWindow(this);
}


void USkillHotKeyWindow::SetAbilitySlots(TMap<int, UAbilityHotKeySlotWidget*> InSlots)
{
	AbilitySlots = InSlots;
}

UAbilityHotKeySlotWidget* USkillHotKeyWindow::GetAbilitySlot(int InputID)
{
	if(!AbilitySlots.Contains(InputID)) return nullptr;
	return AbilitySlots.FindRef(InputID);
}

void USkillHotKeyWindow::ClearWindow()
{
	for (auto& AbilitySlot : AbilitySlots)
	{
		if (AbilitySlot.Value != nullptr)
		{
			AbilitySlot.Value->SetAbilityIcon(nullptr);
		}
	}
}

void USkillHotKeyWindow::SetAbilityIcon(int Idx, const TSharedPtr<class SAbilityIcon>& InIcon)
{
	if (UAbilitySlotWidget* AbilitySlot = GetAbilitySlot(Idx))
	{
		AbilitySlot->SetAbilityIcon(InIcon);
	}
}

void USkillHotKeyWindow::SetOwnerCharacter(APlayerCharacter* InOwner)
{
	OwnerCharacter = InOwner;
}
