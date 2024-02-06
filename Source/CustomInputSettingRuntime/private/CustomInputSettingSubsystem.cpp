// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomInputSettingSubsystem.h"
#include "CustomInputDeveloperSettings.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"
#include "CustomInputSettingRuntime.h"
#include "Widget/SCustomInputSetting.h"
#include "Events/ChangeInputDevice.h"
#include "Events/ChangeMappingInputProcessor.h"
#include "Blueprint/UserWidget.h"

#define LOCTEXT_NAMESPACE "CustomInputSetting"

FCustomNavigationConfig::FCustomNavigationConfig() : FNavigationConfig()
{
	bTabNavigation = false;

}

EUINavigation FCustomNavigationConfig::GetNavigationDirectionFromKey(const FKeyEvent& InKeyEvent) const
{
	if (const EUINavigation* Rule = KeyEventRules.Find(InKeyEvent.GetKey()))
	{
		if (bKeyNavigation)
		{
			return *Rule;
		}
	}

	return EUINavigation::Invalid;
}

EUINavigationAction FCustomNavigationConfig::GetNavigationActionFromKey(const FKeyEvent& InKeyEvent) const
{
	if (const EUINavigationAction* Rule = KeyActionEventRules.Find(InKeyEvent.GetKey()))
	{
		if (bKeyNavigation)
		{
			return *Rule;
		}
	}

	return EUINavigationAction::Invalid;
}


UCustomInputSettingSubsystem* UCustomInputSettingSubsystem::SubsystemInstance = nullptr;

UCustomInputSettingSubsystem::UCustomInputSettingSubsystem()
{
	
}

void UCustomInputSettingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	SubsystemInstance = this;

	const UCustomInputDeveloperSettings* DevelopSetting = GetDefault<UCustomInputDeveloperSettings>();

	IgnoreMappingKeys = DevelopSetting->GetIgnoreMappingKeys();

	const UDataTable* MetaDataTable = DevelopSetting->GetKeySettingMetaDataTable().LoadSynchronous();

	//메타데이터 관련
	TArray<const FKeySettingMetaData*> MetaDatas;
	MetaDataTable->GetAllRows<const FKeySettingMetaData>("", MetaDatas);

	for (int i = 0; i < MetaDatas.Num(); i++)
	{
		KeyIcons.Emplace(MetaDatas[i]->Key.GetFName(), MetaDatas[i]->Icon);
	}


	//네비게이션
	FSlateApplication& SlateApplication = FSlateApplication::Get();

	CustomNavConfig = MakeShared<FCustomNavigationConfig>();
	SlateApplication.SetNavigationConfig(CustomNavConfig.ToSharedRef());

	RebuildNavigationRules();

	OptionalNavKeyActions = DevelopSetting->GetNavigationOptionalActionKeys();


	//Input Processor 초기화
	FSlateApplication& SlateApp = FSlateApplication::Get();

	CheckInputDeviceProcessor = MakeShared<UCheckInputDeviceProcessor>();
	SlateApp.RegisterInputPreProcessor(CheckInputDeviceProcessor);

	ChangeMappingInputProcessor = MakeShared<UChangeMappingInputProcessor>();
	ChangeMappingInputProcessor->RegistryTryCustomMappingEvent(UChangeMappingInputProcessor::FOnTryCustomMapping().
																CreateUObject(this, &UCustomInputSettingSubsystem::TryCustomMapping));

	ResetProcessor = MakeShared<UWaitInputForResetMappingInputProcessor>();
	ResetProcessor->RegistryResetMappingEvent(UWaitInputForResetMappingInputProcessor::FOnResetMappingDelegate().
												CreateUObject(this, &UCustomInputSettingSubsystem::ResetCustomMapping));
}

void UCustomInputSettingSubsystem::Deinitialize()
{
	SubsystemInstance = nullptr;

}


TMap<FName, FKey> UCustomInputSettingSubsystem::GetCustomMappingKeys() const
{
	return CustomMappings;
}

bool UCustomInputSettingSubsystem::TryCustomMapping(const FName& Code, const FKey& NewKey, bool bIsGamepad, FText& ErrorMsg)
{
	if (!bIsGamepad && NewKey.IsGamepadKey())
	{
		ErrorMsg = LOCTEXT("Invalidate Keyboard Key Error", "키보드/마우스 키를 입력해 주십시오");
		return false;
	}
	if (bIsGamepad && !NewKey.IsGamepadKey())
	{
		ErrorMsg = LOCTEXT("Invalidate Gamepad Key Error", "게임패드 키를 입력해 주십시오");
		return false;
	}

	if (IgnoreMappingKeys.Contains(NewKey))
	{
		ErrorMsg = LOCTEXT("Try Mapping Ignore Key Error", "옳지 못한 키를 입력하셨습니다");
		return false;
	}

	CustomMappings.FindOrAdd(Code) = NewKey;
	SaveConfig();

	OnChangedMappingEvent(Code, NewKey);

	return true;
}

