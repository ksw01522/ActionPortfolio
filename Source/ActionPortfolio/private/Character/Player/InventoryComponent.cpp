// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/InventoryComponent.h"
#include "Items/ItemBase.h"
#include "Items/ItemManagerSubsystem.h"
#include "ActionPortfolio.h"




//////////////// For Inventory Slot /////////////////////////////
FName FInventorySlot::GetItemCode() const
{
	return Item == nullptr ? NAME_None : Item->GetItemCode();
}

void FInventorySlot::SetSlot(UItemBase* NewItem, int NewCount)
{
#if WITH_EDITOR
	FString ItemCode = IsValid(NewItem) ? NewItem->GetItemCode().ToString() : "nullptr";
	PFLOG(Warning, TEXT("Try Set Slot By Code : %s, Count : %d"), *ItemCode, NewCount);
#endif

	if (!IsValid(NewItem) || NewCount <= 0)
	{
		ClearSlot();
		return;
	}

	Item = NewItem;
	SetCount(NewCount);
}

void FInventorySlot::SetCount(int NewCount)
{
	Count = NewCount;
	CheckCount();
}

void FInventorySlot::AddCount(int AddCount)
{
	if (IsEmpty())
	{
		PFLOG(Error, TEXT("Try Add Count To EmptySlot."));
		return;
	}

	if (!Item->IsStackableItem())
	{
		PFLOG(Error, TEXT("Try Add Count To Not Stackable Item"));
		return;
	}

	Count += AddCount;

	CheckCount();
}

void FInventorySlot::RemoveCount(int RemoveCount)
{
	if (IsEmpty())
	{
		PFLOG(Error, TEXT("Try Remove Count To EmptySlot."));
		return;
	}

	Count -= RemoveCount;

	CheckCount();
}

void FInventorySlot::CheckCount()
{
	if (IsEmpty())
	{
		ClearSlot();
		return;
	}
	
	if (Item->IsStackableItem())
	{
		Count = FMath::Clamp(Count, 1, Item->GetStackSize());
	}
	else
	{
		Count = 1;
	}
}

bool FInventorySlot::IsEmpty() const
{
	return !IsValid(Item) || Count <= 0;
}





////////////////////// For InventorySlotContainer ///////////////////////////////////////

void FInventorySlotContainer::InitializeSlots(int NewSize)
{
	Slots.Reset();
	Slots.Init(FInventorySlot(), NewSize);

	for (int i = 0; i < Slots.Num(); i++)
	{
		Slots[i].Index = i;
	}
}

FInventorySlot* FInventorySlotContainer::GetEmptySlot(int StartIndex)
{
	FInventorySlot* ReturnSlot = nullptr;

	int Length = Slots.Num();
	for (int i = StartIndex; i < Length; i++)
	{
		if (Slots[i].IsEmpty())
		{
			ReturnSlot = &Slots[i];
			break;
		}
	}

	return ReturnSlot;
}

FInventorySlot* FInventorySlotContainer::FindSlotByCode(const FName& ItemCode, int StartIndex)
{
	FInventorySlot* ReturnSlot = nullptr;

	int Length = Slots.Num();
	for (int i = StartIndex; i < Length; i++)
	{
		if (Slots[i].GetItemCode() == ItemCode)
		{
			ReturnSlot = &Slots[i];
			break;
		}
	}

	return ReturnSlot;
}



////////////////////// For InventoryComponent ///////////////////////////////////////

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	//당장은 쓸 거 같진 않다.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UEnum* EnumClass = StaticEnum<EItemType>();
	Inventory.Reserve(EnumClass->NumEnums() - 2);

	
	UItemManagerSubsystem* ManagerSubsystem = UItemManagerSubsystem::GetManagerInstance();
	const int InventorySize = ManagerSubsystem->GetInventorySize();

	//Inventory칸 초기화
	for (EItemType ItemType : TEnumRange<EItemType>())
	{
		Inventory.Add(ItemType, FInventorySlotContainer()).InitializeSlots(InventorySize);
	}
	
}

