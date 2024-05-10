// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UUserWidget_MainMenu : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> WindowSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> MainWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidget_Settings> SettingWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ResumeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SettingButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> GameExitButton;

	

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnResumeBTNClick();

	UFUNCTION()
	void OnSettingBTNClick();

	UFUNCTION()
	void OnGameExitBTNClick();

	UFUNCTION()
	void ReturnToMenu();
};
