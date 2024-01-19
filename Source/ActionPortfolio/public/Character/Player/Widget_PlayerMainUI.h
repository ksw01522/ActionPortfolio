// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_PlayerMainUI.generated.h"

/**
 * 
 */
 enum class EItemType : uint8;
 enum class EItemGrade : uint8;

UCLASS(Abstract)
class ACTIONPORTFOLIO_API UWidget_PlayerMainUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	class UWidget_AbilityNode* AbilityRMBNode;

	UPROPERTY(meta = (BindWidget))
	class UWidget_AbilityNode* AbilityQNode;

	UPROPERTY(meta = (BindWidget))
	class UWidget_AbilityNode* AbilityENode;

	UPROPERTY(meta = (BindWidget))
	class UWidget_AbilityNode* AbilityRNode;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	class UInventoryWidget* InventoryWidget;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void LinkASC();

	void UpdateInventorySlot(EItemType InventoryType, int idx, TSoftObjectPtr<UMaterialInterface> NewImage, EItemGrade ItemGrade, int NewCount);

};
