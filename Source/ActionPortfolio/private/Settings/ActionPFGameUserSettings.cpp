// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/ActionPFGameUserSettings.h"
#include "ActionPortfolio.h"
#include "Engine/RendererSettings.h"

#define LOCTEXT_NAMESPACE "ActionPFGameUserSetting"

UActionPFGameUserSettings::UActionPFGameUserSettings()
{
	BasicLevelOptions = {	LOCTEXT("Level_Low", "저"),
							LOCTEXT("Level_Middle", "중"),
							LOCTEXT("Level_High", "고"),
							LOCTEXT("Level_Epic", "에픽") };

	ScreenResolutionOptions = {	FIntPoint(1280, 720),
								FIntPoint(1600, 900),
								FIntPoint(1920, 1080),
								FIntPoint(2560, 1440)};

	WindowModeNames = { LOCTEXT("FullScreen", "전체화면"),
						LOCTEXT("WindowedFullscreen", "전체창모드"),
						LOCTEXT("Window", "창모드") };

	FrameRateLimitOptions = { 30, 60, 100, 120, 144, 0};

	ViewDistanceLevelOptions = {	LOCTEXT("ViewDistanceLevel_Low", "근거리"),
									LOCTEXT("ViewDistanceLevel_Middle", "중거리"),
									LOCTEXT("ViewDistanceLevel_High", "원거리"),
									LOCTEXT("ViewDistanceLevel_Epic", "전체")};

	AAMethodOptions = {	LOCTEXT("None", "None"),
						LOCTEXT("FXAA", "FXAA"),
						LOCTEXT("TAA", "TAA"),
						LOCTEXT("2xMSAA", "2xMSAA"),
						LOCTEXT("4xMSAA", "4xMSAA"),
						LOCTEXT("8xMSAA", "8xMSAA"),
						LOCTEXT("TSR", "TSR")};
}

UActionPFGameUserSettings* UActionPFGameUserSettings::Get()
{
	return Cast<UActionPFGameUserSettings>(GEngine->GetGameUserSettings());
}

void UActionPFGameUserSettings::ApplyGraphicSettings()
{
	ApplySettings(true);

	URendererSettings* RendererSettings = GetMutableDefault<URendererSettings>();

	int AAType = RendererSettings->DefaultFeatureAntiAliasing;
	int MSAASampleCount = RendererSettings->MSAASampleCount;

	GetWorld()->GetFirstPlayerController()->ConsoleCommand(FString::Printf(TEXT("r.AntiAliasingMethod %d"), AAType));
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(FString::Printf(TEXT("r.MSAACount %d"), MSAASampleCount));

	RendererSettings->SaveConfig();
}

void UActionPFGameUserSettings::SetScreenResolutionByIndex(int Idx, bool bApply)
{
#if WITH_EDITOR
	if (!ensureMsgf(ScreenResolutionOptions.IsValidIndex(Idx), TEXT("Invalidate Screen Resolution Opion Idx : %d"), Idx)) { return; }
#endif

	SetScreenResolution(ScreenResolutionOptions[Idx]);

	if(bApply) ApplySettings(true);
}

void UActionPFGameUserSettings::SetWindowMode(int Idx, bool bApply)
{
	SetFullscreenMode(EWindowMode::ConvertIntToWindowMode(Idx));

	if(bApply) ApplySettings(true);
}

int UActionPFGameUserSettings::GetCurrentWindowMode() const
{
	EWindowMode::Type CurrentMode = GetFullscreenMode();

	switch (CurrentMode) {
	case EWindowMode::Fullscreen:
		return 0;
		break;
	
	case EWindowMode::WindowedFullscreen:
		return 1;
		break;
	case EWindowMode::Windowed:
	default:
		return 2;
		break;
	}
}

void UActionPFGameUserSettings::SetFrameRateLimitByIdx(int Idx, bool bApply)
{
	SetFrameRateLimit(FrameRateLimitOptions[Idx]);

	if(bApply) ApplySettings(true);
}

void UActionPFGameUserSettings::SetAntiAliasingMethod(int32 NewType, bool bApply)
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

	RendererSettings->SaveConfig();
}

int UActionPFGameUserSettings::GetAntiAliasingMethod() const
{
	URendererSettings* RendererSettings = GetMutableDefault<URendererSettings>();

	int Result = 0;

	switch (RendererSettings->DefaultFeatureAntiAliasing)
	{
		case EAntiAliasingMethod::AAM_None:
		default:
			Result = 0;
		break;

		case EAntiAliasingMethod::AAM_FXAA:
			Result = 1;
		break;

		case EAntiAliasingMethod::AAM_TemporalAA:
			Result = 2;
		break;

		case EAntiAliasingMethod::AAM_MSAA:
			if(RendererSettings->MSAASampleCount == ECompositingSampleCount::Two)
			{ Result = 3; }
			else if(RendererSettings->MSAASampleCount == ECompositingSampleCount::Four)
			{ Result = 4; }
			else if(RendererSettings->MSAASampleCount == ECompositingSampleCount::Eight)
			{ Result = 5; }
		break;

		case EAntiAliasingMethod::AAM_TSR:
			Result = 6;
		break;
	}

	return Result;
}


#undef LOCTEXT_NAMESPACE