// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SInputKeyLabel.h"
#include "SlateOptMacros.h"
#include "CustomInputSettingSubsystem.h"
#include "Widgets/Images/SImage.h"
#include "Events/ChangeInputDevice.h"
#include "CustomInputSettingRuntime.h"

#define LOCTEXT_NAMESPACE "CustomInputSetting"

SLATE_IMPLEMENT_WIDGET(SInputKeyLabel)
void SInputKeyLabel::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
	
}

SInputKeyLabel::SInputKeyLabel()
{
	
}

SInputKeyLabel::~SInputKeyLabel()
{
	UCustomInputSettingSubsystem* SystemInst =  UCustomInputSettingSubsystem::GetInstance();
	if(SystemInst != nullptr) { SystemInst->RemoveInputDeviceChangeEvent(OnChangeInputDeviceHandle); }
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SInputKeyLabel::Construct(const FArguments& InArgs)
{
	SetVisibility(InArgs._Visibility);
	SetKeyboardKey(InArgs._KeyboardKey);
	SetGamepadKey(InArgs._GamepadKey);

	ChildSlot
	[
		SAssignNew(LabelImage, SImage)
	];

	UCustomInputSettingSubsystem* SystemInst = UCustomInputSettingSubsystem::GetInstance();

#if WITH_EDITOR
	if (SystemInst != nullptr)	{	OnChangeInputDeviceHandle = SystemInst->AddInputDeviceChangeEvent(TDelegate<void (EInputDeviceType)>::CreateSP
																								(this, &SInputKeyLabel::OnChangedCurrentDevice)); 
									
									OnChangedCurrentDevice( SystemInst->GetCurrentInputDevice());
								}
#else
	OnChangeInputDeviceHandle = SystemInst->AddInputDeviceChangeEvent(TDelegate<void(EInputDeviceType)>::CreateSP
																	(this, & SInputKeyLabel::OnChangedCurrentDevice));
	OnChangedCurrentDevice(SystemInst->GetCurrentInputDevice());
#endif
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION



void SInputKeyLabel::OnChangedCurrentDevice(EInputDeviceType InDevice)
{
	if (InDevice == EInputDeviceType::Keyboard)
	{
		LabelImage->SetImage(&LabelBrush_Keyboard);
	}
	else
	{
		LabelImage->SetImage(&LabelBrush_Gamepad);
	}
}

void SInputKeyLabel::SetKeyboardKey(const FKey& InKey)
{
	UCustomInputSettingSubsystem* SystemInst = UCustomInputSettingSubsystem::GetInstance();

#if WITH_EDITOR
	if(SystemInst == nullptr) return;
#endif

	LabelBrush_Keyboard.SetResourceObject(SystemInst->GetKeyIcon(InKey));
}

void SInputKeyLabel::SetGamepadKey(const FKey& InKey)
{
	UCustomInputSettingSubsystem* SystemInst = UCustomInputSettingSubsystem::GetInstance();

#if WITH_EDITOR
	if (SystemInst == nullptr) return;
#endif

	LabelBrush_Gamepad.SetResourceObject(SystemInst->GetKeyIcon(InKey));
}

#undef LOCTEXT_NAMESPACE