void UCustomInputSettingSubsystem::RegisterAffectedController(APlayerController* NewController)
{
	TWeakObjectPtr<APlayerController> WeakTarget(NewController);
	if(!WeakTarget.IsValid()) return;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(WeakTarget->GetLocalPlayer()))
	{
		Controller = WeakTarget;

		TMap<FName, FKey> CustomMappingKeys = GetCustomMappingKeys();

		for (const auto& CustomMap : CustomMappingKeys)
		{
			Subsystem->AddPlayerMappedKey(CustomMap.Key, CustomMap.Value);
		}
	}
}



void UCustomInputSettingSubsystem::OnChangedMappingEvent(const FName& Code, const FKey& NewKey)
{
	FSlateApplication& SlateApp = FSlateApplication::Get();

	SlateApp.UnregisterInputPreProcessor(ChangeMappingInputProcessor);


	if (Controller.IsValid())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer()))
		{
			Subsystem->RemovePlayerMappedKey(Code);
			Subsystem->AddPlayerMappedKey(Code, NewKey);
		}
	}

	if(OnChangedMappingDel.IsBound()) OnChangedMappingDel.Broadcast(Code, NewKey);
}

FDelegateHandle UCustomInputSettingSubsystem::AddOnChangedMappingEvent(const TDelegate<void(const FName&, const FKey&)> InDel)
{
	return OnChangedMappingDel.Add(InDel);
}

void UCustomInputSettingSubsystem::RemoveOnChangedMappingEvent(FDelegateHandle& Handle)
{
	if(!Handle.IsValid()) return;

	OnChangedMappingDel.Remove(Handle);
	Handle.Reset();
}

void UCustomInputSettingSubsystem::ResetCustomMapping(UPlayerMappableInputConfig* InInputConfig)
{
	TArray<FName> ResetCodes;

	TArray<FEnhancedActionKeyMapping> MappableKeys = InInputConfig->GetPlayerMappableKeys();
	for (auto& MappableKey : MappableKeys)
	{
		ResetCodes.Add(MappableKey.PlayerMappableOptions.Name);
		CustomMappings.Remove(MappableKey.PlayerMappableOptions.Name);
	}

	//리셋 후 컨트롤러에 영향
	if (Controller.IsValid())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer()))
		{
			for (const FName& ResetCode : ResetCodes)
			{
				Subsystem->RemovePlayerMappedKey(ResetCode);
			}
		}
	}
	
	if (OnResetDelegate.IsBound())
	{
		OnResetDelegate.Broadcast(InInputConfig);
	}

	SaveConfig();
}

FDelegateHandle UCustomInputSettingSubsystem::AddResetMappingEvent(TDelegate<void(UPlayerMappableInputConfig*)> InDel)
{
	return OnResetDelegate.Add(InDel);;
}

void UCustomInputSettingSubsystem::RemoveResetMappingEvent(FDelegateHandle& InHandle)
{
	if(!InHandle.IsValid()) return;

	OnResetDelegate.Remove(InHandle);
	InHandle.Reset();
}

void UCustomInputSettingSubsystem::RequestResetMapping(UPlayerMappableInputConfig* InInputConfig)
{
	if (InInputConfig == nullptr)
	{
		LOG_ERROR(TEXT("Request Reset nullptr Input Config"));
		return;
	}

	ResetProcessor->WaitForResetMapping(InInputConfig);
}


UTexture2D* UCustomInputSettingSubsystem::GetKeyIcon(const FKey& TargetKey) const
{
	const TObjectPtr<UTexture2D>* Temp = KeyIcons.Find(TargetKey.GetFName());
	return Temp == nullptr ? nullptr : *Temp;
}


void UCustomInputSettingSubsystem::RequestMappingKey(const FName& Code, bool bGamepad, const FText& DisplayName)
{
	if (!ensureMsgf(Code != NAME_None, TEXT("Can't Request Custom Mapping Code : NAME_None"))) { return; }

	ChangeMappingInputProcessor->WaitReceiveInput(Code, bGamepad, DisplayName);
}

