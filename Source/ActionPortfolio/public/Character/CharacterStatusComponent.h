// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

//Ability연동을 위한 헤더
#include "GameplayAbilitySpecHandle.h"
#include "ActiveGameplayEffectHandle.h"
#include "AbilitySystemInterface.h"

//EquipmentSlot을 위한 헤더
#include "Items/Slot/ItemSlot.h"
#include "Items/ItemBase.h"

#include "CharacterStatusComponent.generated.h"


class UGameplayEffect;


UCLASS()
class UEquipmentSlot : public UItemSlot
{
	GENERATED_BODY()

public:
	UEquipmentSlot() : UItemSlot("Equipment")
	{}

private:
	EEquipmentPart Part;

	TWeakObjectPtr<class UAbilitySystemComponent> BindedASC;

	FGameplayAbilitySpecHandle AbilitySpecHandle;
	FActiveGameplayEffectHandle StatusUpEffectHandle;

private:
	virtual bool CanDropFrom(const UItemSlot* From) const;
	virtual bool CanDropTo(const UItemSlot* To) const;

	virtual bool CanItemInSlot(UItemBase* InItem) const;

	void ApplyItemEffects();

	void RemoveItemEffects();

	virtual void SlotDropTo(UItemSlot* To) override;

protected:
	virtual void NativeOnChangedItemInSlot(UItemBase* PrevItem) override;

public:
	void SetEquipmentPart(EEquipmentPart NewPart) { Part = NewPart; }

	FGameplayAbilitySpecHandle GetAbilitySpecHandle() const { return AbilitySpecHandle; }
	void SetAbilitySpecHandle(FGameplayAbilitySpecHandle InHandle) { AbilitySpecHandle = InHandle; }

	FActiveGameplayEffectHandle GetStatusUpEffect() const { return StatusUpEffectHandle; }
	void SetStatusUpEffect(FActiveGameplayEffectHandle InHandle) {StatusUpEffectHandle = InHandle;}

	void ClearHandles() { AbilitySpecHandle = FGameplayAbilitySpecHandle(); StatusUpEffectHandle = FActiveGameplayEffectHandle(); }

	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	void BindAbilitySystemComponent(UAbilitySystemComponent* InASC);

	EEquipmentPart GetEquipmentPart() const { return Part;}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONPORTFOLIO_API UCharacterStatusComponent : public UActorComponent, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TArray<TObjectPtr<UEquipmentSlot>> EquipmentSlots;

public:
	UFUNCTION(BlueprintPure, Category = "Equipment")
	UEquipmentSlot* GetEquipmentSlot(EEquipmentPart Part) const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void GetEquipmentSlots(TArray<UEquipmentSlot*>& Out);
};
