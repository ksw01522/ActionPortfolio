// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemSlot.generated.h"

/**
 * 
 */

class UItemBase;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangedItemSlot, UItemSlot*, UItemBase*);

UCLASS(Abstract)
class ACTIONPORTFOLIO_API UItemSlot : public UObject
{
	GENERATED_BODY()
	
public:
	UItemSlot();
	UItemSlot(FName InSlotType);

private:
	UPROPERTY(Transient)
	TObjectPtr<UItemBase> Item;

	FOnChangedItemSlot OnChangedItemInSlot;

	FName SlotType;

protected:
	virtual void NativeOnChangedItemInSlot(UItemBase* PrevItem);

public:
	virtual void SlotDropTo(UItemSlot* To) {}

	virtual bool CanItemInSlot(UItemBase* InItem) const {return true;}

	UItemBase* GetItem() const { return Item; }
	int GetItemCount() const;

	bool IsEmptySlot() const {return Item == nullptr;}

	void SetItem(UItemBase* InItem);

	FOnChangedItemSlot& GetOnChagnedItemInSlot() {return OnChangedItemInSlot;}

	void AddCount(int NewCount);
	void RemoveCount(int NewCount);

	FName GetSlotType() const { return SlotType; }
};
