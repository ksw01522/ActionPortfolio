// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "EquipmentAbility.generated.h"

/**
 * 
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EEquipmentEventFlag : uint8
{
	None = 0 UMETA(Hidden),
	OnAttackStart = 1 << 0,
	OnAttackTarget = 1 << 1,
	OnDamaged = 1 << 2
};
ENUM_CLASS_FLAGS(EEquipmentEventFlag)


UCLASS(Abstract, BlueprintType, Blueprintable)
class ACTIONPORTFOLIO_API UEquipmentAbility : public UActionPFGameplayAbility
{
	GENERATED_BODY()

	UEquipmentAbility();

protected:
	UPROPERTY(EditAnywhere, Category = "EquipmentEvent", meta = (Bitmask, BitmaskEnum = EEquipmentEventFlag))
	uint8 EquipmentEventFlag = 0;

protected:
	virtual void MustActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UFUNCTION(BlueprintNativeEvent)
	void OnAttackStart(FGameplayEventData EventData);
	virtual void OnAttackStart_Implementation(FGameplayEventData EventData) {};

	UFUNCTION(BlueprintNativeEvent)
	void OnAttackTarget(FGameplayEventData EventData);
	virtual void OnAttackTarget_Implementation(FGameplayEventData EventData) {};

	UFUNCTION(BlueprintNativeEvent)
	void OnDamaged(FGameplayEventData EventData);
	virtual void OnDamaged_Implementation(FGameplayEventData EventData) {};

};

