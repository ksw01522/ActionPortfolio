// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/ChangeMappingInputProcessor.h"
#include "Widget/SCustomMapping.h"
#include "CustomInputDeveloperSettings.h"
#include "CustomInputSettingSubsystem.h"
#include "CustomInputSettingRuntime.h"
#include "Widget/SCustomInputSetting.h"
#include "Widget/SCustomMappingReset.h"

void UChangeMappingInputProcessor::ReceiveInputForCustomMapping(const FKey& InKey)
{
	if (Code.IsNone()) {
		LOG_ERROR(TEXT("Wait Input For Mapping Code : NAME_None"));
		StopWaitReceiveInput();
		return;
	}

	FText ErrorTxt = FText::GetEmpty();
	bool Result = OnTryCustomMapping.Execute(Code, InKey, bIsGamepad, ErrorTxt);

	if (Result)
	{
		StopWaitReceiveInput();
	}
	else
	{
		MappingSlate->ShowErrorText(ErrorTxt);
	}
}

FDelegateHandle UChangeMappingInputProcessor::RegistryTryCustomMappingEvent(FOnTryCustomMapping NewEvent)
{
	OnTryCustomMapping = NewEvent;

	return OnTryCustomMapping.GetHandle();
}

UChangeMappingInputProcessor::UChangeMappingInputProcessor()
{
	const UCustomInputDeveloperSettings* DevelopSetting = GetDefault<UCustomInputDeveloperSettings>();

	FMargin Padding = DevelopSetting->GetGuideBoxPadding();
	const FSlateBrush* BorderBrush = DevelopSetting->GetGuideBorderBrush();
	const FTextBlockStyle* NameStyle = DevelopSetting->GetNameTextStyle();
	const FTextBlockStyle* SupportStyle = DevelopSetting->GetSupportTextStyle();
	FVector2D BoxSize = DevelopSetting->GetGuideBoxSize();
	const FSlateColor& TextColor = DevelopSetting->GetDefaultTextColor();
	const FSlateColor& ErrorColor = DevelopSetting->GetErrorColor();

	SAssignNew(MappingSlate, SCustomMappingSlate)
		.Padding(Padding)
		.BorderImage(BorderBrush)
		.NameBlockStyle(NameStyle)
		.GuideBlockStyle(SupportStyle)
		.BoxSize(BoxSize)
		.TextColor(TextColor)
		.ErrorColor(ErrorColor);
}

UChangeMappingInputProcessor::~UChangeMappingInputProcessor()
{
    if (GEngine != nullptr && GEngine->GameViewport != nullptr)
    {
        GEngine->GameViewport->RemoveViewportWidgetContent(MappingSlate.ToSharedRef());
        MappingSlate.Reset();
    }
}

bool UChangeMappingInputProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	ReceiveInputForCustomMapping(InKeyEvent.GetKey());
    return true;
}

bool UChangeMappingInputProcessor::HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	return true;
}

bool UChangeMappingInputProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	ReceiveInputForCustomMapping(MouseEvent.GetEffectingButton());
    return true;
}

bool UChangeMappingInputProcessor::HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	ReceiveInputForCustomMapping(MouseEvent.GetEffectingButton());

	return true;
}

void UChangeMappingInputProcessor::WaitReceiveInput(const FName& InCode, bool InbIsGamepad, const FText& InDisplayName)
{	
	Code = InCode;
	bIsGamepad = InbIsGamepad;

	MappingSlate->ReadyToCustomMapping(InDisplayName);
	GEngine->GameViewport->AddViewportWidgetContent(MappingSlate.ToSharedRef(), 100);

	FSlateApplication::Get().RegisterInputPreProcessor(AsShared());
}

void UChangeMappingInputProcessor::StopWaitReceiveInput()
{
	Code = NAME_None;

	GEngine->GameViewport->RemoveViewportWidgetContent(MappingSlate.ToSharedRef());
	FSlateApplication::Get().UnregisterInputPreProcessor(AsShared());

}


UWaitInputForResetMappingInputProcessor::UWaitInputForResetMappingInputProcessor()
{
	const UCustomInputDeveloperSettings* DevelopSetting = GetDefault<UCustomInputDeveloperSettings>();

	FMargin Padding = DevelopSetting->GetGuideBoxPadding();
	const FSlateBrush* BorderBrush = DevelopSetting->GetGuideBorderBrush();
	const FTextBlockStyle* GuideStyle = DevelopSetting->GetGuideTextStyle();
	const FTextBlockStyle* SupportStyle = DevelopSetting->GetSupportTextStyle();
	FVector2D BoxSize = DevelopSetting->GetGuideBoxSize();
	const FSlateColor& TextColor = DevelopSetting->GetDefaultTextColor();
	const FSlateColor& ErrorColor = DevelopSetting->GetErrorColor();

	SAssignNew(ResetSlate, SCustomMappingReset)
		.Padding(Padding)
		.BorderImage(BorderBrush)
		.GuideBlockStyle(GuideStyle)
		.SupportBlockStyle(SupportStyle)
		.BoxSize(BoxSize)
		.TextColor(TextColor);
}

UWaitInputForResetMappingInputProcessor::~UWaitInputForResetMappingInputProcessor()
{
	if (GEngine != nullptr && GEngine->GameViewport != nullptr)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(ResetSlate.ToSharedRef());
		ResetSlate.Reset();
	}
}

FDelegateHandle UWaitInputForResetMappingInputProcessor::RegistryResetMappingEvent(FOnResetMappingDelegate InDel)
{
	OnResetMapping = InDel;

	return OnResetMapping.GetHandle();
}

bool UWaitInputForResetMappingInputProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	EUINavigationAction NavAction =  SlateApp.GetNavigationActionFromKey(InKeyEvent);

	if (NavAction == EUINavigationAction::Accept)
	{
		OnResetMapping.ExecuteIfBound(TargetConfig);
		StopWaitForResetMapping();
	}
	else if(NavAction == EUINavigationAction::Back)
	{
		StopWaitForResetMapping();
	}

	return true;
}

void UWaitInputForResetMappingInputProcessor::WaitForResetMapping(UPlayerMappableInputConfig* InConfig)
{
	TargetConfig = InConfig;

	GEngine->GameViewport->AddViewportWidgetContent(ResetSlate.ToSharedRef(), 100);

	FSlateApplication& SlateApp = FSlateApplication::Get();
	SlateApp.RegisterInputPreProcessor(AsShared());
}

void UWaitInputForResetMappingInputProcessor::StopWaitForResetMapping()
{
	TargetConfig = nullptr;

	GEngine->GameViewport->RemoveViewportWidgetContent(ResetSlate.ToSharedRef());

	FSlateApplication& SlateApp = FSlateApplication::Get();
	SlateApp.UnregisterInputPreProcessor(AsShared());
}
