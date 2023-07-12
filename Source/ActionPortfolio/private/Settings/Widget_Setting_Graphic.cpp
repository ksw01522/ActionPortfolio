// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/Widget_Setting_Graphic.h"
#include "ActionPortfolio.h"
#include "ActionPortfolioInstance.h"
#include "Settings/GameSettingSubsystem.h"
#include "Components/ComboBoxString.h"
#include "Math/IntPoint.h"



UWidget_Setting_Graphic::UWidget_Setting_Graphic(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UWidget_Setting_Graphic::ChangeScreenResolution(int Idx) const
{
	GameSettingSubsystem->ChangeScreenResolution(Idx);
}

void UWidget_Setting_Graphic::OnChangedScreenResolutionOption(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	GameSettingSubsystem->ChangeScreenResolution( ScreenResolutionComboBox->FindOptionIndex(SelectedItem));
}

void UWidget_Setting_Graphic::OnChangedWindowModeOption(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	GameSettingSubsystem->SetWindowMode(SelectedItem);
}

void UWidget_Setting_Graphic::UpdateWindowModeOptions()
{
	WindowModeComboBox->ClearOptions();

	const TArray<FString> WindowModesString = GameSettingSubsystem->GetWindowModeOptions();
	for (auto WindowModeString : WindowModesString) {
		WindowModeComboBox->AddOption(WindowModeString);
	}

	WindowModeComboBox->SetSelectedOption(GameSettingSubsystem->GetCurrentWindowMode());
}

void UWidget_Setting_Graphic::NativeConstruct()
{
	Super::NativeConstruct();

	GameSettingSubsystem = UGameInstance::GetSubsystem<UGameSettingSubsystem>(GetWorld()->GetGameInstance());

	//해상도 콤보박스
	const TArray<FIntPoint>& ScreenResolutionOptions = GameSettingSubsystem->GetScreenResolutionOptions();
	for (auto ScreenResolution : ScreenResolutionOptions) {
		FString Option = FString::Printf(TEXT("%d X %d"), ScreenResolution.X, ScreenResolution.Y);
		ScreenResolutionComboBox->AddOption(Option);
	}

	FIntPoint CurrentScreenResolution = GameSettingSubsystem->GetCurrentScreenResolution();
	FString CurrentOption = FString::Printf(TEXT("%d X %d"), CurrentScreenResolution.X, CurrentScreenResolution.Y);
	ScreenResolutionComboBox->SetSelectedOption(CurrentOption);

	ScreenResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UWidget_Setting_Graphic::OnChangedScreenResolutionOption);

	//윈도우 모드 콤보박스
	UpdateWindowModeOptions();
	WindowModeComboBox->OnSelectionChanged.AddDynamic(this, &UWidget_Setting_Graphic::OnChangedWindowModeOption);
}
