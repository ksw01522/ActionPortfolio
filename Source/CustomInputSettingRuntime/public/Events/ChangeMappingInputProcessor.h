// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Application/IInputProcessor.h"
/**
 * 
 */

class UChangeMappingInputProcessor : public IInputProcessor, public TSharedFromThis<UChangeMappingInputProcessor>
{
public:
	UChangeMappingInputProcessor();
	virtual ~UChangeMappingInputProcessor();

	DECLARE_DELEGATE_RetVal_FourParams(bool, FOnTryCustomMapping, const FName&, const FKey&, bool, FText&);

private:
	FName Code;
	bool bIsGamepad;

	TSharedPtr<class SCustomMappingSlate> MappingSlate;

	FOnTryCustomMapping OnTryCustomMapping;

protected:
	void ReceiveInputForCustomMapping(const FKey& InKey);

public:
	FDelegateHandle RegistryTryCustomMappingEvent(FOnTryCustomMapping NewEvent);



	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {};

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;

	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override { return true; }

	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override { return true; }

	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override; 

	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return true; }

	virtual bool HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;

	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent) override { return true; }

	virtual bool HandleMotionDetectedEvent(FSlateApplication& SlateApp, const FMotionEvent& MotionEvent) override { return true; };

	virtual const TCHAR* GetDebugName() const override { return TEXT("Change Mapping Input Processor"); }

public:
	void WaitReceiveInput(const FName& InCode, bool InbIsGamepad, const FText& InDisplayName);
	void StopWaitReceiveInput();
};

class UWaitInputForResetMappingInputProcessor : public IInputProcessor, public TSharedFromThis<UWaitInputForResetMappingInputProcessor>
{
public:
	DECLARE_DELEGATE_OneParam(FOnResetMappingDelegate, class UPlayerMappableInputConfig*);

	UWaitInputForResetMappingInputProcessor();
	virtual ~UWaitInputForResetMappingInputProcessor();
private:
	TSharedPtr<class SCustomMappingReset> ResetSlate;
	
	TObjectPtr<UPlayerMappableInputConfig> TargetConfig;

	FOnResetMappingDelegate OnResetMapping;

public:
	FDelegateHandle RegistryResetMappingEvent(FOnResetMappingDelegate InDel);

	void WaitForResetMapping(UPlayerMappableInputConfig* InConfig);

private:
	void StopWaitForResetMapping();

protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {};

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	
	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override { return true; }

	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override { return true; }

	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return true; }

	virtual bool HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return true; }

	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent) override { return true; }

	virtual bool HandleMotionDetectedEvent(FSlateApplication& SlateApp, const FMotionEvent& MotionEvent) override { return true; };

	virtual const TCHAR* GetDebugName() const override { return TEXT("Change Mapping Input Processor"); }

};
