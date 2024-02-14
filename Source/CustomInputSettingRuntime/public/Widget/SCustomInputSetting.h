// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/SBorder.h"
#include "GenericPlatform/ICursor.h"

/**
 * 
 */
class STextBlock;
class APlayerController;
class SScrollBox;





class SCustomInputSettingNode : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SCustomInputSettingNode, SCompoundWidget)

public:
	SCustomInputSettingNode();

public:
	SLATE_BEGIN_ARGS(SCustomInputSettingNode) :
		_Padding(5),
		_TextBorderBrush(nullptr),
		_IconBorderBrush(nullptr),
		_FocusedFrameBrush(nullptr),
		_NameBlockStyle(nullptr),
		_NameText(FText::GetEmpty()),
		_IconSize(32)
		{
			_Clipping = EWidgetClipping::Inherit;
		}

		SLATE_ARGUMENT(FMargin, Padding)
		SLATE_ARGUMENT(const FSlateBrush*, TextBorderBrush)
		SLATE_ARGUMENT(const FSlateBrush*, IconBorderBrush)
		SLATE_ARGUMENT(const FSlateBrush*, FocusedFrameBrush)

		//텍스트 블록
		SLATE_ARGUMENT(const FTextBlockStyle*, NameBlockStyle)
		SLATE_ARGUMENT(FText, NameText)

		//키 관련
		SLATE_ARGUMENT(FName, Code)
		SLATE_ARGUMENT(bool, bIsGamepad)
		SLATE_ARGUMENT(FKey, Key)
		SLATE_ARGUMENT(FVector2D, IconSize)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void SetPadding(FMargin InPadding);

	void SetTextBorderBrush(const FSlateBrush* InBrush);
	void SetIconBorderBrush(const FSlateBrush* InBrush);
	void SetFocusedFrameBrush(const FSlateBrush* InBrush);

	//텍스트 블록 관련
	void SetNameBlockStyle(const FTextBlockStyle* InStyle);
	void SetText(FText InText);

	//Icon 관련
	void SetKeyIcon(const FKey& InKey);

	void SetInputCode(const FName& InCode);

private:
	TSharedPtr<SBorder> NameBorder;
	TSharedPtr<STextBlock> NameTextBlock;
	TSharedPtr<SBorder> IconBorder;
	TSharedPtr<SImage> ButtonIcon;
	TSharedPtr<SImage> FocusedFrame;

	FSlateBrush ButtonBrush;

//기존 SWidget 오버라이드
protected:
	virtual TOptional<bool> OnQueryShowFocus(const EFocusCause InFocusCause) const override;

	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;

	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;

	//키 설정
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	//키 설정
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

	virtual bool SupportsKeyboardFocus() const { return true; }

	//키 설정 관련
private:
	FName Code;
	bool bIsGamepad;


	void RequestCustomKeySettingByNode();


public:
	void SetNavigation(EUINavigation Direction, TSharedPtr<SWidget> InWidget);

	FName GetCode() const {return Code;}

	void SetIconSize(FVector2D InSize);
};

class SCustomInputSettingWindow : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SCustomInputSettingWindow, SCompoundWidget)

public:
	SCustomInputSettingWindow();
	virtual ~SCustomInputSettingWindow();

