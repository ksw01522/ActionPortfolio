// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilitySlot.h"
#include "AbilitySlot_HotKey.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UAbilitySlot_HotKey : public UAbilitySlot
{
	GENERATED_BODY()
	
public:
	UAbilitySlot_HotKey();
	virtual ~UAbilitySlot_HotKey();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputAction> HotKeyAction;

protected:
	virtual bool CanSetAbilityClass(TSubclassOf<class UActionPFGameplayAbility> InClass) override;

public:
	UInputAction* GetHotKeyAction() const { return HotKeyAction; }
};
