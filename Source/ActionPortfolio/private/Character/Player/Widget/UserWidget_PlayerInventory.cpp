// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Widget/UserWidget_PlayerInventory.h"
#include "Items/Widget/InventoryWidget.h"
#include "Items/Widget/ItemInformationWidget.h"
#include "Items/Slot/ItemSlot.h"
#include "Character/Player/Widget/CharacterStatusWidget.h"
#include "Character/Player/InventoryComponent.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "ActionPortfolio.h"
#include "Character/Player/ActionPFPlayerController.h"


void UUserWidget_PlayerInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (AActionPFPlayerController* PlayerController = GetOwningPlayer<AActionPFPlayerController>())
	{
		Inventory->GetOnFocusedItemSlot().AddUObject(this, &UUserWidget_PlayerInventory::OnFocusedItemSlot);
		Status->GetOnFocusedEquipmentSlot().AddUObject(this, &UUserWidget_PlayerInventory::OnFocusedItemSlot);

		ItemInformation->UpdateItemInformation(nullptr);
		Inventory->BindInventorySlots(PlayerController->GetInventory()->GetSlotArray());
		Status->BindStatusComponent(PlayerController->GetStatusComponent());
	}
}

void UUserWidget_PlayerInventory::NativeConstruct()
{
	Super::NativeConstruct();

}

FReply UUserWidget_PlayerInventory::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FSlateApplication& SlateApp = FSlateApplication::Get();

	if (SlateApp.GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Back)
	{
		if (AActionPFPlayerController* PlayerController = GetOwningPlayer<AActionPFPlayerController>())
		{
			PlayerController->CloseInventory();
		}
		return FReply::Handled();
	}

	return FReply::Unhandled();
}
	

void UUserWidget_PlayerInventory::OnFocusedItemSlot(UItemSlot* FocusedSlot)
{
#if WITH_EDITOR
	if(GetOwningPlayer() == nullptr) return;
#endif
	ItemInformation->UpdateItemInformation(FocusedSlot->GetItem());
}


void UUserWidget_PlayerInventory::BindInventorySlots(TArray<UInventorySlot*> InventorySlots)
{
	Inventory->BindInventorySlots(InventorySlots);
}

void UUserWidget_PlayerInventory::BindStatusComponent(UCharacterStatusComponent* InCSC)
{
	Status->BindStatusComponent(InCSC);
}



UInventoryWidget* UUserWidget_PlayerInventory::GetInventoryWidget() const
{
	return Inventory;
}

UCharacterStatusWidget* UUserWidget_PlayerInventory::GetCharacterStatusWidget() const
{
	return Status;
}
