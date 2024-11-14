// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Equipment/EquipmentAbility.h"
#include "TestEquipmentAbility.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UTestEquipmentAbility : public UEquipmentAbility
{
	GENERATED_BODY()
public:
	UTestEquipmentAbility();

private:
	UPROPERTY(EditAnywhere, Category = "OnAttackTarget", meta = (Categories = ""))
	TSubclassOf<class UGameplayEffect> OnAttackTargetEffectClass;

protected:
	virtual void OnAttackTarget_Implementation(FGameplayEventData EventData) override;
};
