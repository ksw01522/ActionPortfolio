// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Application/IInputProcessor.h"


UENUM()
enum class EInputDeviceType : uint8
{
	None = 0 UMETA(Hidden),
	Keyboard,
	Gamepad
};


/**
 *
 */
class UCheckInputDeviceProcessor : public IInputProcessor
{
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputDeviceChangeDelegate, EInputDeviceType);

private:
	EInputDeviceType CurrentDevice = EInputDeviceType::Keyboard;

	void SetCurrentInputDevice(EInputDeviceType InDevice);

public:
	EInputDeviceType GetCurrentInputDevice() const { return CurrentDevice; }

private:
	FOnInputDeviceChangeDelegate DeviceChangeDel;

public:
	FDelegateHandle AddInputDeviceChangeEvent(const TDelegate<void(EInputDeviceType)>& InDelegate);
	void RemoveInputDeviceChangeEvent(const FDelegateHandle& InHandle);

protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {};

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;

	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override;

	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;

	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent) override;

	virtual const TCHAR* GetDebugName() const override { return TEXT("Check Input Device"); }
};

