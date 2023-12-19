// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "ItemBase.generated.h"

/**
 * 
 */

class UEquipmentAbility;
class AActionPortfolioCharacter;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Equipment,
	Consumption,
	Material
};

USTRUCT(BlueprintType)
struct FItemData_Base : public FTableRowBase
{
	GENERATED_BODY()

	FItemData_Base() : ItemClass(), Name(), Description(), IconTexture(nullptr), Price(0), StackSize(0)
	{}
	virtual ~FItemData_Base() {}

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemBase> ItemClass;

	UPROPERTY(EditDefaultsOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly)
	FString Description;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* IconTexture;

	UPROPERTY(EditDefaultsOnly)
	int Price;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0))
	int StackSize;
};

UCLASS(Abstract, BlueprintType)
class ACTIONPORTFOLIO_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	UItemBase() {}
	virtual ~UItemBase(){}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	virtual EItemType GetItemType() const { return EItemType::None; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FName ItemCode;

protected:
	virtual bool CanUseItem(AActionPortfolioCharacter* User) const { return false; }

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	FName GetItemCode() const { return ItemCode; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	bool TryUseItem(AActionPortfolioCharacter* User);

protected:
	virtual void OnUsedItem(AActionPortfolioCharacter* User) {}

};





USTRUCT(BlueprintType)
struct FItemData_Equipment : public FItemData_Base
{
	GENERATED_BODY()

	FItemData_Equipment() : FItemData_Base()
	{}

	UPROPERTY(EditDefaultsOnly, Category = "Item|Equipment")
	TSubclassOf<UEquipmentAbility> EquipmentAbility;


};

UCLASS(Blueprintable)
class ACTIONPORTFOLIO_API UItemBase_Equipment : public UItemBase
{
	GENERATED_BODY()

public:
	UItemBase_Equipment() : EquipCharacter(nullptr)
	{};

	virtual EItemType GetItemType() const override { return EItemType::Equipment; }

protected:
	AActionPortfolioCharacter* EquipCharacter;

protected:
	virtual bool CanUseItem(AActionPortfolioCharacter* User) const override;
	virtual void OnUsedItem(AActionPortfolioCharacter* User) override;
	virtual void OnEquipItem(AActionPortfolioCharacter* User);

public:
	virtual void OnUnequipItem(AActionPortfolioCharacter* User);
};