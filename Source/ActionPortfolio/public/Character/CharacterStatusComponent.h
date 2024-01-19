// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

//Ability연동을 위한 헤더
#include "GameplayAbilitySpecHandle.h"
#include "ActiveGameplayEffectHandle.h"

#include "CharacterStatusComponent.generated.h"

struct FItemData_Equipment;
struct FInventorySlot;

class UGameplayEffect;
class UItemBase_Equipment;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONPORTFOLIO_API UCharacterStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatusComponent();
	/*
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//TArray<FEquipmentSlot> EquipmentSlots;

	TWeakObjectPtr<class UActionPFAbilitySystemComponent> AbilitySystemComponent;

	void EquipItem(FName ItemCode, int Idx);
	void UnequipItem(int Idx);

public:
	bool CanEquipItem(FName ItemCode);

	bool TryEquipItem(FName ItemCode);
	bool TryEquipItem(FName ItemCode, int Idx);
	bool TryEquipItem(FInventorySlot* InventorySlot);
	bool TryEquipItem(FInventorySlot* InventorySlot, int Idx);

	

	FName GetEquippedItemCode(int Idx);

	int GetEmptySlot() const;
	*/
};
