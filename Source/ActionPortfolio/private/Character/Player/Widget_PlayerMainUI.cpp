// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Widget_PlayerMainUI.h"
#include "Character/Player/PlayerCharacter.h"
#include "Ability/ActionPFAbilitySystemComponent.h"

#include "Items/ItemBase.h"
#include "Items/Widget/InventoryWidget.h"

#include "ActionPortfolio.h"
#include "Components/CanvasPanel.h"

void UWidget_PlayerMainUI::NativeConstruct()
{
	Super::NativeConstruct();

}

void UWidget_PlayerMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}



