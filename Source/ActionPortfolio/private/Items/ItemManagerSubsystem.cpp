// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemManagerSubsystem.h"
#include "Engine/DataTable.h"
#include "Items/ItemBase.h"
#include "ActionPortfolio.h"
#include "Misc/ScopeLock.h"
#include "Items/ItemWorldSubsystem.h"
#include "Instance/ActionPortfolioInstance.h"
#include "Items/ItemDeveloperSetting.h"
#include "Items/ItemDataAsset.h"

#include "Items/Equipment/Item_Equipment.h"
#include "Items/Consumption/Item_Consumption.h"
#include "Items/Material/Item_Material.h"
#include "Items/Equipment/EquipmentAbility.h"

#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"

TObjectPtr<UItemManagerSubsystem> UItemManagerSubsystem::ManagerInstance = nullptr;

UItemManagerSubsystem::UItemManagerSubsystem()
{
	MeshCaptureMaterial = nullptr;
}


void UItemManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	PFLOG(Log, TEXT("Start Item Manager Initialize."));

	ManagerInstance = this;

	{
		MeshCaptureMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), this, TEXT("/Game/Item/Manager/ItemMeshCaptureMaterial.ItemMeshCaptureMaterial")));

		if (MeshCaptureMaterial == nullptr)
		{
			PFLOG(Error, TEXT("Can't Find Item MeshCapture Material"));
		}
	}
	ContentFolderPath = "/Game/Item/";

	InitializeItemData();
	

	const UItemDeveloperSetting* ItemDeveloperSetting = GetDefault<UItemDeveloperSetting>();

	InventorySize = ItemDeveloperSetting->GetInventorySize();

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

void UItemManagerSubsystem::InitializeItemData()
{
	LoadItemDataTables();
	if (!DataTableStruct.IsValid())
	{
		LoadItemDataTablesFromResource();
	}

	AddDataTable(DataTableStruct.Equipment);
	AddDataTable(DataTableStruct.Consumption);
	AddDataTable(DataTableStruct.Material);
}

void UItemManagerSubsystem::LoadItemDataTables()
{
	UItemDataAssetForManager* TempTables = Cast<UItemDataAssetForManager>(
						StaticLoadObject(UItemDataAssetForManager::StaticClass(), this, TEXT("/Game/Item/ItemDatas/DataTables/ItemDataTables.ItemDataTables")));
	
	if (TempTables == nullptr)
	{
		return;
	}
	
	DataTableStruct.Equipment = TempTables->Equipment;
	DataTableStruct.Consumption = TempTables->Consumption;
	DataTableStruct.Material = TempTables->Material;
}

void UItemManagerSubsystem::LoadItemDataTablesFromResource()
{
	PFLOG(Warning, TEXT("Load ItemData Tables From Resource"));

	FString ResourceFolderPath = FPaths::ProjectDir() + TEXT("Resources/Item/DataTable/");

	if (DataTableStruct.Equipment == nullptr)
	{
		UDataTable* EquipmentTable = NewObject<UDataTable>(this, "EquipmentTable");
		EquipmentTable->RowStruct = FItemData_Equipment::StaticStruct();

		FString EquipCSVString;
		FString EquipCSVPath = ResourceFolderPath + TEXT("Equipments.csv");

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*EquipCSVPath))
		{
			FFileHelper::LoadFileToString(EquipCSVString, *EquipCSVPath);

			EquipmentTable->CreateTableFromCSVString(EquipCSVString);
			DataTableStruct.Equipment = EquipmentTable;
		}
	}

	if (DataTableStruct.Consumption == nullptr)
	{
		UDataTable* ConsumptionTable = NewObject<UDataTable>(this, "ConsumptionTable");
		ConsumptionTable->RowStruct = FItemData_Consumption::StaticStruct();

		FString ConsumptionCSVString;
		FString ConsumptionCSVPath = ResourceFolderPath + TEXT("Consumption.csv");

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*ConsumptionCSVPath))
		{
			FFileHelper::LoadFileToString(ConsumptionCSVString, *ConsumptionCSVPath);

			ConsumptionTable->CreateTableFromCSVString(ConsumptionCSVString);
			DataTableStruct.Consumption = ConsumptionTable;
		}
	}

	if (DataTableStruct.Material == nullptr)
	{
		UDataTable* MaterialTable = NewObject<UDataTable>(this, "MaterialTable");
		MaterialTable->RowStruct = FItemData_Material::StaticStruct();

		FString MaterialCSVString;
		FString MaterialCSVPath = ResourceFolderPath + TEXT("Materials.csv");

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*MaterialCSVPath))
		{
			FFileHelper::LoadFileToString(MaterialCSVString, *MaterialCSVPath);

			MaterialTable->CreateTableFromCSVString(MaterialCSVString);
			DataTableStruct.Material = MaterialTable;
		}
	}
}


