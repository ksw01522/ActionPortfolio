// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Settings/CustomStructForSetting.h"
#include "ActionPortfolioInstance.generated.h"

/**
 * 
 */
 class UDataTable;

USTRUCT()
struct FItemManagerProperty
{
	GENERATED_BODY()

	friend class UItemManagerSubsystem;

	FItemManagerProperty() {

	}

private:

};


UCLASS()
class ACTIONPORTFOLIO_API UActionPortfolioInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UActionPortfolioInstance();

	static UActionPortfolioInstance* ActionPFInstance;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionPF|Widget", meta = (AllowPrivateAccess = "true"))
	float SlowDialogueAnimTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionPF|Widget", meta = (AllowPrivateAccess = "true"))
	float NormalDialogueAnimTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionPF|Widget", meta = (AllowPrivateAccess = "true"))
	float FastDialogueAnimTime;


public:
	virtual void Init() override;
	virtual void Shutdown() override;

	float GetDialogueAnimTime() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Localization")
	ELanguage GetCurrentLanguage() const;


///////////////////////////////////// Item Manager ///////////////////////////////
private:
	UPROPERTY(EditDefaultsOnly, Category = "ItemManager", meta = (RequiredAssetDataTags = "RowStructure=/Script/ActionPortfolio.ItemData_Equipment"))
	TSoftObjectPtr<UDataTable> ItemDataTable_Equipment;

	UPROPERTY(EditDefaultsOnly, Category = "ItemManager", meta = (RequiredAssetDataTags = "RowStructure=/Script/ActionPortfolio.ItemData_Consumption"))
	TSoftObjectPtr<UDataTable> ItemDataTable_Consumption;

	UPROPERTY(EditDefaultsOnly, Category = "ItemManager", meta = (RequiredAssetDataTags = "RowStructure=/Script/ActionPortfolio.ItemData_Material"))
	TSoftObjectPtr<UDataTable> ItemDataTable_Material;

	UPROPERTY(EditDefaultsOnly, Category = "ItemManager|DropItem")
	int DropItemPoolSize;

	UPROPERTY(EditDefaultsOnly, Category = "ItemManager|DropItem|Bounce")
	TSoftObjectPtr<UCurveFloat> DefaultDIBounceCurve;

	UPROPERTY(EditDefaultsOnly, Category = "ItemManager|DropItem|Bounce")
	float BouncePower;

	UPROPERTY(EditDefaultsOnly, Category = "ItemManager|DropItem|Bounce")
	float BounceHeight;

	UPROPERTY(EditDefaultsOnly, Category = "ItemManager|DropItem|Magnetized")
	TSoftObjectPtr<UCurveFloat> DefaultDIMagnetizedCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int InventorySize;

public:
	TArray<TSoftObjectPtr<UDataTable>> GetItemDataTables() const;
	int GetDropItemPoolSize() const {return DropItemPoolSize;}
	TSoftObjectPtr<UCurveFloat> GetDefaultDIBounceCurve() const {return DefaultDIBounceCurve;}
	float GetBouncePower() const {return BouncePower;}
	float GetBounceHeight() const {return BounceHeight;}
	TSoftObjectPtr<UCurveFloat> GetDefaultDIMagnetizedCurve() const {return DefaultDIMagnetizedCurve;}
	int GetInventorySize() const { return InventorySize; }
};
