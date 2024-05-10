// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Slot/ItemSlot.h"
#include "Items/ItemBase.h"
#include "ActionPortfolio.h"

UItemSlot::UItemSlot() : Item(nullptr), SlotType(NAME_None)
{}

UItemSlot::UItemSlot(FName InSlotType) : SlotType(InSlotType)
{}


void UItemSlot::NativeOnChangedItemInSlot(UItemBase* PrevItem)
{
	if (OnChangedItemInSlot.IsBound()) OnChangedItemInSlot.Broadcast(this, PrevItem);
}

int UItemSlot::GetItemCount() const
{
	if(Item == nullptr) return 0;

	return Item->GetCount();
}

void UItemSlot::SetItem(UItemBase* InItem)
{
	if(Item == InItem || !CanItemInSlot(InItem)) return;

	UItemBase* PrevItem = Item;
	Item = InItem;

	NativeOnChangedItemInSlot(PrevItem);
}

void UItemSlot::AddCount(int NewCount)
{
	check(Item);
	Item->AddCount(NewCount);
}

void UItemSlot::RemoveCount(int NewCount)
{
	check(Item);
	Item->RemoveCount(NewCount);

	if (Item->GetCount() == 0)
	{
		Item = nullptr;
	}
}
