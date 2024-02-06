// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Widget_PlayerMainUI.h"
#include "Character/Player/PlayerCharacter.h"
#include "Ability/Widget_AbilityNode.h"
#include "Ability/ActionPFAbilitySystemComponent.h"

#include "Items/ItemBase.h"
#include "Items/Widget/InventoryWidget.h"

#include "ActionPortfolio.h"

void UWidget_PlayerMainUI::NativeConstruct()
{
	Super::NativeConstruct();
#if WITH_EDITOR
	PFLOG(Log, TEXT("Try Construct Player Main UI"));
#endif

	LinkASC();
#if WITH_EDITOR
	PFLOG(Log, TEXT("Complete Construct Player Main UI"));
#endif

}

void UWidget_PlayerMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}




void UWidget_PlayerMainUI::LinkASC()
{
#if WITH_EDITOR
	PFLOG(Log, TEXT("Try Link ASC With Ability Nodes"));
#endif

	APlayerCharacter* OwningPlayer = GetOwningPlayerPawn<APlayerCharacter>();
	if (IsValid(OwningPlayer))
	{
		//Ability Node Initialize
		UActionPFAbilitySystemComponent* AbilitySystem = Cast<UActionPFAbilitySystemComponent>(OwningPlayer->GetAbilitySystemComponent());

		AbilityRMBNode->LinkWithAbilitySystem(AbilitySystem);
		AbilityRMBNode->AllocateAbility(OwningPlayer->GetPlayerAbilityClass(EPlayerAbilityInputID::Ability_RMB));

		AbilityENode->LinkWithAbilitySystem(AbilitySystem);
		AbilityENode->AllocateAbility(OwningPlayer->GetPlayerAbilityClass(EPlayerAbilityInputID::Ability_E));

		AbilityQNode->LinkWithAbilitySystem(AbilitySystem);
		AbilityQNode->AllocateAbility(OwningPlayer->GetPlayerAbilityClass(EPlayerAbilityInputID::Ability_Q));

		AbilityRNode->LinkWithAbilitySystem(AbilitySystem);
		AbilityRNode->AllocateAbility(OwningPlayer->GetPlayerAbilityClass(EPlayerAbilityInputID::Ability_R));

#if WITH_EDITOR
		PFLOG(Log, TEXT("Complete Link ASC With Ability Nodes"));
#endif
	}
#if WITH_EDITOR
	else {
		PFLOG(Error, TEXT("Can't Link ASC With Ability Nodes"));
	}
#endif
}

void UWidget_PlayerMainUI::UpdateInventorySlot(EItemType InventoryType, int idx, TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount)
{
	InventoryWidget->UpdateSlotWidget(InventoryType, idx, NewImage, ItemGrade, NewCount);
}

