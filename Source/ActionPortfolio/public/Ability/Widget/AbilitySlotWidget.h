// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "AbilitySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UAbilitySlotWidget : public UWidget
{
	GENERATED_BODY()
	
protected:
	TSharedPtr<class SAbilitySlot> AbilitySlot;

private:
	UPROPERTY(EditAnywhere, Category = "Style", meta = (AllowPrivateAccess = "true"))
	FSlateBrush BackgroundBrush;

	UPROPERTY(EditAnywhere, Category = "Style", meta = (AllowPrivateAccess = "true"))
	FVector2D SlotSize;

public:
	FVector2D GetSlotSize() const { return SlotSize; }

	const FSlateBrush* GetBackgroundBrush() const { return &BackgroundBrush; }

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual void SynchronizeProperties() override;

#if WITH_EDITOR
	//virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual const FText GetPaletteCategory() override { return NSLOCTEXT("ActionPFAbility", "AbilityCategory", "Ability"); }
#endif

#if WITH_ACCESSIBILITY
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override;
#endif

public:
	TSharedPtr<SAbilitySlot> GetSlotSlate() const { return AbilitySlot; }

	void SetAbilityIcon(const TSharedPtr<class SAbilityIcon>& InIcon);
};


UCLASS()
class ACTIONPORTFOLIO_API UAbilityHotKeySlotWidget : public UAbilitySlotWidget
{
	GENERATED_BODY()


protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

};