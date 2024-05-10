// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemManagerSubsystem.h"
#include "Engine/DataTable.h"
#include "Items/ItemBase.h"
#include "ActionPortfolio.h"
#include "Misc/ScopeLock.h"
#include "Items/ItemWorldSubsystem.h"
#include "ActionPortfolioInstance.h"
#include "Items/ItemDeveloperSetting.h"

TObjectPtr<UItemManagerSubsystem> UItemManagerSubsystem::ManagerInstance = nullptr;

UItemManagerSubsystem::UItemManagerSubsystem()
{
	MeshCaptureMaterial = nullptr;
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

	const UItemDeveloperSetting* ItemDeveloperSetting = GetDefault<UItemDeveloperSetting>();

	//아이템 데이타들
	TArray<TSoftObjectPtr<UDataTable>> ItemDatas = ItemDeveloperSetting->GetItemDataTables();
	for (auto& ItemData : ItemDatas)
	{
		AddItemData(ItemData);
	}

	InventorySize = ItemDeveloperSetting->GetInventorySize();
	MeshCaptureMaterial = ItemDeveloperSetting->GetMeshCaptureMaterial().LoadSynchronous();

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

UItemBase* UItemManagerSubsystem::MakeItemInstance(const FName& Code, int Count)
{
	const FItemData_Base* ItemData = FindItemData(Code);
	if(ItemData == nullptr) return nullptr;

	UItemBase* NewItemInstance = NewObject<UItemBase>(this, ItemData->ItemClass, ItemData->Name);
	NewItemInstance->InitializeItem(Code, ItemData);
	NewItemInstance->SetCount(Count);

#if WITH_EDITOR
	PFLOG(Warning, TEXT("Make New Item Instance : Code {%s}, ItemName {%s}"), *Code.ToString(), *NewItemInstance->GetItemName().ToString());
#endif

	return NewItemInstance;
}



#if WITH_EDITOR
void UItemManagerSubsystem::DebugItemManager() const
{

}



#endif

void UItemManagerSubsystem::SetCaptureItemMeshByMesh(TSoftObjectPtr<UStaticMesh> InMesh)
{
	UWorld* World = GetWorld();
	check(World);

	UItemWorldSubsystem* IWS = World->GetSubsystem<UItemWorldSubsystem>();
	if (IWS == nullptr) return;

	IWS->SetCaptureItemMesh(InMesh);
}

void UItemManagerSubsystem::SetCaptureItemMesh(FName ItemCode)
{
	const FItemData_Base* ItemData = FindItemData(ItemCode);
	check(ItemData);

	SetCaptureItemMeshByMesh(ItemData->ItemMesh);
}

void UItemManagerSubsystem::SetCaptureItemMesh(const UItemBase* InItem)
{
	SetCaptureItemMeshByMesh(InItem->GetItemMesh());
}

void UItemManagerSubsystem::ClearCaptureItemMesh()
{
	SetCaptureItemMeshByMesh(nullptr);
}
