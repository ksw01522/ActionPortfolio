// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/Widget_Settings.h"
#include "ActionPortfolio.h"
#include "Instance/ActionPortfolioInstance.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Widget/CustomInputSettingWidget.h"
#include "Settings/GameSettingWindowWidget.h"

void UWidget_Settings::NativeConstruct()
{
	Super::NativeConstruct();

	GraphicSettingButton->SetAllNavigationRules(EUINavigationRule::Stop, NAME_None);
	InputSettingButton->SetAllNavigationRules(EUINavigationRule::Stop, NAME_None);
	ExitButton->SetAllNavigationRules(EUINavigationRule::Stop, NAME_None);

	GraphicSettingButton->SetNavigationRuleExplicit(EUINavigation::Down, InputSettingButton);

	InputSettingButton->SetNavigationRuleExplicit(EUINavigation::Up, GraphicSettingButton);
	InputSettingButton->SetNavigationRuleExplicit(EUINavigation::Down, ExitButton);

	ExitButton->SetNavigationRuleExplicit(EUINavigation::Up, InputSettingButton);

	GraphicSettingButton->OnClicked.AddDynamic(this, &UWidget_Settings::OnGraphicSettingBTNClick);
	InputSettingButton->OnClicked.AddDynamic(this, &UWidget_Settings::OnInputSettingBTNClick);
	ExitButton->OnClicked.AddDynamic(this, &UWidget_Settings::OnExitSetting);
}

void UWidget_Settings::OnGraphicSettingBTNClick()
{
	SettingWidgetSwitcher->SetActiveWidget(GraphicSettingWidget);
	GraphicSettingWidget->SetUserFocus(GetOwningPlayer());
}

void UWidget_Settings::OnInputSettingBTNClick()
{
	SettingWidgetSwitcher->SetActiveWidget(CustomMappingWidget);
	CustomMappingWidget->SetUserFocus(GetOwningPlayer());
}

void UWidget_Settings::OnExitSetting()
{
	if (OnExitSettingDelegate.IsBound())
	{
		OnExitSettingDelegate.ExecuteIfBound();
	}
	else
	{
		RemoveFromParent();
	}
}

void UWidget_Settings::ShowCustomMapping()
{
	SettingWidgetSwitcher->SetActiveWidget(CustomMappingWidget);
}

void UWidget_Settings::EnterCustomMapping()
{
	ShowCustomMapping();
	CustomMappingWidget->SetUserFocus(GetOwningPlayer());
}

void UWidget_Settings::ShowGraphicSetting()
{
	SettingWidgetSwitcher->SetActiveWidget(GraphicSettingWidget);
}

void UWidget_Settings::EnterGraphicSetting()
{
	ShowGraphicSetting();
	GraphicSettingWidget->SetUserFocus(GetOwningPlayer());
}

void UWidget_Settings::OnEnterSettings(APlayerController* InPlayer)
{
	if(InPlayer == nullptr) return;

	GraphicSettingButton->SetUserFocus(InPlayer);
	SettingWidgetSwitcher->SetActiveWidget(GraphicSettingWidget);
}

