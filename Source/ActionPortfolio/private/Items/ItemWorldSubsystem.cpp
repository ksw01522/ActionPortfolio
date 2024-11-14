// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemWorldSubsystem.h"
#include "Items/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Items/ItemManagerSubsystem.h"
#include "Items/ItemDeveloperSetting.h"
#include "Items/ItemMeshCaptureActor.h"


UItemWorldSubsystem::UItemWorldSubsystem()
{
	ItemMeshCaptureActor = nullptr;
}

void UItemWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	const UItemDeveloperSetting* IDS = GetDefault<UItemDeveloperSetting>();

	SetDropItemPoolSize(IDS->GetDropItemPoolSize());

	TSubclassOf<AItemMeshCaptureActor> InfoActorClass = IDS->GetMeshCaptureActorClass();
	const FVector InfoActorLocation = FVector(100000, 100000, 100000);
	ItemMeshCaptureActor = InWorld.SpawnActor<AItemMeshCaptureActor>(InfoActorClass, InfoActorLocation, FRotator::ZeroRotator);

	DroppedItemClass = IDS->GetDefaultDroppedItemClass();
}

void UItemWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//드랍 아이템 액터들 비활성화 상태에서 보관될 곳
	ItemPositionInPool = FVector(100000, 100000, 100000);


}

void UItemWorldSubsystem::Deinitialize()
{
	

}

void UItemWorldSubsystem::RemoveInvalidDrropedItemInstanceInPool()
{
	while (!DroppedItemInstancePool.IsEmpty())
	{
		if (IsValid(DroppedItemInstancePool.Last())) break;

		DroppedItemInstancePool.Pop();
	}
}

ADroppedItem* UItemWorldSubsystem::CreateDroppedItemInstance()
{
	//Transform 만들어야함
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(ItemPositionInPool);
	
	ADroppedItem* NewInstance = GetWorld()->SpawnActorDeferred<ADroppedItem>(DroppedItemClass, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	NewInstance->SetActorHiddenInGame(true);
	NewInstance->FinishSpawning(SpawnTransform);

	return NewInstance;
}

ADroppedItem* UItemWorldSubsystem::GetDroppedItem()
{
	if (DroppedItemInstancePool.IsEmpty()) {
		return CreateDroppedItemInstance();
	}

	return DroppedItemInstancePool.Pop();
}

void UItemWorldSubsystem::BackToPool(ADroppedItem& InPoolItem)
{
	if(DroppedItemInstancePool.Contains(&InPoolItem)){ return; }

	if (DroppedItemInstancePool_Size <= DroppedItemInstancePool.Num())
	{
		InPoolItem.Destroy();
	}
	else
	{
		InPoolItem.SetActorHiddenInGame(true);
		InPoolItem.SetActorLocation(ItemPositionInPool, false, nullptr, ETeleportType::ResetPhysics);
		InPoolItem.ClearDroppedItemData();
		DroppedItemInstancePool.AddUnique(&InPoolItem);
	}
}

void UItemWorldSubsystem::ItemDrop(const FName& ItemCode, const FVector& Position, int Count)
{
	UItemManagerSubsystem* IMS = UItemManagerSubsystem::GetManagerInstance();
	check(IMS);

	UItemBase* TargetItem = IMS->MakeItemInstance(ItemCode, Count);

	ItemDrop(TargetItem, Position);
}

void UItemWorldSubsystem::ItemDrop(UItemBase* Item, const FVector& Position)
{
	check(Item);
	ADroppedItem* DroppedInstance = GetDroppedItem();

	DroppedInstance->SetActorLocation(Position);
	DroppedInstance->SetDroppedItem(Item);
}

void UItemWorldSubsystem::SetDropItemPoolSize(int NewSize)
{
	if(DroppedItemInstancePool_Size == NewSize) return;

	DroppedItemInstancePool_Size = NewSize;
	DroppedItemInstancePool.Reserve(DroppedItemInstancePool_Size);
}


void UItemWorldSubsystem::SetCaptureItemMesh(UStaticMesh* InMesh)
{
	if (ItemMeshCaptureActor == nullptr) return;

	ItemMeshCaptureActor->SetItemMesh(InMesh);
}

