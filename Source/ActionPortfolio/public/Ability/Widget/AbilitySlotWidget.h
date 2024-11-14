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
	TSharedPtr<class SAbilitySlot> SlotSlate;

	TWeakObjectPtr<class UAbilitySlot> LinkedSlot;

	TSharedPtr<class SAbilityIcon> AbilityIcon;

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
	TSharedPtr<SAbilitySlot> GetSlotSlate() const { return SlotSlate; }
	void LinkAbilitySlot(UAbilitySlot* InSlot);

	void SetAbilityIcon(const TSharedPtr<SAbilityIcon>& InIcon);
	SAbilityIcon* GetAbilityIcon() const;
};


UCLASS()
class ACTIONPORTFOLIO_API UAbilityHotKeySlotWidget : public UAbilitySlotWidget
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	FKey HotKey_Keyboard;
	FKey HotKey_Gamepad;

public:
	void SetHotKey_Keyboard(const FKey& InKey);
	void SetHotKey_Gamepad(const FKey& InKey);
};