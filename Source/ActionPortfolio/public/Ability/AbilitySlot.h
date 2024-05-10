// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionPFGameplayAbility.h"
#include "AbilitySlot.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew, Abstract)
class ACTIONPORTFOLIO_API UAbilitySlot : public UObject
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<class UActionPFGameplayAbility> AbilityClass;

	TSharedPtr<class SAbilityIcon> AbilityIcon;

	TWeakPtr<class SAbilitySlot> SlotSlate;

	TSubclassOf<UActionPFGameplayAbility> TempAbilityClass;

protected:
	virtual bool PreSetAbilityClass(TSubclassOf<class UActionPFGameplayAbility> InClass);
	virtual void PostSetAbilityClass() {}

public:
	void SetAbilityClass(TSubclassOf<class UActionPFGameplayAbility> InClass);

	const TSharedPtr<SAbilityIcon>& GetAbilityIcon() const {return AbilityIcon;}
	virtual void BringAbilityIcon();

public:
	virtual bool CanChangeSlotFrom(UAbilitySlot* From) const;
	virtual bool CanChangeSlotTo(UAbilitySlot* To) const;

	virtual void ChangeSlotWithOther(UAbilitySlot* Other);
	virtual void ApplyChangeSlot();

	virtual void ClearSlot();

	TSubclassOf<UActionPFGameplayAbility> GetAbilityClass() const { return AbilityClass; }

	virtual void LinkAbilitySlotSlate(const TSharedPtr<SAbilitySlot>& InSlot);
};

