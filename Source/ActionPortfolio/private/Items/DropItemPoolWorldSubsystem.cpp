// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DropItemPoolWorldSubsystem.h"
#include "Items/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Items/ItemManagerSubsystem.h"



UDropItemPoolWorldSubsystem::UDropItemPoolWorldSubsystem()
{
}

void UDropItemPoolWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	UItemManagerSubsystem* ItemManager = UItemManagerSubsystem::GetManagerInstance();
	ItemManager->SetDropItemPool(this);

	SetDropItemPoolSize(ItemManager->GetDropItemPoolSize());

}

void UDropItemPoolWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//드랍 아이템 액터들 비활성화 상태에서 보관될 곳
	ItemPositionInPool = FVector(100000, 100000, 100000);

}

void UDropItemPoolWorldSubsystem::Deinitialize()
{
	UItemManagerSubsystem* ItemManager = UItemManagerSubsystem::GetManagerInstance();
	if(ItemManager) ItemManager->SetDropItemPool(nullptr);

}

void UDropItemPoolWorldSubsystem::RemoveInvalidDrropedItemInstanceInPool()
{
	while (!DroppedItemInstancePool.IsEmpty())
	{
		if (IsValid(DroppedItemInstancePool.Last())) break;

		DroppedItemInstancePool.Pop();
	}
}

ADroppedItem* UDropItemPoolWorldSubsystem::CreateDroppedItemInstance()
{
	//Transform 만들어야함
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(ItemPositionInPool);
	
	ADroppedItem* NewInstance = GetWorld()->SpawnActorDeferred<ADroppedItem>(ADroppedItem::StaticClass(), SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	NewInstance->SetActorHiddenInGame(true);
	NewInstance->FinishSpawning(SpawnTransform);

	return NewInstance;
}

ADroppedItem* UDropItemPoolWorldSubsystem::GetDroppedItem()
{

	if (DroppedItemInstancePool.IsEmpty()) {
		return CreateDroppedItemInstance();
	}

	return DroppedItemInstancePool.Pop();
}

void UDropItemPoolWorldSubsystem::BackToPool(ADroppedItem* InPoolItem)
{
	if(!IsValid(InPoolItem)) return;


	if (DroppedItemInstancePool_Size <= DroppedItemInstancePool.Num())
	{
		InPoolItem->Destroy();
	}
	else
	{
		InPoolItem->SetActorHiddenInGame(true);
		InPoolItem->SetActorLocation(ItemPositionInPool, false, nullptr, ETeleportType::ResetPhysics);
		InPoolItem->ClearDroppedItemData();
		DroppedItemInstancePool.AddUnique(InPoolItem);
	}
}

void UDropItemPoolWorldSubsystem::SetDropItemPoolSize(int NewSize)
{
	if(DroppedItemInstancePool_Size == NewSize) return;

	DroppedItemInstancePool_Size = NewSize;
	DroppedItemInstancePool.Reserve(DroppedItemInstancePool_Size);
}
