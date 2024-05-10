// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Slot/ItemSlot.h"
#include "InventoryComponent.generated.h"


enum class EItemType : uint8;

struct FItemData_Base;

class UItemBase;
class ADroppedItem;

UCLASS()
class UInventorySlot : public UItemSlot
{
	GENERATED_BODY()

public:
	UInventorySlot() : UItemSlot("Inventory")
	{}

private:
	virtual void SlotDropTo(UItemSlot* To) override;
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
	UPROPERTY(Transient)
	TArray<TObjectPtr<UInventorySlot>> Slots;

public:
	TArray<UInventorySlot*> GetSlotArray() const {return Slots;}

	void AddItemByDropItem(class ADroppedItem& DropItem);
	
	//Return Remain Item Count
	int AddItemByCode(const FName& ItemCode, int Count = 1);
	int AddItemByObject(UItemBase* TargetItem);
	int AddItemByIndex(int Idx, int Count);

public:
	//Return Remain Item Count
	int RemoveItemByCode(const FName& ItemCode, int Count);
	int RemoveItemByObject(UItemBase* TargetItem);
	int RemoveItemByIndex(int Idx, int Count);

	bool TryDropItem(int Idx, int Count, const FVector& DropLocation);

	int GetSlotCount() const {return Slots.Num(); }
	UInventorySlot* GetInventorySlot(int Idx) const;
	UInventorySlot* GetEmptySlot(int StartIdx = 0) const;

	int GetCountItemInInventory(const FName& ItemCode);
};
