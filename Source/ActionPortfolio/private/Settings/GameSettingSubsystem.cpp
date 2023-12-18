// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/GameSettingSubsystem.h"
#include "ActionPortfolio.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/RendererSettings.h"
#include "DialogueManager.h"
#include "DialogueBFL.h"
#include "Kismet/KismetInternationalizationLibrary.h"

#define LOCTEXT_NAMESPACE "ActionPFSettings"

UGameSettingSubsystem* UGameSettingSubsystem::SettingInstance = nullptr;

TArray<FText> UGameSettingSubsystem::BasicLevelOptions({LOCTEXT("레벨_저", "저"), 
														LOCTEXT("레벨_중", "중"), 
														LOCTEXT("레벨_고", "고"),
														LOCTEXT("레벨_에픽", "에픽") });

UGameSettingSubsystem::UGameSettingSubsystem()
{
	WindowedMode = LOCTEXT("WindowedMode", "창화면");
	WindowedFullScreenMode = LOCTEXT("WindowedFullScreenMode", "전체 창화면");
	FullScreenMode = LOCTEXT("FullScreenMode", "전체화면");
}

void UGameSettingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	URendererSettings* RendererSettings = GetMutableDefault<URendererSettings>();
	SettingInstance = this;

	ScreenResolutionOptions.Add(FIntPoint(1280, 720));
	ScreenResolutionOptions.Add(FIntPoint(1600, 900));
	ScreenResolutionOptions.Add(FIntPoint(1920, 1080));
	ScreenResolutionOptions.Add(FIntPoint(2560, 1440));

	FrameRateLimitOptions.Add(30);
	FrameRateLimitOptions.Add(60);
	FrameRateLimitOptions.Add(120);
	FrameRateLimitOptions.Add(144);
	FrameRateLimitOptions.Add(0);

	Current_ScreenResolution_IDX = FMath::Clamp(Current_ScreenResolution_IDX, 0, ScreenResolutionOptions.Num() - 1);
	UserSettings->SetScreenResolution(ScreenResolutionOptions[Current_ScreenResolution_IDX]);

	Current_FrameRateLimit_IDX = FMath::Clamp(Current_FrameRateLimit_IDX, 0, FrameRateLimitOptions.Num() - 1);
	UserSettings->SetFrameRateLimit(FrameRateLimitOptions[Current_FrameRateLimit_IDX]);

	EAntiAliasingMethod CurrentAAM = RendererSettings->DefaultFeatureAntiAliasing;
	ECompositingSampleCount::Type CurrentMSAASamepleCount = RendererSettings->MSAASampleCount;

	switch (CurrentAAM)
	{
	case AAM_None:
		CurrentAntiAliasingMethodIDX = 0;
		break;
	case AAM_FXAA:
		CurrentAntiAliasingMethodIDX = 1;
		break;
	case AAM_TemporalAA:
		CurrentAntiAliasingMethodIDX = 2;
		break;
	case AAM_MSAA:
		switch (CurrentMSAASamepleCount)
		{
		case ECompositingSampleCount::Two:
			CurrentAntiAliasingMethodIDX = 3;
			break;
		case ECompositingSampleCount::Four:
			CurrentAntiAliasingMethodIDX = 4;
			break;
		case ECompositingSampleCount::Eight:
			CurrentAntiAliasingMethodIDX = 5;
			break;
		}
		break;
	case AAM_TSR:
		CurrentAntiAliasingMethodIDX = 6;
		break;
	}

	UserSettings->ApplySettings(false);
	SaveConfig();
}

const TArray<FString> UGameSettingSubsystem::GetDialogueAnimSpeedOptions() const
{
	TArray<FText> SpeedTexts({
						LOCTEXT("느림", "느림"),
						LOCTEXT("중간", "중간"),
						LOCTEXT("빠름", "빠름")
	});

	TArray<FString> ReturnValue;

	for (auto SpeedText : SpeedTexts) {
		ReturnValue.Add(SpeedText.ToString());
	}

	return ReturnValue;
}

void UGameSettingSubsystem::SetCurrentLanguage(ELanguage NewLanguage)
{
	if (CurrentLanguage == NewLanguage) return;
	CurrentLanguage = NewLanguage;
	UDialogueManager* DialogueManager = GetGameInstance()->GetSubsystem<UDialogueManager>();

	switch (CurrentLanguage)
	{
	case ELanguage::Korean:
		DialogueManager->SetCurrentLanguage(EDialogueLanguage::Korean);
		UKismetInternationalizationLibrary::SetCurrentCulture("ko-KR");
		break;
	case ELanguage::English:
		DialogueManager->SetCurrentLanguage(EDialogueLanguage::English);
		UKismetInternationalizationLibrary::SetCurrentCulture("en");
		break;
	default:
		break;
	}

	SaveConfig();
}

