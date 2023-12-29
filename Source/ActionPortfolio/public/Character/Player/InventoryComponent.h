// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


enum class EItemType : uint8;

struct FItemData_Base;

class ADroppedItem;

struct FInventorySlot
{
	friend class UInventoryComponent;

	FInventorySlot() : ItemCode(), Count(0) {}
	virtual ~FInventorySlot(){}

private:
	FName ItemCode;
	uint8 Count;
	uint8 StackSize;

	void SetSlot(const FName& NewItemCode, int& NewCount, uint8 NewStackSize);
	void AddCount(int& AddCount);
	void RemoveCount(int& RemoveCount);
	void ClearSlot() { ItemCode = NAME_None; Count = 0; StackSize = 0;}

public:
	bool IsEmpty() const { return Count <= 0 || ItemCode == NAME_None; }
	FName GetItemCode() const { return ItemCode; }
	uint8 GetCount() const {return Count;}
	bool IsFull() const { return Count == StackSize;}
};

struct FInventorySlotContainer
{
	friend class UInventoryComponent;

	FInventorySlotContainer(){}
	virtual ~FInventorySlotContainer(){}

private:
	TArray<FInventorySlot> Cells;

	void InitializeCells(int NewSize);
	FInventorySlot* GetEmptyCell(int StartIndex = 0) const;
	FInventorySlot* FindCellByCode(const FName& ItemCode, bool bFindIncludeFullCell) const;

public:
	bool IsValidIndex(int Idx) const{return Cells.IsValidIndex(Idx); };
	bool IsEmptyIndex(int Idx) const{return !IsValidIndex(Idx) || Cells[Idx].IsEmpty(); };

	FInventorySlot& operator[](const int& Index)
	{
		return Cells[Index];
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONPORTFOLIO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// bCanEverTick = false
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	TMap<EItemType, FInventorySlotContainer> Inventory;

	mutable FCriticalSection CSInventory;

public:
	void GetItemIn(const FName& ItemCode, int& Count);
	void GetItemOut(const FName& ItemCode, int& Count);

	bool TryDropItem(const EItemType& ItemType, int Idx, int Count, const FVector& DropLocation);
};
