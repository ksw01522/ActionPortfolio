// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ItemDeveloperSetting.generated.h"

/**
 * 
 */
UCLASS(Config = Game, defaultconfig)
class ACTIONPORTFOLIO_API UItemDeveloperSetting : public UDeveloperSettings
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "DropItem", GlobalConfig)
	int DropItemPoolSize;

	UPROPERTY(EditDefaultsOnly, Category = "DropItem", GlobalConfig)
	TSubclassOf<class ADroppedItem> DefaultDroppedItemClass;

	UPROPERTY(EditDefaultsOnly, Category = "Item", GlobalConfig)
	TSubclassOf<class AItemMeshCaptureActor> MeshCaptureActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory", GlobalConfig)
	int InventorySize;

public:
	int GetInventorySize() const {return InventorySize;}

	int GetDropItemPoolSize() const { return DropItemPoolSize; }
	TSubclassOf<ADroppedItem> GetDefaultDroppedItemClass() const { return DefaultDroppedItemClass; }

	TSubclassOf<AItemMeshCaptureActor> GetMeshCaptureActorClass() const { return MeshCaptureActorClass;}
};
