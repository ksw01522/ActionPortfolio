// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/ItemBase.h"
#include "Item_Consumption.generated.h"



/**
 * 
 */

UCLASS(Blueprintable)
class ACTIONPORTFOLIO_API UItemBase_Consumption : public UItemBase
{
	GENERATED_BODY()


protected:
	//virtual void InitializeItem(const FName& NewItemCode, const FItemData_Base* Data) override;

};

USTRUCT(BlueprintType)
struct FItemData_Consumption : public FItemData_Base
{
	GENERATED_BODY()

	FItemData_Consumption() : FItemData_Base(EItemType::Consumption, 99, UItemBase_Consumption::StaticClass())
	{}

	virtual TSubclassOf<class UItemBase> GetItemClass() const override { return UItemBase_Consumption::StaticClass(); }
};