void UInventoryComponent::AddItemByDropItem(ADroppedItem& DropItem)
{
	const UItemBase* DroppedItem = DropItem.GetDroppedItem();
	if (DroppedItem == nullptr)
	{
		PFLOG(Error, TEXT("Try Add Empty Drop Item."));
		return;
	}

	int TempCount = DropItem.GetItemCount();

	TempCount = AddItemByCode(DroppedItem->GetItemCode(), TempCount);

	DropItem.SetItemCount(TempCount);
}

int UInventoryComponent::AddItemByCode(const FName& ItemCode, int Count)
{
	if(Count <= 0) return Count;

	UItemManagerSubsystem* ManagerInstance = UItemManagerSubsystem::GetManagerInstance();
	if (!ManagerInstance) return Count;
	
	UItemBase* ItemData = ManagerInstance->MakeItemInstance(ItemCode);
	if (ItemData == nullptr)
	{
		return Count;
	}

	return AddItemByObject(ItemData, Count);
}

int UInventoryComponent::AddItemByObject(UItemBase* TargetItem, int Count)
{
	if(TargetItem == nullptr || Count <= 0) return Count;

	FInventorySlotContainer& TargetContainer = Inventory[TargetItem->GetItemType()];
	FInventorySlot* TargetSlot = TargetContainer.FindSlotByCode(TargetItem->GetItemCode());

	int RemainCount = Count;
	int BeforeTargetSlotCount, AfterTargetSlotCount;

	//Item이 Stackable이면서 인벤토리에 같은 아이템이 있을 시 Count 증가
	if (TargetItem->IsStackableItem())
	{
		while (TargetSlot != nullptr && 0 < Count)
		{
			if (TargetSlot->GetItemInSlot()->CanStackableWithOther(TargetItem))
			{
				BeforeTargetSlotCount = TargetSlot->GetCount();

				TargetSlot->AddCount(Count);

				AfterTargetSlotCount = TargetSlot->GetCount();

				if (OnChangedInventory.IsBound() && BeforeTargetSlotCount != AfterTargetSlotCount) {
					OnChangedInventory.Broadcast(TargetItem->GetItemType(), TargetSlot->GetIndex());
				}

				RemainCount -= AfterTargetSlotCount - BeforeTargetSlotCount;

				if (RemainCount <= 0) { return RemainCount; }
			}
			TargetSlot = TargetContainer.FindSlotByCode(TargetItem->GetItemCode(), TargetSlot->GetIndex() + 1);
		}
	}

	//빈 인벤토리 공간에 넣어야 한다면 
	TargetSlot = TargetContainer.GetEmptySlot();

	while (TargetSlot != nullptr && 0 < Count)
	{
		BeforeTargetSlotCount = 0;

		TargetSlot->SetSlot(TargetItem, Count);

		AfterTargetSlotCount = TargetSlot->GetCount();

		if (OnChangedInventory.IsBound() && BeforeTargetSlotCount != AfterTargetSlotCount) {
			OnChangedInventory.Broadcast(TargetItem->GetItemType(), TargetSlot->GetIndex());
		}

		RemainCount -= AfterTargetSlotCount - BeforeTargetSlotCount;
		if (RemainCount <= 0) { return RemainCount; }

		TargetSlot = TargetContainer.GetEmptySlot(TargetSlot->GetIndex() + 1);
	}

	//Count가 0을 초과한다면 결국 인벤토리 안에 다 못넣었다는 뜻이기에 이에 따라 코드짜야한다.
	return RemainCount;
}

int UInventoryComponent::AddItemByIndex(EItemType InventoryType, int Idx, int Count)
{
	if (!Inventory[InventoryType].IsValidIndex(Idx) || Inventory[InventoryType].IsEmptyIndex(Idx)) return Count;

	int RemainCount = Count;

	int BeforeCount = Inventory[InventoryType][Idx].GetCount();

	Inventory[InventoryType][Idx].AddCount(RemainCount);

	int AfterCount = Inventory[InventoryType][Idx].GetCount();

	RemainCount -= AfterCount - BeforeCount;

	return RemainCount;
}

int UInventoryComponent::RemoveItemByCode(const FName& ItemCode, int Count)
{
	UItemManagerSubsystem* ManagerInstance = UItemManagerSubsystem::GetManagerInstance();
	if (!ManagerInstance) return Count;

	UItemBase* TargetItem = ManagerInstance->MakeItemInstance(ItemCode);
	if (TargetItem == nullptr)
	{
		PFLOG(Error, TEXT("Can't Item Data By : %s"), *ItemCode.ToString());
		return Count;
	}

	return RemoveItemByObject(TargetItem, Count);
}

