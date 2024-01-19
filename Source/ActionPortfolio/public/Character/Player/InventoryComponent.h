// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


enum class EItemType : uint8;

struct FItemData_Base;

class UItemBase;
class ADroppedItem;

struct FInventorySlot
{
	friend class UInventoryComponent;
	friend struct FInventorySlotContainer;
private:
	FInventorySlot() : Item(nullptr), Count(0) {}
public:
	virtual ~FInventorySlot(){}

private:
	UPROPERTY(Transient)
	TObjectPtr<UItemBase> Item;
	int Count;
	int Index;

	void CheckCount();

public:
	bool IsEmpty() const;
	
	UItemBase* GetItemInSlot(){ return Item.Get(); }

	FName GetItemCode() const;
	uint8 GetCount() const {return Count;}

	int GetIndex() const {return Index;}

	void SetSlot(UItemBase* Item, int NewCount);
	void SetCount(int NewCount);

	void AddCount(int AddCount);
	void RemoveCount(int RemoveCount);
	void ClearSlot() { Item = nullptr; Count = 0; }
};

struct FInventorySlotContainer
{
	friend class UInventoryComponent;

	FInventorySlotContainer(){}
	virtual ~FInventorySlotContainer(){}

private:
	TArray<FInventorySlot> Slots;

	void InitializeSlots(int NewSize);
	FInventorySlot* GetEmptySlot(int StartIndex = 0);
	FInventorySlot* FindSlotByCode(const FName& ItemCode, int StartIndex = 0);

public:
	bool IsValidIndex(int Idx) const{return Slots.IsValidIndex(Idx); };
	bool IsEmptyIndex(int Idx) const{return !IsValidIndex(Idx) || Slots[Idx].IsEmpty(); };
	int GetSize() const {return Slots.Num(); }


	FInventorySlot& operator[](const int& Index)
	{
		return Slots[Index];
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONPORTFOLIO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangedInventory, EItemType, InventoryType, int, Idx);

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

public:
	FOnChangedInventory OnChangedInventory;

public:
	void AddItemByDropItem(class ADroppedItem& DropItem);
	
	//Return Remain Item Count
	int AddItemByCode(const FName& ItemCode, int Count);
	int AddItemByObject(UItemBase* TargetItem, int Count);
	int AddItemByIndex(EItemType InventoryType, int Idx, int Count);

public:
	//Return Remain Item Count
	int RemoveItemByCode(const FName& ItemCode, int Count);
	int RemoveItemByObject(UItemBase* TargetItem, int Count);
	int RemoveItemByIndex(EItemType InventoryType, int Idx, int Count);

	bool TryDropItem(const EItemType& ItemType, int Idx, int Count, const FVector& DropLocation);

	FInventorySlot* GetInventorySlot(EItemType InventoryType, int idx);
	FInventorySlot* GetEmptySlot(EItemType InventoryType, int StartIdx = 0);


	int GetCountItemInInventory(const FName& ItemCode);
};
