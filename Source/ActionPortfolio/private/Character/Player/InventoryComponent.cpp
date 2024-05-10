// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/InventoryComponent.h"
#include "Items/ItemBase.h"
#include "Items/ItemManagerSubsystem.h"
#include "ActionPortfolio.h"
#include "Items/Widget/SInventorySlate.h"
#include "Items/ItemWorldSubsystem.h"


////////////////////// For InventoryComponent ///////////////////////////////////////

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	//당장은 쓸 거 같진 않다.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UItemManagerSubsystem* ManagerSubsystem = UItemManagerSubsystem::GetManagerInstance();
	const int InventorySize = ManagerSubsystem->GetInventorySize();

	Slots.Reserve(InventorySize);
	for (int i = 0; i < InventorySize; i++)
	{
		Slots.Add(NewObject<UInventorySlot>(this));
	}
}

void UInventoryComponent::AddItemByDropItem(ADroppedItem& DropItem)
{
	UItemBase* DroppedItem = const_cast<UItemBase*>(DropItem.GetDroppedItem());
	if (DroppedItem == nullptr)
	{
		PFLOG(Error, TEXT("Try Add Empty Drop Item."));
		return;
	}

	int TempCount = AddItemByObject(DroppedItem);

	if(TempCount == 0) { DropItem.ClearDroppedItemData(); }
}

int UInventoryComponent::AddItemByCode(const FName& ItemCode, int Count)
{
	if(Count <= 0) return Count;

	UItemManagerSubsystem* ManagerInstance = UItemManagerSubsystem::GetManagerInstance();
	if (!ManagerInstance) return Count;
	
	UItemBase* ItemData = ManagerInstance->MakeItemInstance(ItemCode, Count);
	if (ItemData == nullptr)
	{
		return Count;
	}

	return AddItemByObject(ItemData);
}

int UInventoryComponent::AddItemByObject(UItemBase* TargetItem)
{
	check(TargetItem);

	//Item이 Stackable이면서 인벤토리에 같은 아이템이 있을 시 Count 증가
	if (TargetItem->IsStackableItem())
	{
		int BeforeTargetSlotCount = 0;
		int AfterTargetSlotCount = 0;

		int SlotCount = Slots.Num();
		for(int i = 0; i < SlotCount && 0 < TargetItem->GetCount(); i++)
		{
			if(Slots[i]->GetItem() == nullptr) continue;

			if (Slots[i]->GetItem()->CanStackableWithOther(TargetItem))
			{
				BeforeTargetSlotCount = Slots[i]->GetItemCount();

				Slots[i]->AddCount(TargetItem->GetCount());

				AfterTargetSlotCount = Slots[i]->GetItemCount();

				TargetItem->RemoveCount( AfterTargetSlotCount - BeforeTargetSlotCount);

				if (TargetItem->GetCount() <= 0) { return 0; }
			}
		}
	}


	int SlotCount = Slots.Num();
	for (int i = 0; i < SlotCount; i++)
	{
		if(Slots[i]->GetItem() != nullptr) continue;

		Slots[i]->SetItem(TargetItem);
		
		return 0;
	}

	return TargetItem->GetCount();
}

int UInventoryComponent::AddItemByIndex(int Idx, int Count)
{
	check(Slots.IsValidIndex(Idx));

	int RemainCount = Count;

	int BeforeCount = Slots[Idx]->GetItemCount();

	Slots[Idx]->AddCount(RemainCount);

	int AfterCount = Slots[Idx]->GetItemCount();

	RemainCount -= AfterCount - BeforeCount;

	return RemainCount;
}

int UInventoryComponent::RemoveItemByCode(const FName& ItemCode, int Count)
{
	UItemManagerSubsystem* ManagerInstance = UItemManagerSubsystem::GetManagerInstance();
	if (!ManagerInstance) return Count;

	UItemBase* TargetItem = ManagerInstance->MakeItemInstance(ItemCode);
	TargetItem->SetCount(Count);
	if (TargetItem == nullptr)
	{
		PFLOG(Error, TEXT("Can't Item Data By : %s"), *ItemCode.ToString());
		return Count;
	}

	return RemoveItemByObject(TargetItem);
}

