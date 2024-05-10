// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "ActionPFGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UActionPFGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UActionPFGameUserSettings();
	virtual ~UActionPFGameUserSettings() {};
	
public:
	static UActionPFGameUserSettings* Get();

private:
	TArray<FText> BasicLevelOptions;

//////////// 그래픽 //////////
public:
	void ApplyGraphicSettings();

	///////////// 화면비 //////////
private:
	TArray<FIntPoint> ScreenResolutionOptions;

public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Setting|Graphic")
	void SetScreenResolutionByIndex(int Idx, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Setting|Graphic")
	const TArray<FIntPoint>& GetScreenResolutionOptions() const { return ScreenResolutionOptions; }

///////////// 화면 모드 //////////
private:
	TArray<FText> WindowModeNames;

public:
	UFUNCTION(BlueprintCallable, Category = "Setting|Graphic")
	void SetWindowMode(int Idx, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Setting|Graphic")
	const TArray<FText>& GetWindowModeOptions() const { return WindowModeNames; }

	/*
		0 = FullScreen,
		1 = WindowedFullScreen,
		2 = Window
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Setting|Graphic")
	int GetCurrentWindowMode() const;

	///////////// 프레임 //////////
private:
	TArray<int> FrameRateLimitOptions;

public:
	UFUNCTION(BlueprintCallable, Category = "Setting|Graphic")
	void SetFrameRateLimitByIdx(int Idx, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Setting|Graphic")
	const TArray<int>& GetFrameRateLimitOptions() const { return FrameRateLimitOptions; };

private:
	TArray<FText> ViewDistanceLevelOptions;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Setting|Graphic")
	const TArray<FText>& GetViewDistanceLevelOptions() const { return ViewDistanceLevelOptions; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Setting|Graphic")
	const TArray<FText>& GetShadowQualityOptions() const {return BasicLevelOptions;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Setting|Graphic")
	const TArray<FText> GetTextureQualityOptions() const { return BasicLevelOptions; }


private:
	TArray<FText> AAMethodOptions;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Setting|Graphic")
	const TArray<FText>& GetAntiAliasingQualityOptions() const { return BasicLevelOptions; }

	UFUNCTION(BlueprintCallable, Category = "Setting|Graphic")
	void SetAntiAliasingMethod(int32 NewType, bool bApply);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Setting|Graphic")
	const TArray<FText>& GetAntiAliasingMethodOptions() const { return AAMethodOptions; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Setting|Graphic")
	int GetAntiAliasingMethod() const;
};
