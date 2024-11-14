// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equipment/Item_Equipment.h"
#include "Items/Equipment/EquipmentAbility.h"
#include "Ability/ActionPFAttributeSet.h"
#include "Items/ItemManagerSubsystem.h"
#include "Items/Equipment/DefaultEquipmentEffect.h"

#include "ActionPortfolio.h"

bool UItemBase_Equipment::CanEquipItem(UActionPFAbilitySystemComponent* ASC) const
{
	return EquipmentAbility.GetDefaultObject()->DoesAbilitySatisfyTagRequirements(*ASC);
}

bool UItemBase_Equipment::IsSame(const UItemBase* Other) const
{
	return this == Other;
}


FGameplayEffectSpec UItemBase_Equipment::MakeAddStatusEffect(const FGameplayEffectContextHandle& InEffectContext) const
{
	//기본 스테이터스 증가 Effect 만들기
	FGameplayEffectSpec ReturnSpec(GetDefault<UDefaultEquipmentEffect>(), InEffectContext);
	ReturnSpec.SetSetByCallerMagnitude(FName("MaxHP"), MaxHP);
	ReturnSpec.SetSetByCallerMagnitude(FName("MaxStamina"), MaxStamina);
	ReturnSpec.SetSetByCallerMagnitude(FName("AttackP"), AttackP);
	ReturnSpec.SetSetByCallerMagnitude(FName("DefenseP"), DefenseP);

	return ReturnSpec;
}


void UItemBase_Equipment::InitializeItem(const FName& NewItemCode, const FItemData_Base* Data, TArray<FText>& OutErroMsgs)
{
	if (Data == nullptr || NewItemCode.IsNone() || Data->ItemType != EItemType::Equipment) return;
\
	const FItemData_Equipment* EquipmentData = static_cast<const FItemData_Equipment*>(Data);

	Super::InitializeItem(NewItemCode, Data, OutErroMsgs);

	UItemManagerSubsystem* IMS = UItemManagerSubsystem::GetManagerInstance();

	EquipmentAbility = IMS->FindEquipmentAbility(EquipmentData->AbilityCode);

	EquipmentPart = EquipmentData->EquipmentPart;

	MaxHP = EquipmentData->MaxHP;

	MaxStamina = EquipmentData->MaxStamina;

	AttackP = EquipmentData->AttackP;

	DefenseP = EquipmentData->DefenseP;
}


