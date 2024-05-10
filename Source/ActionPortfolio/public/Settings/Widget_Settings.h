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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SettingWidgetSwitcher", meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> SettingWidgetSwitcher;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomInput", meta = (BindWidget))
	TObjectPtr<class UCustomInputSettingWidget> CustomMappingWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomInput", meta = (BindWidget))
	TObjectPtr<class UGameSettingWindowWidget_Graphic> GraphicSettingWidget; 

	FSimpleDelegate OnExitSettingDelegate;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> GraphicSettingButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> InputSettingButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ExitButton;

private:
	UFUNCTION()
	void OnGraphicSettingBTNClick();
	
	UFUNCTION()
	void OnInputSettingBTNClick();

	UFUNCTION()
	void OnExitSetting();

	void ShowCustomMapping();
	void EnterCustomMapping();

	void ShowGraphicSetting();
	void EnterGraphicSetting();

public:
	void SetOnExitSettingEvent(const FSimpleDelegate& InEvent) { OnExitSettingDelegate = InEvent; }

	void OnEnterSettings(APlayerController* InPlayer);
};
