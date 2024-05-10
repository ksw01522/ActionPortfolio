// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillHotKeyWindow.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API USkillHotKeyWindow : public UUserWidget
{
	GENERATED_BODY()
	

private:
	UPROPERTY(Transient)
	TMap<int, class UAbilityHotKeySlotWidget*> AbilitySlots;


	TWeakObjectPtr<class APlayerCharacter> OwnerCharacter;

protected:
	virtual void NativeConstruct() override;

public:

	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void SetAbilitySlots(TMap<int, UAbilityHotKeySlotWidget*> InSlots);

	UAbilityHotKeySlotWidget* GetAbilitySlot(int InputID);
	void ClearWindow();
	void SetAbilityIcon(int InputID, const TSharedPtr<class SAbilityIcon>& InIcon);

	void SetOwnerCharacter(APlayerCharacter* InOwner);
};
