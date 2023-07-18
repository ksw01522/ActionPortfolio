// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/Widget_Settings.h"
#include "ActionPortfolio.h"
#include "ActionPortfolioInstance.h"
#include "Settings/GameSettingSubsystem.h"
#include "Components/ComboBoxString.h"
#include "Math/IntPoint.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Settings/CustomStructForSetting.h"
#include "Components/Button.h"

void UWidget_Settings::NativeConstruct()
{
	Super::NativeConstruct();

	GameSettingSubsystem = UGameInstance::GetSubsystem<UGameSettingSubsystem>(GetWorld()->GetGameInstance());

	ExitButton->OnClicked.AddDynamic(this, &UWidget_Settings::HiddenSettingsWidget);

	UpdateBasicOptions();
	UpdateGraphicOptions();
}

void UWidget_Settings::VisibleSettingsWidget()
{
	UpdateBasicOptions();
	UpdateGraphicOptions();

	SetVisibility(ESlateVisibility::Visible);
}

void UWidget_Settings::HiddenSettingsWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
}


void UWidget_Settings::UpdateBasicOptions()
{
	//언어
	LanguageComboBox->OnSelectionChanged.Clear();
	LanguageComboBox->ClearOptions();

	const TArray<FString> LanguageOptions = GameSettingSubsystem->GetLanguageOptions();

	for (auto LanguageOption : LanguageOptions) {
		LanguageComboBox->AddOption(LanguageOption);
	}

	LanguageComboBox->SetSelectedIndex((int32)GameSettingSubsystem->GetCurrentLanguage());
	LanguageComboBox->OnSelectionChanged.AddDynamic(this, &UWidget_Settings::OnChangedLanguageOption);

	//대화창 속도
	DialogueSpeedComboBox->OnSelectionChanged.Clear();
	DialogueSpeedComboBox->ClearOptions();

	const TArray<FString> SpeedOptions = GameSettingSubsystem->GetDialogueAnimSpeedOptions();

	for (auto SpeedOption : SpeedOptions) {
		DialogueSpeedComboBox->AddOption(SpeedOption);
	}

	DialogueSpeedComboBox->SetSelectedIndex((int32)GameSettingSubsystem->GetCurrentDialogueAnimSpeed());
	DialogueSpeedComboBox->OnSelectionChanged.AddDynamic(this, &UWidget_Settings::OnChangedDialogueSpeedOption);
}

void UWidget_Settings::OnChangedLanguageOption(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	GameSettingSubsystem->SetCurrentLanguage((ELanguage)LanguageComboBox->GetSelectedIndex() );

	UpdateBasicOptions();
	UpdateGraphicOptions();
}

void UWidget_Settings::OnChangedDialogueSpeedOption(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	GameSettingSubsystem->SetDialogueAnimSpeed((EDialogueWidgetAnimSpeed)DialogueSpeedComboBox->GetSelectedIndex());
}

