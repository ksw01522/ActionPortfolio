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
	TArray<TObjectPtr<UDataTable>> ItemDataTables;

	UPROPERTY(Transient)
	TObjectPtr<class UMaterialInterface> MeshCaptureMaterial;

///////////////////////////////////////// 초기화 관련 //////////////////////////////////////////////
private:
	bool bInitialized = false;

public:
	void AddItemData(const TSoftObjectPtr<UDataTable>& NewItemDataTable);
	void SetInitialized(bool NewState) { bInitialized = NewState;};

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

///////////////////////////////////////// 기본 //////////////////////////////////////////////
public:
	const FItemData_Base* FindItemData(const FName& Code) const;

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
	void SetCaptureItemMeshByMesh(TSoftObjectPtr<UStaticMesh> InMesh);

public:
	void SetCaptureItemMesh(FName ItemCode);
	void SetCaptureItemMesh(const UItemBase* InItem);
	void ClearCaptureItemMesh();
};
