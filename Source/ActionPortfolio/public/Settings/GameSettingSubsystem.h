// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSettingSubsystem.generated.h"

/**
 * 
 */
UCLASS(Config = ActionPortfolioSetting)
class ACTIONPORTFOLIO_API UGameSettingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	UGameSettingSubsystem();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

///////////// 그래픽 //////////

	///////////// 화면비 //////////
private:
	TArray<FIntPoint> ScreenResolutions;

public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void ChangeScreenResolution(int Idx);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const TArray<FIntPoint>& GetScreenResolutionOptions() const {return ScreenResolutions;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const FIntPoint GetCurrentScreenResolution() const;

	///////////// 화면 모드 //////////
private:
	FText WindowedMode;
	FText WindowedFullScreenMode;
	FText FullScreenMode;

public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetWindowMode(FString NewMode);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const TArray<FString> GetWindowModeOptions() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	FString GetCurrentWindowMode() const;

	///////////// 프레임 //////////
private:
	TArray<int> FrameRateLimitOptions;

public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void ChangeFrameRateLimit(int Idx);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	float GetCurrentFrameRateLimit() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const TArray<int>& GetFrameRateLimitOptions() const {return FrameRateLimitOptions; };

///////////// VSync //////////
public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetVSyncEnabled(bool NewState);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	bool IsVSyncEnabled() const;

///////////// ViewDistance //////////
public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetViewDistanceLevel(int32 NewLevel);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	int32 GetViewDistanceLevel() const;

///////////// Shadow //////////
public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetShadowQuality(int32 NewLevel);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	int32 GetShadowQuality() const;

///////////// Texture //////////
public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetTextureQuality(int32 NewLevel);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	int32 GetTextureQuality() const;

///////////// Anti-Aliasing //////////
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetAntiAliasingQuality(int32 NewLevel);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	int32 GetAntiAliasingQuality() const;

private:
	void ForStudy();
};