const TArray<FString> UGameSettingSubsystem::GetLanguageOptions() const
{
	TArray<FString> FinalOptions;
	FinalOptions.Add(TEXT("한국어"));
	FinalOptions.Add("English");

	return FinalOptions;
}

void UGameSettingSubsystem::ApplyGraphicSettings()
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	UserSettings->ApplySettings(true);

	URendererSettings* RendererSettings = GetMutableDefault<URendererSettings>();

	int AAType = RendererSettings->DefaultFeatureAntiAliasing;
	int MSAASampleCount = RendererSettings->MSAASampleCount;

	GetWorld()->GetFirstPlayerController()->ConsoleCommand(FString::Printf(TEXT("r.AntiAliasingMethod %d"), AAType));
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(FString::Printf(TEXT("r.MSAACount %d"), MSAASampleCount));
	
	float Brightness = RendererSettings->DefaultFeatureAutoExposureBias;
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(FString::Printf(TEXT("r.DefaultFeature.AutoExposure.Bias %.2f"), Brightness));
}

void UGameSettingSubsystem::SetScreenResolution(int Idx, bool bApply)
{
	if (ScreenResolutionOptions.IsEmpty()) {
		PFLOG(Warning, TEXT("Screen Resolution Options is Empty."));
		return;
	}

	Current_ScreenResolution_IDX = Idx;

	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	SaveConfig();
	UserSettings->SetScreenResolution(ScreenResolutionOptions[Current_ScreenResolution_IDX]);

	if(bApply) UserSettings->ApplySettings(true);
}

void UGameSettingSubsystem::SetWindowMode(int Idx, bool bApply)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	switch (Idx) {
		case 0:
			UserSettings->SetFullscreenMode(EWindowMode::Windowed);
			break;
		case 1:
			UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
			break;
		case 2:
			UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
			break;
	}

	if (bApply) UserSettings->ApplySettings(true);
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

void UGameSettingSubsystem::SetFrameRateLimit(int Idx, bool bApply)
{
#if WITH_EDITOR
	if (FrameRateLimitOptions.IsEmpty()) {
		PFLOG(Warning, TEXT("Frame Rate Limit Options is Empty."));
		return;
	}
#endif

	Current_FrameRateLimit_IDX = FMath::Clamp(Idx, 0, FrameRateLimitOptions.Num() - 1);

	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	UserSettings->SetFrameRateLimit(FrameRateLimitOptions[Current_FrameRateLimit_IDX]);
	if (bApply) UserSettings->ApplySettings(false);
	SaveConfig();
}

float UGameSettingSubsystem::GetCurrentFrameRateLimit() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return UserSettings->GetFrameRateLimit();
}

void UGameSettingSubsystem::SetVSyncEnabled(bool NewState, bool bApply)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	UserSettings->SetVSyncEnabled(NewState);
	if (bApply) UserSettings->ApplySettings(false);
}

bool UGameSettingSubsystem::IsVSyncEnabled() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return UserSettings->IsVSyncEnabled();
}

void UGameSettingSubsystem::SetViewDistanceLevel(int32 NewLevel, bool bApply)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	UserSettings->SetViewDistanceQuality(NewLevel);
	if (bApply) UserSettings->ApplySettings(false);
}

int32 UGameSettingSubsystem::GetViewDistanceLevel() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return UserSettings->GetViewDistanceQuality();
}

const TArray<FString> UGameSettingSubsystem::GetViewDistanceLevelOptions() const
{
	static TArray<FText> ViewDistanceLevelOptions({	LOCTEXT("가시거리_근거리", "근거리"), 
													LOCTEXT("가시거리_중거리", "중거리"), 
													LOCTEXT("가시거리_원거리", "원거리"), 
													LOCTEXT("가시거리_전체", "전체")});

	TArray<FString> FinalOptions;
	for (auto Option : ViewDistanceLevelOptions) {
		FinalOptions.Add(Option.ToString());
	}

	return FinalOptions;
}

void UGameSettingSubsystem::SetShadowQuality(int32 NewLevel, bool bApply)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	UserSettings->SetShadowQuality(NewLevel);
	if (bApply) UserSettings->ApplySettings(false);
}

int32 UGameSettingSubsystem::GetShadowQuality() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return UserSettings->GetShadowQuality();
}

const TArray<FString> UGameSettingSubsystem::GetShadowQualityOptions() const
{
	TArray<FString> FinalOptions;
	for (auto QualityOption : BasicLevelOptions) {
		FinalOptions.Add(QualityOption.ToString());
	}

	return FinalOptions;
}

