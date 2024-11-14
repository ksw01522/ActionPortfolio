// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UItemDataAssetForManager : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "DataTable", meta = (RequiredAssetDataTags = "RowStructure=/Script/ActionPortfolio.ItemData_Equipment"))
	TObjectPtr<class UDataTable> Equipment;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable", meta = (RequiredAssetDataTags = "RowStructure=/Script/ActionPortfolio.ItemData_Consumption"))
	TObjectPtr<UDataTable> Consumption;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable", meta = (RequiredAssetDataTags = "RowStructure=/Script/ActionPortfolio.ItemData_Material"))
	TObjectPtr<UDataTable> Material;
};
