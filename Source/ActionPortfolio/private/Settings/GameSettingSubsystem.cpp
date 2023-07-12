// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/GameSettingSubsystem.h"
#include "ActionPortfolio.h"
#include "GameFramework/GameUserSettings.h"

#define LOCTEXT_NAMESPACE "ActionPFGameSettings"

UGameSettingSubsystem::UGameSettingSubsystem()
{
	WindowedMode = LOCTEXT("WindowedMode", "창화면");
	WindowedFullScreenMode = LOCTEXT("WindowedFullScreenMode", "전체 창화면");
	FullScreenMode = LOCTEXT("FullScreenMode", "전체화면");
}

void UGameSettingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	ScreenResolutions.Add(FIntPoint(1280, 720));
	ScreenResolutions.Add(FIntPoint(1600, 900));
	ScreenResolutions.Add(FIntPoint(1920, 1080));
	ScreenResolutions.Add(FIntPoint(2560, 1440));

	FrameRateLimitOptions.Add(30);
	FrameRateLimitOptions.Add(60);
	FrameRateLimitOptions.Add(120);
	FrameRateLimitOptions.Add(144);
	FrameRateLimitOptions.Add(0);
}

void UGameSettingSubsystem::ChangeScreenResolution(int Idx)
{
	if (Idx < 0 || ScreenResolutions.Num() <= Idx) {
		PFLOG(Error, TEXT("Can't Find ScreenResolution."));
		return;
	}

	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	UserSettings->SetScreenResolution(ScreenResolutions[Idx]);
	UserSettings->ApplySettings(true);
}

const FIntPoint UGameSettingSubsystem::GetCurrentScreenResolution() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return UserSettings->GetScreenResolution();
}

void UGameSettingSubsystem::SetWindowMode(FString NewMode)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	if (NewMode == WindowedMode.ToString()) {
		UserSettings->SetFullscreenMode(EWindowMode::Windowed);
	}
	else if (NewMode == WindowedFullScreenMode.ToString()) {
		UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	}
	else if (NewMode == FullScreenMode.ToString()) {
		UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
	}
	else
	{
		PFLOG(Error, TEXT("Can't find Selected WindowMode."));
		return;
	}


	UserSettings->ApplySettings(true);
}

const TArray<FString> UGameSettingSubsystem::GetWindowModeOptions() const
{
	TArray<FString> WindowModes;

	WindowModes.Add(WindowedMode.ToString());
	WindowModes.Add(WindowedFullScreenMode.ToString());
	WindowModes.Add(FullScreenMode.ToString());

	return WindowModes;
}

FString UGameSettingSubsystem::GetCurrentWindowMode() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	EWindowMode::Type CurrentMode = UserSettings->GetFullscreenMode();

	switch (CurrentMode) {
		case EWindowMode::Windowed:
			return WindowedMode.ToString();
		break;
		case EWindowMode::WindowedFullscreen:
			return WindowedFullScreenMode.ToString();
		break;
		case EWindowMode::Fullscreen:
			return FullScreenMode.ToString();
		break;
	}

	return "Not Valid Mode";
}

void UGameSettingSubsystem::ChangeFrameRateLimit(int Idx)
{
	if (!FrameRateLimitOptions.IsValidIndex(Idx)) {
		PFLOG(Error, TEXT("Can't find Selected Frame Limit Option."));
		return;
	}

	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	UserSettings->SetFrameRateLimit(FrameRateLimitOptions[Idx]);
	UserSettings->ApplySettings(false);
}

float UGameSettingSubsystem::GetCurrentFrameRateLimit() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return UserSettings->GetFrameRateLimit();
}

void UGameSettingSubsystem::SetVSyncEnabled(bool NewState)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	UserSettings->SetVSyncEnabled(NewState);
	UserSettings->ApplySettings(false);
}

bool UGameSettingSubsystem::IsVSyncEnabled() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return UserSettings->IsVSyncEnabled();
}

void UGameSettingSubsystem::SetViewDistanceLevel(int32 NewLevel)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	UserSettings->SetViewDistanceQuality(NewLevel);
	UserSettings->ApplySettings(false);
}

int32 UGameSettingSubsystem::GetViewDistanceLevel() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return UserSettings->GetViewDistanceQuality();
}

void UGameSettingSubsystem::SetShadowQuality(int32 NewLevel)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	UserSettings->SetShadowQuality(NewLevel);
	UserSettings->ApplySettings(false);
}

int32 UGameSettingSubsystem::GetShadowQuality() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return UserSettings->GetShadowQuality();
}

void UGameSettingSubsystem::SetTextureQuality(int32 NewLevel)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	UserSettings->SetTextureQuality(NewLevel);
	UserSettings->ApplySettings(false);
}

int32 UGameSettingSubsystem::GetTextureQuality() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return UserSettings->GetTextureQuality();
}

void UGameSettingSubsystem::SetAntiAliasingQuality(int32 NewLevel)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	UserSettings->SetAntiAliasingQuality(NewLevel);
	UserSettings->ApplySettings(false);
}

int32 UGameSettingSubsystem::GetAntiAliasingQuality() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return UserSettings->GetAntiAliasingQuality();
}

void UGameSettingSubsystem::ForStudy()
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	
}

#undef LOCTEXT_NAMESPACE