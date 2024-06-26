// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ActionPFAbilityBFL.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "ActionPortfolio.h"
#include "AbilitySystemGlobals.h"
#include "Ability/AbilitySlot.h"

FGameplayEventData UActionPFAbilityBFL::MakeEventDataForAbility(AActor* instigator, TArray<AActor*> TargetArray)
{
	FGameplayEventData ReturnEvent;

	ReturnEvent.Instigator = instigator;
	FGameplayAbilityTargetData_ActorArray* NewTargetArray = new FGameplayAbilityTargetData_ActorArray();
	for (auto Target : TargetArray)
	{
		NewTargetArray->TargetActorArray.Add(Target);
	}

	ReturnEvent.TargetData.Add(NewTargetArray);

	return ReturnEvent;
}

UActionPFAbilitySystemComponent* UActionPFAbilityBFL::GetAbilitySystemComponent(AActor* Target)
{
	if(!IsValid(Target)) return nullptr;

	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Target);
	if(ASI == nullptr) return nullptr;

	return Cast<UActionPFAbilitySystemComponent>(ASI->GetAbilitySystemComponent());
}

bool UActionPFAbilityBFL::TryChangeAbilitySlot(UAbilitySlot* From, UAbilitySlot* To)
{
	if(From == nullptr || To == nullptr) return false;

	bool bCanChange = From->CanChangeSlotTo(To) && To->CanChangeSlotFrom(From);
	if (bCanChange)
	{
		From->ChangeSlotWithOther(To);
		To->ChangeSlotWithOther(From);

		From->ApplyChangeSlot();
		To->ApplyChangeSlot();
	}

	return bCanChange;
}
