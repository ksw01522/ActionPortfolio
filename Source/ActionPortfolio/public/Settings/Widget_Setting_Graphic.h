// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Setting_Graphic.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UWidget_Setting_Graphic : public UUserWidget
{
	GENERATED_BODY()
	UWidget_Setting_Graphic(const FObjectInitializer& ObjectInitializer);

protected:
	TWeakObjectPtr<class UGameSettingSubsystem> GameSettingSubsystem;

///////////// 화면비 //////////
private:
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ScreenResolutionComboBox;

	void ChangeScreenResolution(int Idx) const;

	UFUNCTION()
	void OnChangedScreenResolutionOption(FString SelectedItem, ESelectInfo::Type SelectionType);

///////////// 화면 모드 //////////
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* WindowModeComboBox;

	UFUNCTION()
	void OnChangedWindowModeOption(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdateWindowModeOptions();

protected:
	virtual void NativeConstruct() override;


};
