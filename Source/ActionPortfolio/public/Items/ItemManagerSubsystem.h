// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemManagerSubsystem.generated.h"

/**
 * 
 */
 class UDataTable;
 class ADroppedItem;
 class UItemBase;
 class UItemWorldSubsystem;

 struct FItemData_Base;
 struct FItemData_Equipment;
 struct FItemData_Consumption;
 struct FItemData_Material;

USTRUCT()
struct FLoadedItemStruct
{
	GENERATED_BODY();

	FLoadedItemStruct() : Icon(nullptr), Mesh(nullptr), DataPtr(nullptr)
	{}

	FLoadedItemStruct(UTexture2D* InIcon, UStaticMesh* InMesh, const FItemData_Base* InDataPtr) : 
						Icon(InIcon), Mesh(InMesh), DataPtr(InDataPtr)
	{}

public:
	UPROPERTY(Transient)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(Transient)
	TObjectPtr<UStaticMesh> Mesh;

	const FItemData_Base* DataPtr;
};

USTRUCT()
struct FItemDataTableStruct
{
	GENERATED_BODY();

	FItemDataTableStruct() : Equipment(nullptr)
	{}

public:
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> Equipment;

	UPROPERTY(Transient)
	TObjectPtr<UDataTable> Consumption;

	UPROPERTY(Transient)
	TObjectPtr<UDataTable> Material;

	bool IsValid() const {return Equipment != nullptr && Consumption != nullptr && Material != nullptr; }
};

UCLASS(Config=APGame, Blueprintable, Config = APSetting)
class ACTIONPORTFOLIO_API UItemManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	static TObjectPtr<UItemManagerSubsystem> ManagerInstance;

public:
	static UItemManagerSubsystem* GetManagerInstance() {return ManagerInstance;};
	UItemManagerSubsystem();

private:
	UPROPERTY(Transient)
	FItemDataTableStruct DataTableStruct;

	UPROPERTY(Transient)
	TMap<FName, FLoadedItemStruct> LoadedDataMap;

	UPROPERTY(Transient)
	TObjectPtr<class UMaterialInterface> MeshCaptureMaterial;

	FString ContentFolderPath;

///////////////////////////////////////// 초기화 관련 //////////////////////////////////////////////
private:
	bool bInitialized = false;

public:
	void SetInitialized(bool NewState) { bInitialized = NewState;};

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	void InitializeItemData();
		void LoadItemDataTables();
		void LoadItemDataTablesFromResource();

	void AddDataTable(UDataTable* InTable);
		UTexture2D* LoadItemIcon(FString InItemCode);
		UStaticMesh* LoadItemMesh(FString InItemCode);

public:
	TSubclassOf<class UEquipmentAbility> FindEquipmentAbility(FString Code);

///////////////////////////////////////// 기본 //////////////////////////////////////////////
public:
	const FLoadedItemStruct* FindLoadedItemStruct(const FName& Code) const;

	UItemBase* MakeItemInstance(const FName& Code, int Count = 1);

	UMaterialInterface* GetMeshCaptureMaterial() const { return MeshCaptureMaterial; }

///////////////////////////////////////// 인벤토리 //////////////////////////////////////////////
private:
	uint8 InventorySize;

public:
	uint8 GetInventorySize() const {return InventorySize;}


#if WITH_EDITOR
///////////////////////////////////////// For Editor //////////////////////////////////////////////
private:
	void DebugItemManager() const;
#endif

////////////////////////////////// MeshCapture ///////////////////////////////////////////
private:
	void SetCaptureItemMeshByMesh(UStaticMesh* InMesh);

public:
	void SetCaptureItemMesh(FName ItemCode);
	void SetCaptureItemMesh(const UItemBase* InItem);
	void ClearCaptureItemMesh();


};
