// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Settings.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UWidget_Settings : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	TWeakObjectPtr<class UGameSettingSubsystem> GameSettingSubsystem;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Widget")
	void VisibleSettingsWidget();
	
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Widget")
	void HiddenSettingsWidget();

///////////// 일반 //////////
private:
	void UpdateBasicOptions();

	///////////// 언어 //////////
private:
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* LanguageComboBox;

	UFUNCTION()
	void OnChangedLanguageOption(FString SelectedItem, ESelectInfo::Type SelectionType);


	///////////// 대화속도 //////////
private:
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* DialogueSpeedComboBox;

	UFUNCTION()
	void OnChangedDialogueSpeedOption(FString SelectedItem, ESelectInfo::Type SelectionType);


///////////// 그래픽 //////////
private:
	void UpdateGraphicOptions();

	UPROPERTY(meta = (BindWidget))
	class UButton* GraphicApplyButton;

	UFUNCTION()
	void ApplyGraphicSettings();

	///////////// 화면비 //////////
private:
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ScreenResolutionComboBox;

	void UpdateScreenResolutionOptions();

	///////////// 화면 모드 //////////
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* WindowModeComboBox;

	void UpdateWindowModeOptions();

	///////////// 프레임 //////////
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* FrameRateLimitComboBox;

	void UpdateFrameRateLimitOptions();

	///////////// VSync //////////
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* VSyncEnableCheckBox;

	///////////// 가시거리 //////////
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ViewDistanceComboBox;

	void UpdateViewDistanceOptions();

	///////////// 그림자 //////////
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ShadowQualityComboBox;

	void UpdateShadowQualityOptions();

	///////////// Texture //////////
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* TextureQualityComboBox;

	void UpdateTextureQualityOptions();

	///////////// AntiAliasing //////////
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* AntiAliasingQualityComboBox;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* AntiAliasingMethodComboBox;

	int PreApply_AAMethod;

	void UpdateAntiAliasingOptions();

	//////////// 밝기 /////////////
	UPROPERTY(meta = (BindWidget))
	class USlider* BrightnessSlider;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BrightnessTextBlock;

	UFUNCTION()
	void OnChangedBrightness(float NewValue);

	void UpdateBrightnessOptions();
};
