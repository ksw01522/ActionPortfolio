// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomInputDeveloperSettings.h"
#include "CustomInputSettingRuntime.h"
#include "Widgets/Images/SImage.h"

#define LOCTEXT_NAMESPACE "CustomInputSetting"

UCustomInputDeveloperSettings::UCustomInputDeveloperSettings()
{
	NavigationKeys.FindOrAdd(EUINavigation::Up);
	NavigationKeys.FindOrAdd(EUINavigation::Down);
	NavigationKeys.FindOrAdd(EUINavigation::Left);
	NavigationKeys.FindOrAdd(EUINavigation::Right);
	NavigationKeys.FindOrAdd(EUINavigation::Previous);
	NavigationKeys.FindOrAdd(EUINavigation::Next);

	NavigationActionKeys.FindOrAdd(EUINavigationAction::Accept);
	NavigationActionKeys.FindOrAdd(EUINavigationAction::Back);
}

#if WITH_EDITOR
void UCustomInputDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	
	/*
	LOG_WARNING(TEXT("Property Name                    : %s"), *PropertyChangedEvent.GetPropertyName().ToString());
	LOG_WARNING(TEXT("Member Property Name             : %s"), *PropertyChangedEvent.GetMemberPropertyName().ToString());
	LOG_WARNING(TEXT("ChangeType                       : %d"), PropertyChangedEvent.ChangeType);
	LOG_WARNING(TEXT("Array Index by Property          : %d"), PropertyChangedEvent.GetArrayIndex(PropertyChangedEvent.GetPropertyName().ToString()));
	LOG_WARNING(TEXT("Array Index by Member Property   : %d"), PropertyChangedEvent.GetArrayIndex(PropertyChangedEvent.GetMemberPropertyName().ToString()));
	LOG_WARNING(TEXT("Get Num Objects Being Edited     : %d"), PropertyChangedEvent.GetNumObjectsBeingEdited());
	*/

	if (PropertyChangedEvent.GetMemberPropertyName() == "NavigationKeys")
	{
		if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd ||
			PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear ||
			PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayMove ||
			PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove ||
			PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate)
		{
			NavigationKeys.FindOrAdd(EUINavigation::Up);
			NavigationKeys.FindOrAdd(EUINavigation::Down);
			NavigationKeys.FindOrAdd(EUINavigation::Left);
			NavigationKeys.FindOrAdd(EUINavigation::Right);
			NavigationKeys.FindOrAdd(EUINavigation::Previous);
			NavigationKeys.FindOrAdd(EUINavigation::Next);

			NavigationKeys.Remove(EUINavigation::Num);
			NavigationKeys.Remove(EUINavigation::Invalid);
		}

		CheckNavigationKey();
	}
	else if (PropertyChangedEvent.GetMemberPropertyName() == "NavigationActionKeys")
	{
		if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd ||
			PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear ||
			PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayMove ||
			PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove ||
			PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate)
		{
			NavigationActionKeys.FindOrAdd(EUINavigationAction::Accept);
			NavigationActionKeys.FindOrAdd(EUINavigationAction::Back);

			NavigationActionKeys.Remove(EUINavigationAction::Num);
			NavigationActionKeys.Remove(EUINavigationAction::Invalid);
		}

		CheckNavigationKey();
	}
	else if (PropertyChangedEvent.GetMemberPropertyName() == "NavigationOptionalActionKeys")
	{
		CheckNavigationKey();
	}
}

