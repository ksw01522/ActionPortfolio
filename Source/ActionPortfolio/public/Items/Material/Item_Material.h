// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/ItemBase.h"
#include "Item_Material.generated.h"



/**
 * 
 */



UCLASS(Blueprintable)
class ACTIONPORTFOLIO_API UItemBase_Material : public UItemBase
{
	GENERATED_BODY()

protected:
	//virtual void InitializeItem(const FName& NewItemCode, const FItemData_Base* Data) override;

};

USTRUCT(BlueprintType)
struct FItemData_Material : public FItemData_Base
{
	GENERATED_BODY()

	FItemData_Material() : FItemData_Base(EItemType::Material, 99, UItemBase_Material::StaticClass())
	{}

	virtual TSubclassOf<class UItemBase> GetItemClass() const override {return UItemBase_Material::StaticClass(); }
};
