// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/ItemBase.h"
#include "GameplayEffectTypes.h"
#include "Item_Equipment.generated.h"



/**
 * 
 */



UENUM(BlueprintType)
enum class EEquipmentPart : uint8
{
	None = 0 UMETA(Hidden),
	Body,
	Pants,
	Head,
	Arm,
	Foot
};

ENUM_RANGE_BY_FIRST_AND_LAST(EEquipmentPart, EEquipmentPart::Body, EEquipmentPart::Foot);

UCLASS(Blueprintable)
class ACTIONPORTFOLIO_API UItemBase_Equipment : public UItemBase
{
	GENERATED_BODY()

public:
	UItemBase_Equipment()
	{};

private:
	TSubclassOf<UEquipmentAbility> EquipmentAbility;

	EEquipmentPart EquipmentPart;

	float MaxHP;

	float MaxStamina;

	float AttackP;

	float DefenseP;

protected:
	virtual void InitializeItem(const FName& NewItemCode, const FItemData_Base* Data, TArray<FText>& OutErroMsgs) override;

public:
	virtual bool IsSame(const UItemBase* Other) const override;

	EEquipmentPart GetEquipmentPart() const {return EquipmentPart;}

	virtual FGameplayEffectSpec MakeAddStatusEffect(const FGameplayEffectContextHandle& InEffectContext) const;

	virtual bool CanEquipItem(class UActionPFAbilitySystemComponent* ASC) const;

	TSubclassOf<UEquipmentAbility> GetEquipmentAbility() const { return EquipmentAbility;}
};

USTRUCT(BlueprintType)
struct FItemData_Equipment : public FItemData_Base
{
	GENERATED_BODY()

	FItemData_Equipment() : FItemData_Base(EItemType::Equipment, 1, UItemBase_Equipment::StaticClass()), MaxHP(0), MaxStamina(0), AttackP(0), DefenseP(0)
	{}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	FString AbilityCode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	EEquipmentPart EquipmentPart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	float MaxHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	float AttackP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	float DefenseP;

	virtual TSubclassOf<class UItemBase> GetItemClass() const override { return UItemBase_Equipment::StaticClass(); }
};




