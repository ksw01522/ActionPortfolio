// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilitySlot.h"
#include "AbilitySlotWithInput.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UAbilitySlotWithInput : public UAbilitySlot
{
	GENERATED_BODY()
	
public:
	virtual ~UAbilitySlotWithInput();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	int InputID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputAction> InputAction;

	TWeakObjectPtr<class APlayerCharacter> OwnerPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	FName KeyboardCustomMappingKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	FName GamepadCustomMappingKey;

	FDelegateHandle OnChangedMappingHandle;

	FKey KeyboardHotKey;
	FKey GamepadHotKey;

private:
	virtual void BringAbilityIcon() override;

protected:
	virtual bool PreSetAbilityClass(TSubclassOf<class UActionPFGameplayAbility> InClass) override;
	virtual void PostSetAbilityClass() override;

private:
	void OnChangedMapping(const FName& InCode, const FKey& InKey);

public:
	virtual bool CanChangeSlotFrom(UAbilitySlot* From) const override;
	virtual bool CanChangeSlotTo(UAbilitySlot* To) const override;

	UInputAction* GetAbilityInputAction() const { return InputAction.Get(); }

	//virtual void ChangeSlotWithOther(UAbilitySlot* Other);
	//virtual void ApplyChangeSlot() override;

	int GetInputID() const { return InputID; }

	void SetOwnerPlayer(APlayerCharacter* InOwner);

	void SetKeyboardHotKeyDirect(const FKey& InKey);
	void SetGamepadHotKeyDirect(const FKey& InKey);
	void SetHotKeysDirect(const FKey& InKeyboardKey, const FKey& InGamepadKey);

	const FName& GetKeyboardMappingKey() const { return KeyboardCustomMappingKey;}
	const FName& GetGamepadMappingKey() const { return GamepadCustomMappingKey; }

	virtual void LinkAbilitySlotSlate(const TSharedPtr<SAbilitySlot>& InSlot) override;;
};
