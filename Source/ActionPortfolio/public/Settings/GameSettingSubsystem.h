// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Settings/CustomStructForSetting.h"
#include "GameFramework/GameUserSettings.h"
#include "GameSettingSubsystem.generated.h"

/**
 * 
 */
 

UCLASS(Config = ActionPortfolioSetting)
class ACTIONPORTFOLIO_API UGameSettingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	UGameSettingSubsystem();

private:
	friend class UActionPortfolioInstance;
	static TArray<FText> BasicLevelOptions;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

///////////// 일반 /////////////
	UPROPERTY(GlobalConfig)
	EDialogueWidgetAnimSpeed CurrentDialogueAnimSpeed;

	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Basic")
	void SetDialogueAnimSpeed(EDialogueWidgetAnimSpeed NewSpeed) {
		if (CurrentDialogueAnimSpeed == NewSpeed) return;
		CurrentDialogueAnimSpeed = NewSpeed;
		SaveConfig();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Basic")
	const TArray<FString> GetDialogueAnimSpeedOptions() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Basic")
	EDialogueWidgetAnimSpeed GetCurrentDialogueAnimSpeed() const {return CurrentDialogueAnimSpeed; }

	UPROPERTY(GlobalConfig)
	ELanguage CurrentLanguage;

	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Basic")
	void SetCurrentLanguage(ELanguage NewLanguage);

	

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Basic")
	const TArray<FString> GetLanguageOptions() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Basic")
	ELanguage GetCurrentLanguage() const {return CurrentLanguage;}

///////////// 그래픽 //////////
public:
	void ApplyGraphicSettings();

	///////////// 화면비 //////////
private:
	TArray<FIntPoint> ScreenResolutionOptions;

	UPROPERTY(GlobalConfig)
	int Current_ScreenResolution_IDX;

public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetScreenResolution(int Idx, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const TArray<FIntPoint>& GetScreenResolutionOptions() const {return ScreenResolutionOptions;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const FIntPoint GetCurrentScreenResolution() const { return ScreenResolutionOptions[Current_ScreenResolution_IDX]; }

	///////////// 화면 모드 //////////
private:
	FText WindowedMode;
	FText WindowedFullScreenMode;
	FText FullScreenMode;

public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetWindowMode(int Idx, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const TArray<FString> GetWindowModeOptions() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	FString GetCurrentWindowMode() const;

	///////////// 프레임 //////////
private:
	TArray<int> FrameRateLimitOptions;

	UPROPERTY(GlobalConfig)
	int Current_FrameRateLimit_IDX;

public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetFrameRateLimit(int Idx, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	float GetCurrentFrameRateLimit() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const TArray<int>& GetFrameRateLimitOptions() const {return FrameRateLimitOptions; };

///////////// VSync //////////
public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetVSyncEnabled(bool NewState, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	bool IsVSyncEnabled() const;

///////////// 가시거리 //////////
public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetViewDistanceLevel(int32 NewLevel, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	int32 GetViewDistanceLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const TArray<FString> GetViewDistanceLevelOptions() const;

///////////// 그림자 //////////
public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetShadowQuality(int32 NewLevel, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	int32 GetShadowQuality() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const TArray<FString> GetShadowQualityOptions() const;

///////////// Texture //////////
public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetTextureQuality(int32 NewLevel, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	int32 GetTextureQuality() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const TArray<FString> GetTextureQualityOptions() const;

///////////// AntiAliasing //////////
private:
	int32 CurrentAntiAliasingMethodIDX = -1;

public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetAntiAliasingQuality(int32 NewLevel, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	int32 GetAntiAliasingQuality() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const TArray<FString> GetAntiAliasingQualityOptions() const;

	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetAntiAliasingMethod(int32 NewType, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const TArray<FString>& GetAntiAliasingMethodOptions() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const int32 GetAntiAliaisingMethodIDX() const {return CurrentAntiAliasingMethodIDX;}

//////////// 밝기 /////////////
public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetBrightness(float NewValue, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	float GetBrightness() const;


private:
	void ForStudy();
};
