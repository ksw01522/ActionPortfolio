// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CustomInputDeveloperSettings.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FKeySettingContainer
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	FKey Keyboard;

	UPROPERTY(EditAnywhere)
	FKey Gamepad;
};

UCLASS(DisplayName = "Custom Input", Config = "Game", defaultconfig)
class CUSTOMINPUTSETTINGRUNTIME_API UCustomInputDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UCustomInputDeveloperSettings();

private:
	UPROPERTY(EditDefaultsOnly, Category = "PlayerInput", GlobalConfig, meta = (RequiredAssetDataTags = "RowStructure=/Script/CustomInputSettingRuntime.KeySettingMetaData"))
	TSoftObjectPtr<class UDataTable> KeySettingMetaDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerInput", GlobalConfig)
	TMap<EUINavigation, FKeySettingContainer> NavigationKeys;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerInput", GlobalConfig)
	TMap<EUINavigationAction, FKeySettingContainer> NavigationActionKeys;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerInput", GlobalConfig)
	TMap<FName, FKeySettingContainer> NavigationOptionalActionKeys;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerInput", GlobalConfig)
	TSet<FKey> IgnoreMappingKeys;

	virtual FName GetContainerName() const override
	{
		return "Project";
	}

	virtual FName GetCategoryName() const override
	{
		return "Input";
	}

	virtual FName GetSectionName() const override
	{
		return "CustomInput";
	}

public:
	const TSoftObjectPtr<UDataTable> GetKeySettingMetaDataTable() const { return KeySettingMetaDataTable; }
	const TMap<EUINavigation, FKeySettingContainer>& GetCustomNavigationKeys() const { return NavigationKeys; }
	const TMap<EUINavigationAction, FKeySettingContainer>& GetCustomNavigationActionKeys() const { return NavigationActionKeys; }
	const TMap<FName, FKeySettingContainer>& GetNavigationOptionalActionKeys() const { return NavigationOptionalActionKeys; }
	const TSet<FKey> GetIgnoreMappingKeys() const { return IgnoreMappingKeys; }

#if WITH_EDITOR
private:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void CheckNavigationKey();

	//virtual FText GetSectionText() const;
	//virtual FText GetSectionDescription() const;
#endif


//슬레이트 관련
private:
	UPROPERTY(EditDefaultsOnly, Category = "Slate", GlobalConfig)
	FVector2D GuideBoxSize;

	UPROPERTY(EditDefaultsOnly, Category = "Slate", GlobalConfig)
	FMargin GuideBoxPadding;

	UPROPERTY(EditDefaultsOnly, Category = "Slate", GlobalConfig)
	FSlateBrush GuideBorderBrush;

	UPROPERTY(EditDefaultsOnly, Category = "Slate|Text", GlobalConfig)
	FTextBlockStyle NameTextStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Slate|Text", GlobalConfig)
	FTextBlockStyle GuideTextStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Slate|Text", GlobalConfig)
	FTextBlockStyle SupportTextStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Slate|Text", GlobalConfig)
	FSlateColor DefaultTextColor;

	UPROPERTY(EditDefaultsOnly, Category = "Slate|Text", GlobalConfig)
	FSlateColor ErrorColor;

public:
	FMargin GetGuideBoxPadding() const { return GuideBoxPadding; }

	const FSlateBrush* GetGuideBorderBrush() const { return &GuideBorderBrush; }

	const FTextBlockStyle* GetNameTextStyle() const { return &NameTextStyle; }

	const FTextBlockStyle* GetGuideTextStyle() const { return &GuideTextStyle; }

	const FTextBlockStyle* GetSupportTextStyle() const { return &SupportTextStyle; }

	const FSlateColor& GetDefaultTextColor() const { return DefaultTextColor; }

	const FSlateColor& GetErrorColor() const { return ErrorColor; }

	FVector2D GetGuideBoxSize() const { return GuideBoxSize; }
};