void UCustomInputDeveloperSettings::CheckNavigationKey()
{
	//Key : Key.ToString(), Value : Navigation Enum Name
	TMap<FString, TArray<FString>> TempKeyboardTable;
	TMap<FString, TArray<FString>> TempGamepadTable;
	
	//TArray<Navigation Enum Name>
	TArray<FString> NoneTable_Keyboard;
	TArray<FString> NoneTable_Gamepad;

	//Check
	{
		//Navigation Direction Key Check
		for (auto& CheckKeySetting : NavigationKeys)
		{
			FKey& KeyboardKey = CheckKeySetting.Value.Keyboard;
			FKey& GamepadKey = CheckKeySetting.Value.Gamepad;
			UEnum* NavigationEnum = StaticEnum<EUINavigation>();
			FString NavigationName = NavigationEnum->GetNameStringByValue(static_cast<int64>(CheckKeySetting.Key));
			
			if (!KeyboardKey.IsValid())
			{
				NoneTable_Keyboard.Add(NavigationName);
			}
			else if (KeyboardKey.IsGamepadKey())
			{
				KeyboardKey = FKey();
				NoneTable_Keyboard.Add(NavigationName);
			}
			else
			{
				TempKeyboardTable.FindOrAdd(KeyboardKey.ToString()).Add(NavigationName);
			}


			if (!GamepadKey.IsValid())
			{
				NoneTable_Gamepad.Add(NavigationName);
			}
			else if (!GamepadKey.IsGamepadKey())
			{
				GamepadKey = FKey();
				NoneTable_Gamepad.Add(NavigationName);
			}
			else
			{
				TempGamepadTable.FindOrAdd(GamepadKey.ToString()).Add(NavigationName);
			}
		}

		//Navigation Action Key Check
		for (auto& CheckKeySetting : NavigationActionKeys)
		{
			FKey& KeyboardKey = CheckKeySetting.Value.Keyboard;
			FKey& GamepadKey = CheckKeySetting.Value.Gamepad;
			UEnum* NavigationActionEnum = StaticEnum<EUINavigationAction>();
			FString NavigationName = NavigationActionEnum->GetNameStringByValue(static_cast<int64>(CheckKeySetting.Key));

			if (!KeyboardKey.IsValid())
			{
				NoneTable_Keyboard.Add(NavigationName);
			}
			else if (KeyboardKey.IsGamepadKey())
			{
				KeyboardKey = FKey();
				NoneTable_Keyboard.Add(NavigationName);
			}
			else
			{
				TempKeyboardTable.FindOrAdd(KeyboardKey.ToString()).Add(NavigationName);
			}


			if (!GamepadKey.IsValid())
			{
				NoneTable_Gamepad.Add(NavigationName);
			}
			else if (!GamepadKey.IsGamepadKey())
			{
				GamepadKey = FKey();
				NoneTable_Gamepad.Add(NavigationName);
			}
			else
			{
				TempGamepadTable.FindOrAdd(KeyboardKey.ToString()).Add(NavigationName);
			}
		}

		//Navagation Optional Key Check
		for (auto& CheckKeySetting : NavigationOptionalActionKeys)
		{
			FKey& KeyboardKey = CheckKeySetting.Value.Keyboard;
			FKey& GamepadKey = CheckKeySetting.Value.Gamepad;
			FString ActionName = CheckKeySetting.Key.ToString();

			if (!KeyboardKey.IsValid())
			{
				NoneTable_Keyboard.Add(ActionName);
			}
			else if (KeyboardKey.IsGamepadKey())
			{
				KeyboardKey = FKey();
				NoneTable_Keyboard.Add(ActionName);
			}
			else
			{
				TempKeyboardTable.FindOrAdd(KeyboardKey.ToString()).Add(ActionName);
			}


			if (!GamepadKey.IsValid())
			{
				NoneTable_Gamepad.Add(ActionName);
			}
			else if (!GamepadKey.IsGamepadKey())
			{
				GamepadKey = FKey();
				NoneTable_Gamepad.Add(ActionName);
			}
			else
			{
				TempGamepadTable.FindOrAdd(KeyboardKey.ToString()).Add(ActionName);
			}
		}
	} //end Section : Check

	//Duplication Key Error
	{
		FString KeyArrayString;

		for (auto& TempKeyboard : TempKeyboardTable)
		{
			int TempNum = TempKeyboard.Value.Num();
			if(TempNum <= 1) continue;

			for (int i = 0; i < TempNum; i++)
			{
				KeyArrayString += ' ';
				KeyArrayString.Append(TempKeyboard.Value[i]);
			}

			LOG_ERROR(TEXT("Duplication Keyboard Key %s :%s"), *TempKeyboard.Key, *KeyArrayString);
			KeyArrayString.Empty();
		}

		for (auto& TempGamepad : TempGamepadTable)
		{
			int TempNum = TempGamepad.Value.Num();
			if (TempNum <= 1) continue;

			for (int i = 0; i < TempNum; i++)
			{
				KeyArrayString += ' ';
				KeyArrayString.Append(TempGamepad.Value[i]);
			}

			LOG_ERROR(TEXT("Duplication Gamepad Key %s :%s"), *TempGamepad.Key, *KeyArrayString);
			KeyArrayString.Empty();
		}
	}//End Section : Duplication Key Error

	//Invalidat Key Error
	{
		FString KeyArrayString;
		if (!NoneTable_Keyboard.IsEmpty())
		{
			for (const auto& NoneKey : NoneTable_Keyboard)
			{
				KeyArrayString += ' ';
				KeyArrayString.Append(NoneKey);
			}
			LOG_ERROR(TEXT("{NavigationKey}.{Keyboard} Invalidate Key :%s"), *KeyArrayString);
			KeyArrayString.Empty();
		}

		if (!NoneTable_Gamepad.IsEmpty())
		{
			for (const auto& NoneKey : NoneTable_Gamepad)
			{
				KeyArrayString += ' ';
				KeyArrayString.Append(NoneKey);
			}
			LOG_ERROR(TEXT("{NavigationKey}.{Gamepad} Invalidate Key :%s"), *KeyArrayString);
			KeyArrayString.Empty();
		}

	} //End Section : Invalidat Key Error

}

#endif

#undef LOCTEXT_NAMESPACE