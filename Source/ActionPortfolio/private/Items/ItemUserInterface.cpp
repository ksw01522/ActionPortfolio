// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemUserInterface.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Items/ItemBase.h"
#include "Items/Euipment/EquipmentAbility.h"
#include "ActionPortfolio.h"

// Add default functionality here for any IItemUserInterface functions that are not pure virtual.

bool IItemUserInterface::CanEquipItem(UItemBase_Equipment* NewItem) const
{
	if(!IsValid(NewItem)) return false;

	bool bCanEquip = NewItem->CanEquipItem(this);

	return bCanEquip;
}

bool IItemUserInterface::EquipItem(UItemBase_Equipment* NewItem)
{
	if(!CanEquipItem(NewItem)) return false;

	bool bResult = NewItem->TryEquipItem(this);

	if(bResult) OnEquipItem(NewItem);

	return bResult;
}

bool IItemUserInterface::CanUnequipItem(UItemBase_Equipment* NewItem) const
{
	if (!IsValid(NewItem)) return false;

	bool bCanUnequip = NewItem->CanUnequipItem(this);

	return bCanUnequip;
}

bool IItemUserInterface::UnequipItem(UItemBase_Equipment* NewItem)
{
	if (!CanUnequipItem(NewItem)) return false;

	bool bResult = NewItem->TryUnequipItem(this);

	return false;
}