int UInventoryComponent::RemoveItemByObject(UItemBase* TargetItem)
{
	int RemainCount = TargetItem->GetCount();
	int BeforeTargetSlotCount, AfterTargetSlotCount;

	int SlotCount = Slots.Num();
	for (int i = 0; i < SlotCount && 0 < RemainCount; i++)
	{
		if(Slots[i]->IsEmptySlot() || !Slots[i]->GetItem()->IsSame(TargetItem)) continue;

		BeforeTargetSlotCount = Slots[i]->GetItemCount();

		Slots[i]->RemoveCount(RemainCount);

		AfterTargetSlotCount = Slots[i]->GetItemCount();

		RemainCount += AfterTargetSlotCount - BeforeTargetSlotCount;

		if (RemainCount <= 0) { return 0; }
	}

	return RemainCount;
}

int UInventoryComponent::RemoveItemByIndex(int Idx, int Count)
{
	check(0 < Count)
	check(Slots.IsValidIndex(Idx));

	int RemainCount = Count;

	int BeforeCount = Slots[Idx]->GetItemCount();

	Slots[Idx]->RemoveCount(RemainCount);

	int AfterCount = Slots[Idx]->GetItemCount();

	RemainCount += AfterCount - BeforeCount;

	return RemainCount;
}

bool UInventoryComponent::TryDropItem(int Idx, int Count, const FVector& DropLocation)
{
	check(0 < Count)
	check(Slots.IsValidIndex(Idx));
	checkf(Count <= Slots[Idx]->GetItemCount(), TEXT("Call Try Drop Item Over Count %d Over Item Count %d"), Count, Slots[Idx]->GetItemCount());
	
	UItemBase* ItemForDrop = Slots[Idx]->GetItem();
	check(ItemForDrop);

	UItemWorldSubsystem* IWS = GetWorld()->GetSubsystem<UItemWorldSubsystem>();
	check(IWS);

	Slots[Idx]->RemoveCount(Count);
	if (Slots[Idx]->GetItem() != nullptr)
	{
		IWS->ItemDrop(ItemForDrop->GetItemCode(), DropLocation, Count);
	}
	else
	{
		IWS->ItemDrop(ItemForDrop, DropLocation);
	}

	return true;
}



UInventorySlot* UInventoryComponent::GetInventorySlot(int Idx) const
{
	if(!Slots.IsValidIndex(Idx)) return nullptr;
	return Slots[Idx];
}

UInventorySlot* UInventoryComponent::GetEmptySlot(int StartIdx) const
{
	if (!Slots.IsValidIndex(StartIdx)) return nullptr;

	int SlotCount = Slots.Num();
	for (int i = StartIdx; i < SlotCount; i++)
	{
		if(Slots[i]->IsEmptySlot()) return Slots[i];
	}

	return nullptr;
}

int UInventoryComponent::GetCountItemInInventory(const FName& ItemCode)
{
	if(ItemCode.IsNone()) return 0;

	int SumCount = 0;

	int SlotCount = Slots.Num();
	for (int i = 0; i < SlotCount; i++)
	{
		if(Slots[i]->IsEmptySlot()) continue;

		if (Slots[i]->GetItem()->GetItemCode() == ItemCode)
		{
			SumCount += Slots[i]->GetItemCount();
		}
	}

	return SumCount;
}

void UInventorySlot::SlotDropTo(UItemSlot* To)
{
	if(To == nullptr) {return;}
	else if (To->GetSlotType() == "Inventory" || To->GetSlotType() == "Equipment")
	{
		if (CanItemInSlot(To->GetItem()) && To->CanItemInSlot(GetItem()))
		{
			UItemBase* ToItem = To->GetItem();
			To->SetItem(GetItem());
			SetItem(ToItem);
		}
	}
}
