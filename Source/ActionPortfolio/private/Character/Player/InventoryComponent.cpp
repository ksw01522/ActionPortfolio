// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/InventoryComponent.h"
#include "Items/ItemBase.h"
#include "Items/ItemManagerSubsystem.h"
#include "ActionPortfolio.h"


void FInventorySlotContainer::InitializeCells(int NewSize)
{
	Cells.Reset();
	Cells.Init(FInventorySlot(), NewSize);
}

FInventorySlot* FInventorySlotContainer::GetEmptyCell(int StartIndex) const
{
	FInventorySlot* ReturnCell = nullptr;

	int Length = Cells.Num();
	for (int i = StartIndex; i < Length; i++)
	{
		if (Cells[i].IsEmpty())
		{
			ReturnCell = const_cast<FInventorySlot*>(&Cells[i]);
			break;
		}
	}

	return ReturnCell;
}

FInventorySlot* FInventorySlotContainer::FindCellByCode(const FName& ItemCode, bool bFindIncludeFullCell) const
{
	FInventorySlot* ReturnCell = nullptr;

	int Length = Cells.Num();
	for (int i = 0; i < Length; i++)
	{
		if (Cells[i].GetItemCode() == ItemCode && (!bFindIncludeFullCell || !Cells[i].IsFull() ) )
		{
			ReturnCell = const_cast<FInventorySlot*>(&Cells[i]);
			break;
		}
	}

	return ReturnCell;
}







// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	//������ �� �� ���� �ʴ�.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UEnum* EnumClass = StaticEnum<EItemType>();
	Inventory.Reserve(EnumClass->NumEnums() - 2);

	UItemManagerSubsystem* ManagerSubsystem = UItemManagerSubsystem::GetManagerInstance();
	const int InventorySize = ManagerSubsystem->GetInventorySize();

	//Inventoryĭ �ʱ�ȭ
	for (EItemType ItemType : TEnumRange<EItemType>())
	{
		Inventory.Add(ItemType, FInventorySlotContainer()).InitializeCells(InventorySize);
	}
}

void UInventoryComponent::GetItemIn(const FName& ItemCode, int& Count)
{
	UItemManagerSubsystem* ManagerInstance = UItemManagerSubsystem::GetManagerInstance();
	if (!ManagerInstance) return;
	
	const FItemData_Base* ItemData = ManagerInstance->FindItemData(ItemCode);
	if (ItemData == nullptr)
	{
		return;
	}

	FInventorySlotContainer& TargetContainer = Inventory[ItemData->ItemType];
	FInventorySlot* TargetCell = TargetContainer.FindCellByCode(ItemCode, false);

	//�κ��丮�� ���� �������� ���� �� Count ����
	while (TargetCell != nullptr && 0 < Count)
	{
		TargetCell->AddCount(Count);

		if(Count <= 0) {return;}

		TargetCell = TargetContainer.FindCellByCode(ItemCode, false);
	}

	//�� �κ��丮 ������ �־�� �Ѵٸ� 
	TargetCell = TargetContainer.GetEmptyCell();
	while (TargetCell != nullptr && 0 < Count)
	{
		TargetCell->SetSlot(ItemCode, Count, ItemData->StackSize);

		if(Count <= 0) { return; }
		TargetCell = TargetContainer.GetEmptyCell();
	}

	//Count�� 0�� �ʰ��Ѵٸ� �ᱹ �κ��丮 �ȿ� �� ���־��ٴ� ���̱⿡ �̿� ���� �ڵ�¥���Ѵ�.
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

	ManagerInstance->ItemDrop(TargetSlot.GetItemCode(), TargetSlot.GetCount(), DropLocation);
	TargetSlot.ClearSlot();

	return true;
}

void FInventorySlot::SetSlot(const FName& NewItemCode, int& NewCount, uint8 NewStackSize)
{
	ItemCode = NewItemCode;
	StackSize = NewStackSize;
	if (NewStackSize < NewCount)
	{
		Count = NewStackSize;
		NewCount -= NewStackSize;
	}
	else
	{
		Count = NewCount;
		NewCount = 0;
	}

}

void FInventorySlot::AddCount(int& AddCount)
{
	if (StackSize < AddCount + Count)
	{
		AddCount -= StackSize - Count;
		Count = StackSize;
	}
	else
	{
		Count += AddCount;
		AddCount = 0;
	}
}

void FInventorySlot::RemoveCount(int& RemoveCount)
{
	if (Count <= RemoveCount)
	{
		RemoveCount -= Count;
		ClearSlot();
	}
	else
	{
		Count -= RemoveCount;
		RemoveCount = 0;
	}
}
