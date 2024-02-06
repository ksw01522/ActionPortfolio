// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/ChangeInputDevice.h"
#include "CustomInputSettingRuntime.h"

#define LOCTEXT_NAMESPACE "CustomInputSetting"


void UCheckInputDeviceProcessor::SetCurrentInputDevice(EInputDeviceType InDevice)
{
	if (CurrentDevice == InDevice) return;

	CurrentDevice = InDevice;
	DeviceChangeDel.Broadcast(CurrentDevice);
}

FDelegateHandle UCheckInputDeviceProcessor::AddInputDeviceChangeEvent(const TDelegate<void(EInputDeviceType)>& InDelegate)
{
	return DeviceChangeDel.Add(InDelegate);
}

void UCheckInputDeviceProcessor::RemoveInputDeviceChangeEvent(const FDelegateHandle& InHandle)
{
	DeviceChangeDel.Remove(InHandle);
}

bool UCheckInputDeviceProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	EInputDeviceType NewDevice = InKeyEvent.GetKey().IsGamepadKey() ? EInputDeviceType::Gamepad : EInputDeviceType::Keyboard;

	SetCurrentInputDevice(NewDevice);

	return false;
}

bool UCheckInputDeviceProcessor::HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent)
{
	if (0.25 < InAnalogInputEvent.GetAnalogValue()) {
		SetCurrentInputDevice(EInputDeviceType::Gamepad);
	}

	return false;
}

bool UCheckInputDeviceProcessor::HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	if (4 < MouseEvent.GetCursorDelta().SizeSquared()) {
		SetCurrentInputDevice(EInputDeviceType::Keyboard);
	}

	return false;
}

bool UCheckInputDeviceProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	SetCurrentInputDevice(EInputDeviceType::Keyboard);

	return false;
}

bool UCheckInputDeviceProcessor::HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent)
{
	SetCurrentInputDevice(EInputDeviceType::Keyboard);

	return false;
}


#undef LOCTEXT_NAMESPACE