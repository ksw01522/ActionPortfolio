// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
 class SInventory;
 enum class EItemType : uint8;
 enum class EItemGrade : uint8;

UCLASS(BlueprintType)
class ACTIONPORTFOLIO_API UInventoryWidget : public UWidget
{
	GENERATED_BODY()

	UInventoryWidget();

private:
	TSharedPtr<SInventory> InventorySlate;

#if WITH_EDITORONLY_DATA
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview", meta = (AllowPrivateAccess = "true"))
	int Preview_InventorySize;
	
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	FMargin WindowPadding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Slot|Background", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UMaterialInterface> SlotBackgroundMaterial_Equip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Slot|Background", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UMaterialInterface> SlotBackgroundMaterial_Consum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Slot|Background", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UMaterialInterface> SlotBackgroundMaterial_Mat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Slot", meta = (AllowPrivateAccess = "true"))
	FVector2D SlotSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Slot", meta = (AllowPrivateAccess = "true"))
	FVector2D InnerSlotPadding;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual void SynchronizeProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual const FText GetPaletteCategory() override;
#endif

#if WITH_ACCESSIBILITY
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override;
#endif

public:
	void UpdateSlotWidget(EItemType InventoryType, int idx, TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount);

};
