// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/ActionPFAbilityBFL.h"
#include "Items/Euipment/EquipmentAbility.h"

bool UItemBase::TryUseItem(AActionPortfolioCharacter* User)
{
	if (!CanUseItem(User)) return false;

	OnUsedItem(User);
	return true;
}


bool UItemBase_Equipment::CanUseItem(AActionPortfolioCharacter* User) const
{
	UActionPFAbilitySystemComponent* UserASC = UActionPFAbilityBFL::GetAbilitySystemComponent(User);
	if(UserASC == nullptr) return false;

	return true;
}

void UItemBase_Equipment::OnUsedItem(AActionPortfolioCharacter* User)
{
	UActionPFAbilitySystemComponent* UserASC = UActionPFAbilityBFL::GetAbilitySystemComponent(User);

	UserASC->GiveAbilityAndActivateOnce()
}


