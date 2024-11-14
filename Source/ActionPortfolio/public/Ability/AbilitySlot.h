// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionPFGameplayAbility.h"
#include "AbilitySlot.generated.h"

/**
 * 
 */

 DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetAbilityInSlot, UAbilitySlot*);

UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew, Abstract)
class ACTIONPORTFOLIO_API UAbilitySlot : public UObject
{
	GENERATED_BODY()
	
public:
	UAbilitySlot();
	UAbilitySlot(FName InSlotType);

protected:
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<class UActionPFGameplayAbility> AbilityClass;

private:
	FOnSetAbilityInSlot OnSetAbilityDel;
	FName SlotType;

protected:
	virtual bool CanSetAbilityClass(TSubclassOf<class UActionPFGameplayAbility> InClass);
	virtual void PostSetAbilityClass() {}

public:
	FName GetSlotType() const {return SlotType;}

	void SetAbilityClass(TSubclassOf<class UActionPFGameplayAbility> InClass);

	virtual void SlotDropTo(UAbilitySlot* To) {}

public:
	FOnSetAbilityInSlot& GetAbilityChangedDelegate() {return OnSetAbilityDel;}

	TSubclassOf<UActionPFGameplayAbility> GetAbilityInSlot() const { return AbilityClass; }
};

