// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DropItemPoolWorldSubsystem.generated.h"

/**
 * 
 */
 class ADroppedItem;

UCLASS()
class ACTIONPORTFOLIO_API UDropItemPoolWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	UDropItemPoolWorldSubsystem();

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:

	TArray<TObjectPtr<ADroppedItem>> DroppedItemInstancePool;

	int DroppedItemInstancePool_Size;

	FVector ItemPositionInPool;

	//인스턴스 풀 안에 유효하지않는 아이템들 삭제하기 일단 만들어두고 쓸필요 있을지 생각
	void RemoveInvalidDrropedItemInstanceInPool();

	ADroppedItem* CreateDroppedItemInstance();

public:
	ADroppedItem* GetDroppedItem();
	void BackToPool(ADroppedItem* InPoolItem);
	void SetDropItemPoolSize(int NewSize);
};
