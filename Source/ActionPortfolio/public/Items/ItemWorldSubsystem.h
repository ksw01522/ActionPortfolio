// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ItemWorldSubsystem.generated.h"

/**
 * 
 */
 class ADroppedItem;
 class UItemBase;

UCLASS()
class ACTIONPORTFOLIO_API UItemWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	UItemWorldSubsystem();

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

//////////////////////////// Drop Item /////////////////////////
private:
	TArray<TObjectPtr<ADroppedItem>> DroppedItemInstancePool;

	int DroppedItemInstancePool_Size;

	FVector ItemPositionInPool;

	TSubclassOf<ADroppedItem> DroppedItemClass;

	//인스턴스 풀 안에 유효하지않는 아이템들 삭제하기 일단 만들어두고 쓸필요 있을지 생각
	void RemoveInvalidDrropedItemInstanceInPool();

	ADroppedItem* CreateDroppedItemInstance();

	void SetDropItemPoolSize(int NewSize);

public:
	ADroppedItem* GetDroppedItem();
	void BackToPool(ADroppedItem& InPoolItem);
	
	void ItemDrop(const FName& ItemCode, const FVector& Position, int Count = 1);
	void ItemDrop(UItemBase* Item, const FVector& Position);

/////////////////////// For Item Information /////////////////
private:
	UPROPERTY(Transient)
	TObjectPtr<class AItemMeshCaptureActor> ItemMeshCaptureActor;

public:
	void SetCaptureItemMesh(UStaticMesh* InMesh);

};
