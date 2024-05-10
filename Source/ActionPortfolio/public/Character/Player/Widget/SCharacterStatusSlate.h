// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Ability/ActionPFAttributeSet.h"
#include "Items/Widget/SItemSlot.h"
/**
 * 
 */
 enum class EEquipmentPart : uint8;

struct FOnAttributeChangedDelegateStruct
{
	FOnAttributeChangedDelegateStruct(const FGameplayAttribute& InAttribute, const FDelegateHandle& InHandle);

	FGameplayAttribute Attribute;
	FDelegateHandle DelegateHandle;
};

class ACTIONPORTFOLIO_API SCharacterStatusSlate : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SCharacterStatusSlate, SCompoundWidget);
public:
	virtual ~SCharacterStatusSlate();

	SLATE_BEGIN_ARGS(SCharacterStatusSlate) : _SlotBackgroundBrush(nullptr), _SlotPadding(0), _TextStyle(nullptr), _TextBlockPadding(0), _BindStatus(nullptr)
	{}
	
	SLATE_ARGUMENT(const FSlateBrush*, SlotBackgroundBrush)
	SLATE_ARGUMENT(FMargin, SlotPadding)
	SLATE_ARGUMENT(const FTextBlockStyle*, TextStyle)
	SLATE_ARGUMENT(FMargin, TextBlockPadding)
	SLATE_ARGUMENT(class UCharacterStatusComponent*, BindStatus)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	mutable FOnFocusedItemSlot OnFocsedEqipmentSlot;

	TSharedPtr<SHorizontalBox> SlotBox;
	TMap<EEquipmentPart, TSharedPtr<class SItemSlot>> EquipmentSlots;

	TWeakObjectPtr<UCharacterStatusComponent> BindedStatus;
	TArray<FOnAttributeChangedDelegateStruct> Handles;

	TSharedPtr<STextBlock> HP;
	float CurrentHP, MaxHP;

	void OnHPValueChanged(const struct FOnAttributeChangeData& Data);
	void OnMaxHPValueChanged(const FOnAttributeChangeData& Data);
	void UpdateHPText();

	TSharedPtr<STextBlock> Stamina;
	float CurrentStamina, MaxStamina;
	void OnStaminaValueChanged(const FOnAttributeChangeData& Data);
	void OnMaxStaminaValueChanged(const FOnAttributeChangeData& Data);
	void UpdateStaminaText();

	TSharedPtr<STextBlock> Attack;
	void OnAttackValueChanged(const FOnAttributeChangeData& Data);
	void SetAttackText(float InPoint);

	TSharedPtr<STextBlock> Defense;
	void OnDefenseValueChanged(const FOnAttributeChangeData& Data);
	void SetDefenseText(float InPoint); 

	TSharedPtr<STextBlock> FireResistance;
	void OnFireResistanceValueChanged(const FOnAttributeChangeData& Data);
	void SetFireResistanceText(float InPoint);

	TSharedPtr<STextBlock> IceResistance;
	void OnIceResistanceValueChanged(const FOnAttributeChangeData& Data);
	void SetIceResistanceText(float InPoint);

	TSharedPtr<STextBlock> ElectricResistance;
	void OnElectricResistanceValueChanged(const FOnAttributeChangeData& Data);
	void SetElectricResistanceText(float InPoint);

private:
	void NativeOnFocusedEquipmentSlot(UItemSlot* FocusedSlot);

public:
	SItemSlot* GetEquipmentSlot(EEquipmentPart Part) const;

	void SetSlotBackgroundBrush(const FSlateBrush* InBrush);
	void SetSlotPadding(const FMargin& NewPadding);
	void SetTextStyle(const FTextBlockStyle* InStyle);
	void SetTextBlockPadding(const FMargin& NewPadding);


	FOnFocusedItemSlot& GetOnFocusedEquipmentSlot() const { return OnFocsedEqipmentSlot; }


private:
	void ClearBindComponent();

	void ResetTexts();

public:
	void BindStatusComponent(UCharacterStatusComponent* Target);

};
