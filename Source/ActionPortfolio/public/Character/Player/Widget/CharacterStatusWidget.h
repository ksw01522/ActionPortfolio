// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Character/Player/Widget/SCharacterStatusSlate.h"
#include "CharacterStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UCharacterStatusWidget : public UWidget
{
	GENERATED_BODY()

public:
	UCharacterStatusWidget();

private:
	TSharedPtr<class SCharacterStatusSlate> Slate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot", meta = (AllowPrivateAccess = "true"))
	FSlateBrush SlotBackgroundBrush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot", meta = (AllowPrivateAccess = "true"))
	FMargin SlotPadding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text", meta = (AllowPrivateAccess = "true"))
	FTextBlockStyle TextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text", meta = (AllowPrivateAccess = "true"))
	FMargin TextBlockPadding;

	mutable FOnFocusedItemSlot OnFocusedEqipmentSlot;

	TWeakObjectPtr<class UCharacterStatusComponent> BindedStatus;

public:
	FOnFocusedItemSlot& GetOnFocusedEquipmentSlot() const { return OnFocusedEqipmentSlot; }

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
	void NativeOnFocusedEuipmentSlot(class UItemSlot* FocusedSlot);

public:
	void BindStatusComponent(UCharacterStatusComponent* TargetStatus);
};
