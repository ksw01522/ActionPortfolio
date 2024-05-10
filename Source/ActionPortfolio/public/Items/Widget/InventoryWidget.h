// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
 class SInventoryWindow;
 class UItemSlot;
 enum class EItemType : uint8;
 enum class EItemGrade : uint8;

 DECLARE_MULTICAST_DELEGATE_OneParam(FOnFocusedItemSlot, UItemSlot*);

UCLASS(BlueprintType)
class ACTIONPORTFOLIO_API UInventoryWidget : public UWidget
{
	GENERATED_BODY()

	UInventoryWidget();

private:
	TSharedPtr<SInventoryWindow> InventorySlate;

#if WITH_EDITORONLY_DATA
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview", meta = (AllowPrivateAccess = "true"))
	int Preview_InventorySize;
#endif

	mutable FOnFocusedItemSlot OnFocusedItemSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Slot|Background", meta = (AllowPrivateAccess = "true"))
	FSlateBrush SlotBackgroundBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Slot", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	int SlotCountByRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Slot", meta = (AllowPrivateAccess = "true"))
	FMargin SlotPadding;

	TArray<class UInventorySlot*> InSlots;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual void SynchronizeProperties() override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

#if WITH_ACCESSIBILITY
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override;
#endif

private:
	UFUNCTION()
	void OnNativeFocusedItemSlot(UItemSlot* InSlot); 

public:
	void SetSlotCountByRow(int NewCount);

	FOnFocusedItemSlot& GetOnFocusedItemSlot() const { return OnFocusedItemSlot; }

	void BindInventorySlots(TArray<class UInventorySlot*> InventorySlots);
};
