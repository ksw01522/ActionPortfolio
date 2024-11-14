// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilitySlot.h"
#include "Ability/Widget/SAbilitySlot.h"
#include "Ability/Widget/SAbilityIcon.h"
#include "ActionPortfolio.h"


UAbilitySlot::UAbilitySlot() : SlotType()
{
}

UAbilitySlot::UAbilitySlot(FName InSlotType) : SlotType(InSlotType)
{
}

bool UAbilitySlot::CanSetAbilityClass(TSubclassOf<class UActionPFGameplayAbility> InClass)
{
	return AbilityClass != InClass;
}

void UAbilitySlot::SetAbilityClass(TSubclassOf<class UActionPFGameplayAbility> InClass)
{
	if(!CanSetAbilityClass(InClass)) return;

	AbilityClass = InClass;

	if (OnSetAbilityDel.IsBound())
	{
		OnSetAbilityDel.Broadcast(this);
	}

	PostSetAbilityClass();
}