const FLoadedItemStruct* UItemManagerSubsystem::FindLoadedItemStruct(const FName& Code) const
{
	const FLoadedItemStruct* FindedStrcut = LoadedDataMap.Find(Code);
	if (FindedStrcut == nullptr)
	{
		PFLOG(Error, TEXT("Can't find Item Data by Code : %s"), *Code.ToString());
	}

	return FindedStrcut;
}

UItemBase* UItemManagerSubsystem::MakeItemInstance(const FName& Code, int Count)
{
	const FLoadedItemStruct* ItemData = FindLoadedItemStruct(Code);
	if(ItemData == nullptr) return nullptr;

	UItemBase* NewItemInstance = NewObject<UItemBase>(this, ItemData->DataPtr->GetItemClass(), Code);
	TArray<FText> ErrorMsgs;

	NewItemInstance->SetItemIcon(ItemData->Icon);
	NewItemInstance->SetItemMesh(ItemData->Mesh);
	NewItemInstance->InitializeItem(Code, ItemData->DataPtr, ErrorMsgs);
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


void UItemManagerSubsystem::SetCaptureItemMeshByMesh(UStaticMesh* InMesh)
{
	UWorld* WorldInstance = GetWorld();
	if (WorldInstance == nullptr)
	{
		return;
	}

	UItemWorldSubsystem* IWS = WorldInstance->GetSubsystem<UItemWorldSubsystem>();
	IWS->SetCaptureItemMesh(InMesh);
}

void UItemManagerSubsystem::SetCaptureItemMesh(FName ItemCode)
{
	const FLoadedItemStruct* ItemData = FindLoadedItemStruct(ItemCode);
	check(ItemData);

	SetCaptureItemMeshByMesh(ItemData->Mesh);
}

void UItemManagerSubsystem::SetCaptureItemMesh(const UItemBase* InItem)
{
	SetCaptureItemMeshByMesh(InItem->GetItemMesh());
}

void UItemManagerSubsystem::ClearCaptureItemMesh()
{
	SetCaptureItemMeshByMesh(nullptr);
}



void UItemManagerSubsystem::AddDataTable(UDataTable* InTable)
{
	if(InTable == nullptr) return;

	check(InTable)

	const TMap<FName, uint8*>& TempMap = InTable->GetRowMap();

	for (auto& TempPair : TempMap)
	{
		UTexture2D* TempItemIcon = LoadItemIcon(TempPair.Key.ToString());
		UStaticMesh* TempItemMesh = LoadItemMesh(TempPair.Key.ToString());
		const FItemData_Base* TempItemData = reinterpret_cast<FItemData_Base*>(TempPair.Value);

		LoadedDataMap.Add(TempPair.Key, FLoadedItemStruct(TempItemIcon, TempItemMesh, TempItemData));
	}
}

UTexture2D* UItemManagerSubsystem::LoadItemIcon(FString InItemCode)
{
	FString ItemIconPath = ContentFolderPath + TEXT("Resource/") + InItemCode + TEXT("_Icon.") + InItemCode + TEXT("_Icon");
	UTexture2D* TargetIcon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), this, *ItemIconPath));

	if (TargetIcon == nullptr)
	{
		PFLOG(Error, TEXT("Can't find Item Icon Code : %s"), *InItemCode);
	}

	return TargetIcon;
}

UStaticMesh* UItemManagerSubsystem::LoadItemMesh(FString InItemCode)
{
	FString ItemMeshPath = ContentFolderPath + TEXT("Resource/") + InItemCode + TEXT("_Mesh.") + InItemCode + TEXT("_Mesh");
	UStaticMesh* TargetMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), this, *ItemMeshPath));

	if (TargetMesh)
	{
		PFLOG(Error, TEXT("Can't find Item Icon Mesh : %s"), *InItemCode);
	}

	return TargetMesh;
}

TSubclassOf<UEquipmentAbility> UItemManagerSubsystem::FindEquipmentAbility(FString Code)
{
	if(Code.IsEmpty()) return TSubclassOf<UEquipmentAbility>();

	FString AbilityPath = ContentFolderPath + TEXT("EquipmentAbility/") + Code + TEXT(".") + Code + TEXT("_C");
	TSubclassOf<UEquipmentAbility> ReturnAbility = StaticLoadClass(UEquipmentAbility::StaticClass(), nullptr, *AbilityPath);

	if (ReturnAbility.GetDefaultObject() == nullptr)
	{
		PFLOG(Error, TEXT("Can't find Equipment Ability By Code : %s"), *Code);
	}

	return ReturnAbility;
}