void UCustomInputSettingSubsystem::RebuildNavigationRules()
{
	if (!CustomNavConfig.IsValid()) return;

	CustomNavConfig->KeyEventRules.Empty();

	const UCustomInputDeveloperSettings* DevelopSetting = GetDefault<UCustomInputDeveloperSettings>();

	const TMap<EUINavigation, FKeySettingContainer>& NavKeys = DevelopSetting->GetCustomNavigationKeys();
	for (const auto& NavKey : NavKeys)
	{
		if(NavKey.Value.Keyboard.IsValid() && !NavKey.Value.Keyboard.IsGamepadKey()) { CustomNavConfig->KeyEventRules.Add(NavKey.Value.Keyboard, NavKey.Key); }
		else { LOG_ERROR(TEXT("Custom NavKey Invalidate.")); }
		
		if (NavKey.Value.Gamepad.IsValid() && NavKey.Value.Gamepad.IsGamepadKey()) { CustomNavConfig->KeyEventRules.Add(NavKey.Value.Gamepad, NavKey.Key); }
		else { LOG_ERROR(TEXT("Custom NavKey Invalidate.")); }
	}


	const TMap<EUINavigationAction, FKeySettingContainer>& NavActionKeys = DevelopSetting->GetCustomNavigationActionKeys();
	for (const auto& NavActionKey : NavActionKeys)
	{
		if(NavActionKey.Value.Keyboard.IsValid() && !NavActionKey.Value.Keyboard.IsGamepadKey()) { CustomNavConfig->KeyActionEventRules.Add(NavActionKey.Value.Keyboard, NavActionKey.Key); }
		else { LOG_ERROR(TEXT("Custom NavKey Invalidate.")); }

		if(NavActionKey.Value.Gamepad.IsValid() && NavActionKey.Value.Gamepad.IsGamepadKey()) { CustomNavConfig->KeyActionEventRules.Add(NavActionKey.Value.Gamepad, NavActionKey.Key); }
		else { LOG_ERROR(TEXT("Custom NavKey Invalidate.")); }
	}
}

void UCustomInputSettingSubsystem::GetNavigationDirectionKey(EUINavigation InDirection, FKey& OutKeyboard, FKey& OutGamepad) const
{
	if(InDirection == EUINavigation::Invalid || InDirection == EUINavigation::Num || !CustomNavConfig.IsValid()) return;

	const TMap<FKey, EUINavigation> NavDirectionMap = CustomNavConfig->KeyEventRules;

	bool bHasKeyboard = false;
	bool bHasGamepad = false;

	for (const auto& NavDir : NavDirectionMap)
	{
		if (NavDir.Value == InDirection)
		{
			if (NavDir.Key.IsGamepadKey())
			{
				OutGamepad = NavDir.Key;
				bHasGamepad = true;
			}
			else
			{
				OutKeyboard = NavDir.Key;
				bHasKeyboard = true;
			}

			if(bHasGamepad && bHasKeyboard) break;
		}
	}
}

void UCustomInputSettingSubsystem::GetNavigationActionKey(EUINavigationAction InAction, FKey& OutKeyboard, FKey& OutGamepad) const
{
	if (InAction == EUINavigationAction::Invalid || InAction == EUINavigationAction::Num || !CustomNavConfig.IsValid()) return;

	const TMap<FKey, EUINavigationAction> NavDirectionMap = CustomNavConfig->KeyActionEventRules;

	bool bHasKeyboard = false;
	bool bHasGamepad = false;

	for (const auto& NavDir : NavDirectionMap)
	{
		if (NavDir.Value == InAction)
		{
			if (NavDir.Key.IsGamepadKey())
			{
				OutGamepad = NavDir.Key;
				bHasGamepad = true;
			}
			else
			{
				OutKeyboard = NavDir.Key;
				bHasKeyboard = true;
			}

			if (bHasGamepad && bHasKeyboard) break;
		}
	}
}



void UCustomInputSettingSubsystem::GetNavigationOptionalActionKey(const FName& InActionName, FKey& OutKeyboard, FKey& OutGamepad) const
{
	if(InActionName.IsNone()) return;


	if (const FKeySettingContainer* OptionalActions = OptionalNavKeyActions.Find(InActionName))
	{
		OutKeyboard = OptionalActions->Keyboard;
		OutGamepad = OptionalActions->Gamepad;
	}
}



EInputDeviceType UCustomInputSettingSubsystem::GetCurrentInputDevice() const
{
	return CheckInputDeviceProcessor->GetCurrentInputDevice();
}

FDelegateHandle UCustomInputSettingSubsystem::AddInputDeviceChangeEvent(const TDelegate<void(EInputDeviceType)>& InDelegate)
{
	return CheckInputDeviceProcessor->AddInputDeviceChangeEvent(InDelegate);
}

void UCustomInputSettingSubsystem::RemoveInputDeviceChangeEvent(FDelegateHandle& InHandle)
{
	if(!InHandle.IsValid()) return;

	CheckInputDeviceProcessor->RemoveInputDeviceChangeEvent(InHandle);
	InHandle.Reset();
}



#undef LOCTEXT_NAMESPACE