void UGameSettingSubsystem::SetTextureQuality(int32 NewLevel, bool bApply)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	UserSettings->SetTextureQuality(NewLevel);
	if (bApply) UserSettings->ApplySettings(false);
}

int32 UGameSettingSubsystem::GetTextureQuality() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return UserSettings->GetTextureQuality();
}

const TArray<FString> UGameSettingSubsystem::GetTextureQualityOptions() const
{
	TArray<FString> FinalOptions;
	for (auto QualityOption : BasicLevelOptions) {
		FinalOptions.Add(QualityOption.ToString());
	}

	return FinalOptions;
}

void UGameSettingSubsystem::SetAntiAliasingQuality(int32 NewLevel, bool bApply)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	UserSettings->SetAntiAliasingQuality(NewLevel);
	if (bApply) UserSettings->ApplySettings(false);
}

int32 UGameSettingSubsystem::GetAntiAliasingQuality() const
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return UserSettings->GetAntiAliasingQuality();
}

const TArray<FString> UGameSettingSubsystem::GetAntiAliasingQualityOptions() const
{
	TArray<FString> FinalOptions;
	for (auto QualityOption : BasicLevelOptions) {
		FinalOptions.Add(QualityOption.ToString());
	}

	return FinalOptions;
}

void UGameSettingSubsystem::SetAntiAliasingMethod(int32 NewType, bool bApply)
{
	URendererSettings* RendererSettings = GetMutableDefault<URendererSettings>();

	// 0 = None, 1 = FXAA, 2 = TAA, 3 = 2xMSAA, 4 = 4xMSAA, 5 = 8xMSAA, 6 = TSR
	int32 ClampedType = FMath::Clamp(NewType, 0, 6);

	switch (ClampedType) {
		case 0:
			RendererSettings->DefaultFeatureAntiAliasing = EAntiAliasingMethod::AAM_None;
			break;
		case 1:
			RendererSettings->DefaultFeatureAntiAliasing = EAntiAliasingMethod::AAM_FXAA;
			break;
		case 2:
			RendererSettings->DefaultFeatureAntiAliasing = EAntiAliasingMethod::AAM_TemporalAA;
			break;
		case 3:
			RendererSettings->DefaultFeatureAntiAliasing = EAntiAliasingMethod::AAM_MSAA;
			RendererSettings->MSAASampleCount = ECompositingSampleCount::Two;
			break;
		case 4:
			RendererSettings->DefaultFeatureAntiAliasing = EAntiAliasingMethod::AAM_MSAA;
			RendererSettings->MSAASampleCount = ECompositingSampleCount::Four;
			break;
		case 5:
			RendererSettings->DefaultFeatureAntiAliasing = EAntiAliasingMethod::AAM_MSAA;
			RendererSettings->MSAASampleCount = ECompositingSampleCount::Eight;
			break;
		case 6:
			RendererSettings->DefaultFeatureAntiAliasing = EAntiAliasingMethod::AAM_TSR;
			break;
	}

	int T = RendererSettings->DefaultFeatureAntiAliasing;
	int MSAASC = RendererSettings->MSAASampleCount;

	if (bApply) {
		GetWorld()->GetFirstPlayerController()->ConsoleCommand(FString::Printf(TEXT("r.AntiAliasingMethod %d"), T));
		GetWorld()->GetFirstPlayerController()->ConsoleCommand(FString::Printf(TEXT("r.MSAACount %d"), MSAASC));
	}

	CurrentAntiAliasingMethodIDX = ClampedType;
	RendererSettings->SaveConfig();
}

const TArray<FString>& UGameSettingSubsystem::GetAntiAliasingMethodOptions() const
{
	static TArray<FString> AAMOptions({"None","FXAA", "TAA", "2xMSAA", "4xMSAA", "8xMSAA", "TSR"});

	return AAMOptions;
}


void UGameSettingSubsystem::SetBrightness(float NewValue, bool bApply)
{
	URendererSettings* RendererSettings = GetMutableDefault<URendererSettings>();

	float ClampedValue = FMath::Clamp<float>(NewValue, 0, 100);
	float FinalValue = 0.04 * ClampedValue;

	RendererSettings->DefaultFeatureAutoExposureBias = FinalValue;

	if (bApply) {
		GetWorld()->GetFirstPlayerController()->ConsoleCommand(FString::Printf(TEXT("r.DefaultFeature.AutoExposure.Bias %.2f"), FinalValue));
	}
	RendererSettings->SaveConfig();
	
}

float UGameSettingSubsystem::GetBrightness() const
{
	URendererSettings* RendererSettings = GetMutableDefault<URendererSettings>();
	return 25 * RendererSettings->DefaultFeatureAutoExposureBias;
}

#undef LOCTEXT_NAMESPACE