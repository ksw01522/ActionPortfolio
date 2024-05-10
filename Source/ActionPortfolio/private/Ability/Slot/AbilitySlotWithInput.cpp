// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Slot/AbilitySlotWithInput.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "Character/Player/PlayerCharacter.h"
#include "Ability/Widget/SAbilitySlot.h"
#include "Ability/Widget/SAbilityIcon.h"
#include "ActionPortfolio.h"
#include "CustomInputSettingSubsystem.h"

UAbilitySlotWithInput::~UAbilitySlotWithInput()
{
	if (UCustomInputSettingSubsystem* CISS = UCustomInputSettingSubsystem::GetInstance())
	{
		CISS->RemoveOnChangedMappingEvent(OnChangedMappingHandle);
	}
	
	
}

void UAbilitySlotWithInput::BringAbilityIcon()
{
	if (!OwnerPlayer.IsValid() || !SlotSlate.IsValid()) { return; }
	SlotSlate.Pin()->SetAbilityIcon( OwnerPlayer->GetGivenAbilityIcon(AbilityClass));
}

bool UAbilitySlotWithInput::PreSetAbilityClass(TSubclassOf<class UActionPFGameplayAbility> InClass)
{
	if(!OwnerPlayer.IsValid() || (InClass.Get() != nullptr && InClass.GetDefaultObject()->GetAbilityType() != EAbilityType::Active)) return false;


	if (AbilityClass.Get() != nullptr)
	{
		if(FGameplayAbilitySpec* AbilitySpec = OwnerPlayer->GetAbilitySystemComponent()->FindAbilitySpecFromClass(AbilityClass))
		{
			AbilitySpec->InputID = -1;
			AbilitySpec->InputPressed = false;
		}

	}

	return true;
}

void UAbilitySlotWithInput::PostSetAbilityClass()
{
	if (AbilityClass.Get() != nullptr)
	{
		if (FGameplayAbilitySpec* AbilitySpec = OwnerPlayer->GetAbilitySystemComponent()->FindAbilitySpecFromClass(AbilityClass))
		{
			AbilitySpec->InputID = InputID;
		}
	}
}

void UAbilitySlotWithInput::OnChangedMapping(const FName& InCode, const FKey& InKey)
{
	if(!SlotSlate.IsValid()) return;

	if (InCode == KeyboardCustomMappingKey)
	{
		SetKeyboardHotKeyDirect(InKey);
	}
	else if (InCode == GamepadCustomMappingKey)
	{
		SetGamepadHotKeyDirect(InKey);
	}

}

bool UAbilitySlotWithInput::CanChangeSlotFrom(UAbilitySlot* From) const
{
	if(From == nullptr) return false;

	if (UActionPFGameplayAbility* AbilityObj = From->GetAbilityClass().GetDefaultObject())
	{
		return AbilityObj->GetAbilityType() == EAbilityType::Active;
	}

	return false;
}

bool UAbilitySlotWithInput::CanChangeSlotTo(UAbilitySlot* To) const
{
	return Cast<UAbilitySlotWithInput>(To) != nullptr;
}

void UAbilitySlotWithInput::SetOwnerPlayer(APlayerCharacter* InOwner)
{
	OwnerPlayer = InOwner;
	BringAbilityIcon();
}

void UAbilitySlotWithInput::SetKeyboardHotKeyDirect(const FKey& InKey)
{
	KeyboardHotKey = InKey;

	if (!SlotSlate.IsValid()) return;
	TSharedPtr<SAbilitySlot_HotKey> TempHotKeySlot = StaticCastSharedPtr<SAbilitySlot_HotKey>(SlotSlate.Pin());

	TempHotKeySlot->SetHotKeyKeyboard(KeyboardHotKey);
}

void UAbilitySlotWithInput::SetGamepadHotKeyDirect(const FKey& InKey)
{
	GamepadHotKey = InKey;

	if (!SlotSlate.IsValid()) return;
	TSharedPtr<SAbilitySlot_HotKey> TempHotKeySlot = StaticCastSharedPtr<SAbilitySlot_HotKey>(SlotSlate.Pin());

	TempHotKeySlot->SetHotKeyGamepad(GamepadHotKey);
}

void UAbilitySlotWithInput::SetHotKeysDirect(const FKey& InKeyboardKey, const FKey& InGamepadKey)
{
	KeyboardHotKey = InKeyboardKey;
	GamepadHotKey = InGamepadKey;

	if (!SlotSlate.IsValid()) return;
	TSharedPtr<SAbilitySlot_HotKey> TempHotKeySlot = StaticCastSharedPtr<SAbilitySlot_HotKey>(SlotSlate.Pin());

	TempHotKeySlot->SetHotKeys(KeyboardHotKey, GamepadHotKey);
}

void UAbilitySlotWithInput::LinkAbilitySlotSlate(const TSharedPtr<SAbilitySlot>& InSlot)
{
	if(SlotSlate == InSlot) return;
	Super::LinkAbilitySlotSlate(InSlot);

	TSharedPtr<SAbilitySlot_HotKey> TempHotKeySlot = StaticCastSharedPtr<SAbilitySlot_HotKey>(SlotSlate.Pin());

	TempHotKeySlot->SetHotKeys(KeyboardHotKey, GamepadHotKey);
}
