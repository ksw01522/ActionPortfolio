// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Euipment/EquipmentAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ActionPortfolio.h"

UEquipmentAbility::UEquipmentAbility()
{
	AbilityType = EAbilityType::Passive;
}

void UEquipmentAbility::MustActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
#if WITH_EDITOR
//디버깅용 로그
	FString DebugString = FString::Printf(TEXT("%s Has Equipment Events :"), *GetName());

	if (EquipmentEventFlag & (uint8)EEquipmentEventFlag::OnAttackStart)
	{
		DebugString.Append(FString(" OnAttackStart"));
	}

	if (EquipmentEventFlag & (uint8)EEquipmentEventFlag::OnAttackTarget)
	{
		DebugString.Append(FString(", OnAttackTarget"));
	}

	if (EquipmentEventFlag & (uint8)EEquipmentEventFlag::OnDamaged)
	{
		DebugString.Append(FString(", OnDamaged"));
	}

	PFLOG(Warning, TEXT("%s"), *DebugString);

#endif


	if(EquipmentEventFlag & (uint8)EEquipmentEventFlag::OnAttackStart)
	{
		UAbilityTask_WaitGameplayEvent* OnAttackStartEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, OnAttackStartTag);
		OnAttackStartEvent->EventReceived.AddDynamic(this, &UEquipmentAbility::OnAttackStart);
		OnAttackStartEvent->ReadyForActivation();
	}

	if (EquipmentEventFlag & (uint8)EEquipmentEventFlag::OnAttackTarget)
	{
		UAbilityTask_WaitGameplayEvent* OnAttackTargetEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, OnAttackTargetTag);
		OnAttackTargetEvent->EventReceived.AddDynamic(this, &UEquipmentAbility::OnAttackTarget);
		OnAttackTargetEvent->ReadyForActivation();
	}

	if (EquipmentEventFlag & (uint8)EEquipmentEventFlag::OnDamaged)
	{
		UAbilityTask_WaitGameplayEvent* OnDamagedEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, OnDamagedTag);
		OnDamagedEvent->EventReceived.AddDynamic(this, &UEquipmentAbility::OnDamaged);
		OnDamagedEvent->ReadyForActivation();
	}
}
