// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemManagerSubsystem.h"
#include "Engine/DataTable.h"
#include "ActionPortfolio.h"

void UItemManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UDataTable* ItemTable = LoadObject<UDataTable>(nullptr, *ItemDataTablePath);
	if (ItemTable == nullptr)
	{
		PFLOG(Error, TEXT("Check Item DataTable Path : %s"), *ItemDataTablePath);
	}
}
