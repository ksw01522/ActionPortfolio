// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Slot/AbilitySlot_HotKey.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "Character/Player/PlayerCharacter.h"
#include "Ability/Widget/SAbilitySlot.h"
#include "Ability/Widget/SAbilityIcon.h"
#include "ActionPortfolio.h"
#include "CustomInputSettingSubsystem.h"

UAbilitySlot_HotKey::UAbilitySlot_HotKey() : UAbilitySlot(FName("HotKey"))
{
}

UAbilitySlot_HotKey::~UAbilitySlot_HotKey()
{

}

bool UAbilitySlot_HotKey::CanSetAbilityClass(TSubclassOf<class UActionPFGameplayAbility> InClass)
{
	if (UActionPFGameplayAbility* AbilityCDO = InClass.GetDefaultObject())
	{
		return AbilityCDO->GetAbilityType() == EAbilityType::Active;
	}
	
	return true;
}
