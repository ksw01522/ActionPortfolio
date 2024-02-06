// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
enum class EInputDeviceType : uint8;

class SImage;

class CUSTOMINPUTSETTINGRUNTIME_API SInputKeyLabel : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SInputKeyLabel, SCompoundWidget)

public:
	SInputKeyLabel();
	virtual ~SInputKeyLabel();

	SLATE_BEGIN_ARGS(SInputKeyLabel)
	{
		_Visibility = EVisibility::HitTestInvisible;
	}

	SLATE_ARGUMENT(FKey, KeyboardKey)
	SLATE_ARGUMENT(FKey, GamepadKey)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	FSlateBrush LabelBrush_Keyboard;
	FSlateBrush LabelBrush_Gamepad;

	TSharedPtr<SImage> LabelImage;
	
	FDelegateHandle OnChangeInputDeviceHandle;

public:
	void OnChangedCurrentDevice(EInputDeviceType InDevice);

	void SetKeyboardKey(const FKey& InKey);
	void SetGamepadKey(const FKey& InKey);
};