void UWidget_Settings::UpdateGraphicOptions()
{
	GraphicApplyButton->OnClicked.Clear();
	GraphicApplyButton->OnClicked.AddDynamic(this, &UWidget_Settings::ApplyGraphicSettings);

	UpdateScreenResolutionOptions();
	
	UpdateWindowModeOptions();
	
	UpdateFrameRateLimitOptions();

	VSyncEnableCheckBox->SetCheckedState(GameSettingSubsystem->IsVSyncEnabled() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

	UpdateViewDistanceOptions();
	
	UpdateShadowQualityOptions();
	
	UpdateTextureQualityOptions();
	
	UpdateAntiAliasingOptions();
	
	UpdateBrightnessOptions();
}

void UWidget_Settings::ApplyGraphicSettings()
{
	//해상도
	GameSettingSubsystem->SetScreenResolution(ScreenResolutionComboBox->GetSelectedIndex(), false);

	//화면 모드
	GameSettingSubsystem->SetWindowMode(WindowModeComboBox->GetSelectedIndex(), false);

	//프레임 제한
	GameSettingSubsystem->SetFrameRateLimit(FrameRateLimitComboBox->GetSelectedIndex(), false);

	//수직동기화
	GameSettingSubsystem->SetVSyncEnabled(VSyncEnableCheckBox->IsChecked(), false);

	//가시거리
	GameSettingSubsystem->SetViewDistanceLevel(ViewDistanceComboBox->GetSelectedIndex(), false);

	//그림자 품질
	GameSettingSubsystem->SetShadowQuality(ShadowQualityComboBox->GetSelectedIndex(), false);

	//텍스쳐 품질
	GameSettingSubsystem->SetTextureQuality(TextureQualityComboBox->GetSelectedIndex(), false);

	//안티얼라이어싱
	GameSettingSubsystem->SetAntiAliasingQuality(AntiAliasingQualityComboBox->GetSelectedIndex(), false);
	GameSettingSubsystem->SetAntiAliasingMethod(AntiAliasingMethodComboBox->GetSelectedIndex(), false);

	//밝기
	GameSettingSubsystem->SetBrightness(BrightnessSlider->GetValue(), false);

	GameSettingSubsystem->ApplyGraphicSettings();
}


void UWidget_Settings::UpdateScreenResolutionOptions()
{
	ScreenResolutionComboBox->ClearOptions();

	const TArray<FIntPoint>& ScreenResolutionOptions = GameSettingSubsystem->GetScreenResolutionOptions();
	for (auto ScreenResolution : ScreenResolutionOptions) {
		FString Option = FString::Printf(TEXT("%d X %d"), ScreenResolution.X, ScreenResolution.Y);
		ScreenResolutionComboBox->AddOption(Option);
	}

	FIntPoint CurrentScreenResolution = GameSettingSubsystem->GetCurrentScreenResolution();
	FString CurrentOption = FString::Printf(TEXT("%d X %d"), CurrentScreenResolution.X, CurrentScreenResolution.Y);
	ScreenResolutionComboBox->SetSelectedOption(CurrentOption);
}


void UWidget_Settings::UpdateWindowModeOptions()
{
	WindowModeComboBox->ClearOptions();

	const TArray<FString> WindowModesString = GameSettingSubsystem->GetWindowModeOptions();
	for (auto WindowModeString : WindowModesString) {
		WindowModeComboBox->AddOption(WindowModeString);
	}

	WindowModeComboBox->SetSelectedOption(GameSettingSubsystem->GetCurrentWindowMode());
}



void UWidget_Settings::UpdateFrameRateLimitOptions()
{
	FrameRateLimitComboBox->ClearOptions();

	const TArray<int>& FrameRateLimitOptions = GameSettingSubsystem->GetFrameRateLimitOptions();
	for (auto FrameRateLimitOption : FrameRateLimitOptions) {
		FString Option = FrameRateLimitOption == 0 ? "Unlimited" : FString::FromInt(FrameRateLimitOption);
		FrameRateLimitComboBox->AddOption(Option);
	}
	float SelectedFrameRate = GameSettingSubsystem->GetCurrentFrameRateLimit();
	FString SelectedFrameRateOption = SelectedFrameRate == 0 ? "Unlimited" : FString::FromInt(SelectedFrameRate);
	FrameRateLimitComboBox->SetSelectedOption(SelectedFrameRateOption);
}


void UWidget_Settings::UpdateViewDistanceOptions()
{
	ViewDistanceComboBox->ClearOptions();

	const TArray<FString> ViewDistanceLevelOptions = GameSettingSubsystem->GetViewDistanceLevelOptions();
	for (auto ViewDistanceLevelOption : ViewDistanceLevelOptions) {
		ViewDistanceComboBox->AddOption(ViewDistanceLevelOption);
	}

	ViewDistanceComboBox->SetSelectedIndex(GameSettingSubsystem->GetViewDistanceLevel());
}

void UWidget_Settings::UpdateShadowQualityOptions()
{
	ShadowQualityComboBox->ClearOptions();

	const TArray<FString> ShadowQualityOptions = GameSettingSubsystem->GetShadowQualityOptions();
	for (auto ShadowQualityOption : ShadowQualityOptions) {
		ShadowQualityComboBox->AddOption(ShadowQualityOption);
	}

	ShadowQualityComboBox->SetSelectedIndex(GameSettingSubsystem->GetShadowQuality());
}


void UWidget_Settings::UpdateTextureQualityOptions()
{
	TextureQualityComboBox->ClearOptions();

	const TArray<FString> TextureQualityOptions = GameSettingSubsystem->GetTextureQualityOptions();
	for (auto TextureQualityOption : TextureQualityOptions) {
		TextureQualityComboBox->AddOption(TextureQualityOption);
	}

	TextureQualityComboBox->SetSelectedIndex(GameSettingSubsystem->GetTextureQuality());
}


void UWidget_Settings::UpdateAntiAliasingOptions()
{
	AntiAliasingQualityComboBox->ClearOptions();

	const TArray<FString> AntiAliasingQualityOptions = GameSettingSubsystem->GetAntiAliasingQualityOptions();
	for (auto AntiAliasingQualityOption : AntiAliasingQualityOptions) {
		AntiAliasingQualityComboBox->AddOption(AntiAliasingQualityOption);
	}

	AntiAliasingQualityComboBox->SetSelectedIndex(GameSettingSubsystem->GetAntiAliasingQuality());

	AntiAliasingMethodComboBox->OnSelectionChanged.Clear();
	AntiAliasingMethodComboBox->ClearOptions();

	const TArray<FString>& AntiAliasingMethodOptions = GameSettingSubsystem->GetAntiAliasingMethodOptions();
	for (auto AntiAliasingMethodOption : AntiAliasingMethodOptions) {
		AntiAliasingMethodComboBox->AddOption(AntiAliasingMethodOption);
	}

	AntiAliasingMethodComboBox->SetSelectedIndex(GameSettingSubsystem->GetAntiAliaisingMethodIDX());
}

void UWidget_Settings::OnChangedBrightness(float NewValue)
{
	BrightnessTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), NewValue)));
}

void UWidget_Settings::UpdateBrightnessOptions()
{
	BrightnessSlider->OnValueChanged.Clear();

	BrightnessSlider->SetMaxValue(100);
	float CurrentBrightness = GameSettingSubsystem->GetBrightness();
	BrightnessTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentBrightness)));

	BrightnessSlider->SetValue(CurrentBrightness);

	BrightnessSlider->OnValueChanged.AddDynamic(this, &UWidget_Settings::OnChangedBrightness);
}