public:
	SLATE_BEGIN_ARGS(SCustomInputSettingWindow) :
		_NodePadding(5),
		_TextBorderBrush(nullptr),
		_IconBorderBrush(nullptr),
		_FocusedFrameBrush(nullptr),
		_NodeNameBlockStyle(nullptr),
		_SelectedTabBrush(nullptr),
		_UnselectedTabBrush(nullptr),
		_DistanceBetweenNode(5),
		_IconSize(32),
		_TabTextStyle(nullptr),
		_DescriptionTextStyle(nullptr),
		_DescriptionLabelSize(12)
		{
			_Clipping = EWidgetClipping::Inherit;
		}

		SLATE_ARGUMENT(FMargin, NodePadding)
		SLATE_ARGUMENT(const FSlateBrush*, TextBorderBrush)
		SLATE_ARGUMENT(const FSlateBrush*, IconBorderBrush)
		SLATE_ARGUMENT(const FSlateBrush*, FocusedFrameBrush)
		SLATE_ARGUMENT(const FTextBlockStyle*, NodeNameBlockStyle)

		SLATE_ARGUMENT(const FSlateBrush*, SelectedTabBrush)
		SLATE_ARGUMENT(const FSlateBrush*, UnselectedTabBrush)

		SLATE_ARGUMENT(float, DistanceBetweenNode)

		SLATE_ARGUMENT(FVector2D, IconSize)


		SLATE_ARGUMENT(FVector2D, TabSize)
		SLATE_ARGUMENT(const FTextBlockStyle*, TabTextStyle)

		SLATE_ARGUMENT(const FTextBlockStyle*, DescriptionTextStyle)
		SLATE_ARGUMENT(FVector2D, DescriptionLabelSize)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SBox> TabSizeBox;

	TSharedPtr<SBorder> KeyboardTab;
	TSharedPtr<SBorder> GamepadTab;

	TSharedPtr<SWidgetSwitcher> InputDeviceSwitcher;
	TSharedPtr<SScrollBox> KeyboardScrollBox;
	class SScrollPanel* KeyboardScrollPanel;
	
	TSharedPtr<SScrollBox> GamepadScrollBox;
	SScrollPanel* GamepadScrollPanel;

	TSharedPtr<SHorizontalBox> DescriptionBox;
	TArray<TSharedPtr<class STextBlock>> DescriptionTexts;
	TArray<TSharedPtr<class SInputKeyLabel>> DescriptionLabels;

private:
	FMargin NodePadding;
	float DistanceBetweenNode;

	const FSlateBrush* NodeTextBorderBrush;
	const FSlateBrush* NodeIconBorderBrush;
	const FSlateBrush* NodeFocusedFrameBrush;

	const FSlateBrush* SelectedTabBrush;
	const FSlateBrush* UnselectedTabBrush;

	const FTextBlockStyle* NodeNameBlockStyle;

	TObjectPtr<class UPlayerMappableInputConfig> TargetInputConfig;

	FVector2D IconSize;


public:
//Tab
	void SetTabSize(FVector2D InSize);
	void SetTabTextStyle(const FTextBlockStyle* InStyle);

	void SetSelectedTabBrush(const FSlateBrush* InBrush);
	void SetUnselectedTabBrush(const FSlateBrush* InBrush);

//Node
	void SetNodePadding(const FMargin& InNodePadding);
	void SetNodeTextBorderBrush(const FSlateBrush* InBrush);
	void SetNodeIconBorderBrush(const FSlateBrush* InBrush);
	void SetNodeFocusedFrameBrush(const FSlateBrush* InBrush);
	
	void SetDistanceBetweenNode(float InDistance);

	void SetNodeNameBlockStyle(const FTextBlockStyle* InStyle);

	void SetIconSize(FVector2D InSize);

//Description
private:
	void CreateDescriptionBox();

public:
	void SetDescriptionTextStyle(const FTextBlockStyle* InStyle);
	void SetDescriptionLabelSize(FVector2D InSize);

protected:
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override {return FReply::Handled(); }

	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	virtual bool SupportsKeyboardFocus() const { return true; }

private:
	void EnterCustomInputSetting(uint32 UserIndex);
	void ExitCustomInputSetting();

	FDelegateHandle OnChangedMappingHandle;
	void OnChangedMapping(const FName& InCode, const FKey& NewKey);

	FDelegateHandle OnResetMappingHandle;
	void OnResetMapping(UPlayerMappableInputConfig* InConfig);

public:
	void ShowCustomInputDevice(bool bIsGamepad);

private:
	void UpdateNodes();

public:
	void SetMappableKeys(class UPlayerMappableInputConfig* Target);
	void SetMappableKeys(TSoftObjectPtr<class UPlayerMappableInputConfig> Target);

private:
	void RebuildNavigation();
};