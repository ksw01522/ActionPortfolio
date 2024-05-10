// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Equipment/TestEquipmentAbility.h"
#include "AbilitySystemGlobals.h"
#include "Ability/ActionPFAbilitySystemComponent.h"

UTestEquipmentAbility::UTestEquipmentAbility()
{
	EquipmentEventFlag = (uint8)EEquipmentEventFlag::OnAttackTarget;
}

void UTestEquipmentAbility::OnAttackTarget_Implementation(FGameplayEventData EventData)
{
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EventData.Target);

	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), OnAttackTargetEffectClass);
	ASC->ApplyGameplayEffectToSelf(Effect, 1, ASC->MakeEffectContext());
}
