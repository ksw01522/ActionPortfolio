// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Widget/UserWidget_MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Character/Player/ActionPFPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Settings/Widget_Settings.h"
#include "ActionPortfolio.h"

void UUserWidget_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnClicked.AddDynamic(this, &UUserWidget_MainMenu::OnResumeBTNClick);
	SettingButton->OnClicked.AddDynamic(this, &UUserWidget_MainMenu::OnSettingBTNClick);
	GameExitButton->OnClicked.AddDynamic(this, &UUserWidget_MainMenu::OnGameExitBTNClick);

	SettingWidget->SetOnExitSettingEvent(FSimpleDelegate::CreateUObject(this, &UUserWidget_MainMenu::ReturnToMenu));

}

void UUserWidget_MainMenu::OnResumeBTNClick()
{
	AActionPFPlayerController* Player = GetOwningPlayer<AActionPFPlayerController>();
	
	if (Player == nullptr)
	{
		RemoveFromParent();
	}
	else
	{
		Player->ChangeGameInputMode();
		Player->DisplayMainUI();
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUserWidget_MainMenu::OnSettingBTNClick()
{
	WindowSwitcher->SetActiveWidget(SettingWidget);
	SettingWidget->OnEnterSettings(GetOwningPlayer());
}

void UUserWidget_MainMenu::OnGameExitBTNClick()
{
	UKismetSystemLibrary::QuitGame(GetOwningPlayer(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UUserWidget_MainMenu::ReturnToMenu()
{
	WindowSwitcher->SetActiveWidget(MainWidget);
	ResumeButton->SetUserFocus(GetOwningPlayer());
}
