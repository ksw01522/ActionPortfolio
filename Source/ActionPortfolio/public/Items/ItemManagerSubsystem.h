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
 class UDropItemPoolWorldSubsystem;

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
private:
	const FItemData_Base* FindItemData(const FName& Code) const;

public:
	UItemBase* MakeItemInstance(const FName& Code);


///////////////////////////////////////// 드랍 아이템 인스턴스 풀 //////////////////////////////////////////////
private:
	TWeakObjectPtr<UDropItemPoolWorldSubsystem> DropItemPool;
	int DropItemPoolSize;

	UPROPERTY()
	TObjectPtr<UCurveFloat> DefaultDIBounceCurve;
	
	float BouncePower;

	float BounceHeight;

	UPROPERTY()
	TObjectPtr<UCurveFloat> DefaultDIMagnetizedCurve;

public: 
	void SetDropItemPool(UDropItemPoolWorldSubsystem* NewPool);

	void SetDropItemPoolSize(int NewSize);
	int GetDropItemPoolSize() const {return DropItemPoolSize;};

	UFUNCTION(BlueprintCallable)
	void ItemDrop(const FName& ItemCode, int Count, const FVector& Position);

	void ItemDrop(UItemBase* Item, int Count, const FVector& Position);


	void BackToDropItemPool(ADroppedItem* Target);

	UCurveFloat* GetDIBounceCurve() const {return DefaultDIBounceCurve;}
	UCurveFloat* GetDIMagnetizedCurve() const { return DefaultDIMagnetizedCurve; }

	float GetBouncePower() const {return BouncePower;}
	float GetBounceHeight() const {return BounceHeight;}

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


};
