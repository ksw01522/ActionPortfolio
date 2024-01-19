// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemManagerSubsystem.h"
#include "Engine/DataTable.h"
#include "Items/ItemBase.h"
#include "ActionPortfolio.h"
#include "Misc/ScopeLock.h"
#include "Items/DropItemPoolWorldSubsystem.h"
#include "ActionPortfolioInstance.h"

TObjectPtr<UItemManagerSubsystem> UItemManagerSubsystem::ManagerInstance = nullptr;

UItemManagerSubsystem::UItemManagerSubsystem()
{
	InventorySize = 64;
	

	DropItemPool = nullptr;
	DefaultDIBounceCurve = nullptr;
	DefaultDIMagnetizedCurve = nullptr;

	BouncePower = 250;
	BounceHeight = 100;
}

void UItemManagerSubsystem::AddItemData(const TSoftObjectPtr<UDataTable>& NewItemDataTable)
{
	if(bInitialized) return;

	if(NewItemDataTable.IsNull()) 
	{
		ensureMsgf(false, TEXT("Add Item Data By Null DataTable"));
		return;
	}

	UDataTable* LoadedDataTable = NewItemDataTable.LoadSynchronous();
	if (!LoadedDataTable->GetRowStruct()->IsChildOf(FItemData_Base::StaticStruct()))
	{
		PFLOG(Error, TEXT("Try Add Not ItemDataStruct's Child Strcut"));
		return;
	}
	if(LoadedDataTable->GetRowMap().IsEmpty()) return;

	ItemDataTables.AddUnique(LoadedDataTable);
	
	TMap<FName, uint8*> TempDataMap = LoadedDataTable->GetRowMap();
	for (const auto& TempData : TempDataMap)
	{
		const FItemData_Base* EquipmentData = reinterpret_cast<FItemData_Base*>(TempData.Value);
		UItemBase* TempItem = NewObject<UItemBase>(this, EquipmentData->ItemClass, EquipmentData->Name);
		TempItem->InitializeItem(TempData.Key, EquipmentData);

	}
}

void UItemManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	PFLOG(Log, TEXT("Start Item Manager Initialize."));

	ManagerInstance = this;

	UActionPortfolioInstance* GameInstance = Cast<UActionPortfolioInstance>(GetGameInstance());

	//아이템 데이타들
	TArray<TSoftObjectPtr<UDataTable>> ItemDatas = GameInstance->GetItemDataTables();
	for (auto& ItemData : ItemDatas)
	{
		AddItemData(ItemData);
	}

	//그 외 데이터들 등록
	DefaultDIBounceCurve = GameInstance->GetDefaultDIBounceCurve().LoadSynchronous();
	BouncePower = GameInstance->GetBouncePower();
	BounceHeight = GameInstance->GetBounceHeight();
	DefaultDIMagnetizedCurve = GameInstance->GetDefaultDIMagnetizedCurve().LoadSynchronous();

	InventorySize = GameInstance->GetInventorySize();
	DropItemPoolSize = GameInstance->GetDropItemPoolSize();


#if WITH_EDITOR
	DebugItemManager();
#endif

		
	SetInitialized(true);
	PFLOG(Log, TEXT("End Item Manager Initialize."));
}

void UItemManagerSubsystem::Deinitialize()
{
	PFLOG(Log, TEXT("Start Item Manager Deinitialize."));

	ManagerInstance = nullptr;

	PFLOG(Log, TEXT("End Item Manager Deinitialize."));
}

const FItemData_Base* UItemManagerSubsystem::FindItemData(const FName& Code) const
{
	int TableNum = ItemDataTables.Num();
	for (int i = 0; i < TableNum; i++)
	{
		if(const FItemData_Base* TempData = ItemDataTables[i]->FindRow<FItemData_Base>(Code, FString(""), false)) return TempData;
	}

	return nullptr;
}

UItemBase* UItemManagerSubsystem::MakeItemInstance(const FName& Code)
{
	const FItemData_Base* ItemData = FindItemData(Code);
	if(ItemData == nullptr) return nullptr;

	UItemBase* NewItemInstance = NewObject<UItemBase>(this, ItemData->ItemClass, ItemData->Name);
	NewItemInstance->InitializeItem(Code, ItemData);

#if WITH_EDITOR
	PFLOG(Warning, TEXT("Make New Item Instance : Code {%s}, ItemName {%s}"), *Code.ToString(), *NewItemInstance->GetItemName().ToString());
#endif

	return NewItemInstance;
}


void UItemManagerSubsystem::SetDropItemPool(UDropItemPoolWorldSubsystem* NewPool)
{
	DropItemPool = NewPool;
}

void UItemManagerSubsystem::SetDropItemPoolSize(int NewSize)
{
	DropItemPoolSize = NewSize;

	if (DropItemPool.IsValid())
	{
		DropItemPool->SetDropItemPoolSize(NewSize);
	}
}

void UItemManagerSubsystem::ItemDrop(const FName& ItemCode, int Count, const FVector& Position)
{
	if (Count <= 0)
	{
		PFLOG(Error, TEXT("Invalidate Count : %d"), Count);
		return;
	}

	UItemBase* TargetItem = MakeItemInstance(ItemCode);
	ItemDrop(TargetItem, Count, Position);
}

void UItemManagerSubsystem::ItemDrop(UItemBase* Item, int Count, const FVector& Position)
{
	if(!IsValid(Item)) 
	{
		PFLOG(Error, TEXT("Try Drop Invalid Item"));
		return;
	}
	ADroppedItem* DroppedItemInstance = DropItemPool->GetDroppedItem();

	DroppedItemInstance->SetActorLocation(Position);
	DroppedItemInstance->SetDroppedItem(Item, Count);
}

void UItemManagerSubsystem::BackToDropItemPool(ADroppedItem* Target)
{
	if (!IsValid(Target))
	{
		PFLOG(Error, TEXT("Invalid Draopped Item."));
		return;
	}

	DropItemPool->BackToPool(Target);
}


#if WITH_EDITOR
void UItemManagerSubsystem::DebugItemManager() const
{
	if (DefaultDIBounceCurve == nullptr)
	{
		PFLOG(Error, TEXT("Can't find Default DI Bounce Curve"));
	}
	else
	{
		float MinDIBTime, MaxDIBTime;
		DefaultDIBounceCurve->GetTimeRange(MinDIBTime, MaxDIBTime);
		PFLOG(Warning, TEXT("Default DI Bounce Curve TimeRange : %.1f ~ %.1f"), MinDIBTime, MaxDIBTime);
	}



}

#endif

