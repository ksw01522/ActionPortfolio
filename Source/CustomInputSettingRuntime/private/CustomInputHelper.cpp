// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomInputHelper.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "CustomInputSettingSubsystem.h"

void UCustomInputHelper::RegistryCustomMappingUser(APlayerController* Target)
{
	UCustomInputSettingSubsystem* SettingInst = UCustomInputSettingSubsystem::GetInstance();

#if WITH_EDITOR
	if(ensureMsgf(SettingInst != nullptr, TEXT("Can't find Custom Input Setting Instance."))) return;
#endif

	SettingInst->RegisterAffectedController(Target);
}


void UCustomInputHelper::AddInputMapping(APlayerController* PlayerController, UInputMappingContext* Mapping, int Priority, const FModifyContextOptions& Options)
{
	if (ensureMsgf(PlayerController == nullptr, TEXT("Try Add Input Mapping nullptr Controller"))) return;
	if (ensureMsgf(Mapping == nullptr, TEXT("Try Add Input Mapping nullptr Mapping In Controller : %s"), *PlayerController->GetName())) return;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(Mapping, Priority, Options);
	}
}

void UCustomInputHelper::RemoveInputMapping(APlayerController* PlayerController, UInputMappingContext* Mapping, const FModifyContextOptions& Options)
{
	if (ensureMsgf(PlayerController == nullptr, TEXT("Try Remove Input Mapping nullptr Controller"))) return;
	if (ensureMsgf(Mapping == nullptr, TEXT("Try Remove Input Mapping nullptr Mapping In Controller : %s"), *PlayerController->GetName())) return;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(Mapping, Options);
	}
}

void UCustomInputHelper::AddPlayerMappableConfig(APlayerController* PlayerController, const UPlayerMappableInputConfig* Config, const FModifyContextOptions& Options)
{
	if (ensureMsgf(PlayerController == nullptr, TEXT("Try Remove Input Mapping nullptr Controller"))) return;
	if (ensureMsgf(Config == nullptr, TEXT("Try Remove Input Mapping config nullptr Mapping In Controller : %s"), *PlayerController->GetName())) return;
	

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddPlayerMappableConfig(Config, Options);
	}
}

void UCustomInputHelper::RemovePlayerMappableConfig(APlayerController* PlayerController, const UPlayerMappableInputConfig* Config, const FModifyContextOptions& Options)
{
	if (ensureMsgf(PlayerController == nullptr, TEXT("Try Remove Input Mapping nullptr Controller"))) return;
	if (ensureMsgf(Config == nullptr, TEXT("Try Remove Input Mapping config nullptr Mapping In Controller : %s"), *PlayerController->GetName())) return;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->RemovePlayerMappableConfig(Config, Options);
	}
}