int UInventoryComponent::RemoveItemByObject(UItemBase* TargetItem, int Count)
{
	FInventorySlotContainer& TargetContainer = Inventory[TargetItem->GetItemType()];
	FInventorySlot* TargetSlot = TargetContainer.FindSlotByCode(TargetItem->GetItemCode());

	int RemainCount = Count;
	int BeforeTargetSlotCount, AfterTargetSlotCount;

	while (TargetSlot != nullptr && 0 < Count)
	{
		BeforeTargetSlotCount = TargetSlot->GetCount();

		TargetSlot->RemoveCount(Count);

		AfterTargetSlotCount = TargetSlot->GetCount();

		if (OnChangedInventory.IsBound() && BeforeTargetSlotCount != AfterTargetSlotCount) {
			OnChangedInventory.Broadcast(TargetItem->GetItemType(), TargetSlot->GetIndex());
		}

		RemainCount += AfterTargetSlotCount - BeforeTargetSlotCount;

		if (RemainCount <= 0) { return RemainCount; }

		TargetSlot = TargetContainer.FindSlotByCode(TargetItem->GetItemCode(), TargetSlot->GetIndex() + 1);
	}

	return RemainCount;
}

int UInventoryComponent::RemoveItemByIndex(EItemType InventoryType, int Idx, int Count)
{
	if(!Inventory[InventoryType].IsValidIndex(Idx) || Inventory[InventoryType].IsEmptyIndex(Idx)) return Count;

	int RemainCount = Count;

	int BeforeCount = Inventory[InventoryType][Idx].GetCount();

	Inventory[InventoryType][Idx].RemoveCount(RemainCount);

	int AfterCount = Inventory[InventoryType][Idx].GetCount();

	RemainCount += AfterCount - BeforeCount;

	return RemainCount;
}

bool UInventoryComponent::TryDropItem(const EItemType& ItemType, int Idx, int Count, const FVector& DropLocation)
{
	if (ItemType == EItemType::None)
	{
		PFLOG(Error, TEXT("Try Drop {None} Type Item"));
		return false;
	}
	if (!Inventory[ItemType].IsValidIndex(Idx))
	{
		PFLOG(Error, TEXT("Try Drop Invalidate Index : %d"), Idx);
		return false;
	}

	FInventorySlot& TargetSlot = Inventory[ItemType][Idx];

	if (TargetSlot.IsEmpty())
	{
		PFLOG(Error, TEXT("Try Drop Empty Slot"));
		return false;
	}

	UItemManagerSubsystem* ManagerInstance = UItemManagerSubsystem::GetManagerInstance();
	if (!IsValid(ManagerInstance))
	{
		PFLOG(Error, TEXT("Item Manager Invalidate"));
		return false;
	}

	ManagerInstance->ItemDrop(TargetSlot.GetItemInSlot(), TargetSlot.GetCount(), DropLocation);
	TargetSlot.ClearSlot();

	return true;
}



FInventorySlot* UInventoryComponent::GetInventorySlot(EItemType InventoryType, int Idx)
{
	if(InventoryType == EItemType::None) return nullptr;
	if(!Inventory[InventoryType].IsValidIndex(Idx)) return nullptr;
	return &Inventory[InventoryType][Idx];
}

FInventorySlot* UInventoryComponent::GetEmptySlot(EItemType InventoryType, int StartIdx)
{
	return Inventory[InventoryType].GetEmptySlot(StartIdx);
}


int UInventoryComponent::GetCountItemInInventory(const FName& ItemCode)
{
	if(ItemCode.IsNone()) return 0;

	int SumCount = 0;

	for (EItemType InventoryType : TEnumRange<EItemType>())
	{
		FInventorySlotContainer& TargetContainer = Inventory[InventoryType];

		FInventorySlot* TargetSlot = TargetContainer.FindSlotByCode(ItemCode);

		while (TargetSlot != nullptr)
		{
			SumCount += TargetSlot->GetCount();
			TargetSlot = TargetContainer.FindSlotByCode(ItemCode, TargetSlot->GetIndex() + 1);
		}
	}

	return SumCount;
}


