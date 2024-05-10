// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_PlayerInventory.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UUserWidget_PlayerInventory : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UInventoryWidget> Inventory;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCharacterStatusWidget> Status;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UItemInformationWidget> ItemInformation;


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	void OnFocusedItemSlot(class UItemSlot* FocusedSlot);

public:
	void BindInventorySlots(TArray<class UInventorySlot*> InventorySlots);
	void BindStatusComponent(class UCharacterStatusComponent* InCSC);

	UInventoryWidget* GetInventoryWidget() const;
	UCharacterStatusWidget* GetCharacterStatusWidget() const;
};
