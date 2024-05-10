// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemDeveloperSetting.h"
#include "Engine/DataTable.h"

TArray<TSoftObjectPtr<UDataTable>> UItemDeveloperSetting::GetItemDataTables() const
{
	TArray<TSoftObjectPtr<UDataTable>> ReturnTables;
	ReturnTables.Reserve(3);
	ReturnTables.Add(ItemDataTable_Equipment);
	ReturnTables.Add(ItemDataTable_Consumption);
	ReturnTables.Add(ItemDataTable_Material);

	return MoveTemp(ReturnTables);
}