// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/AbilityChainNode.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

bool UAbilityChainNode::TryActivateAbility(UAbilitySystemComponent* InASC) const
{
	if(InASC == nullptr) return false;

	return InASC->TryActivateAbilityByClass(GetAbilityClass());
}

bool UAbilityChainNode::TryActivateAbility(IAbilitySystemInterface* InASI) const
{
	if(InASI == nullptr) return false;

	UAbilitySystemComponent* ASC = InASI->GetAbilitySystemComponent();
	return TryActivateAbility(ASC);
}
