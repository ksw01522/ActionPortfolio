// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Settings/SGameSettingNode.h"
/**
 * 
 */
class ACTIONPORTFOLIO_API SGameSettingWindow : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SGameSettingWindow, SCompoundWidget);

public:
	DECLARE_DELEGATE_RetVal_TwoParams(FReply, FOnBackKeyDown, const FGeometry&, const FKeyEvent&);

public:
	SLATE_BEGIN_ARGS(SGameSettingWindow)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:
	TSharedPtr<class SScrollBox> ScrollBox;
	TArray<TSharedPtr<SGameSettingNode>> SettingNodes;

	const FTextBlockStyle* TextStyle = nullptr;
	const FSlateBrush* FocusedFrameBrush = nullptr;
	
	FMargin NameBlockPadding;

	const FSlateBrush* ArrowImageBrush = nullptr;

	const FSlateBrush* CheckedImageBrush = nullptr;
	const FSlateBrush* UncheckedImageBrush = nullptr;

	FOnBackKeyDown OnBackKeyDown;

	const FTextBlockStyle* DescTextStyle;
	FVector2D DescKeyLabelSize;

	TArray<TSharedPtr<class SInputKeyLabel>> DescKeyLabels;
	TArray<TSharedPtr<STextBlock>> DescTexts;

public:
	void SetTextStyle(const FTextBlockStyle* InTextStyle);
	void SetFocusedFrame(const FSlateBrush* InBrush);
	void SetNameblockPadding(FMargin InPadding);

	void SetOnBackKeyDownEvent(FOnBackKeyDown InEvent) { OnBackKeyDown = InEvent; }

	void SetDescTextStyle(const FTextBlockStyle* InStyle);
	void SetDescKeyLabelSize(FVector2D InSize);

protected:
	void RebuildNavigation();
	virtual TSharedRef<SWidget> CreateDescriptionWidget();

//multiple
public:
	void SetArrowImage(const FSlateBrush* InImage);
	
//Toggle
public:
	void SetCheckedImage(const FSlateBrush* InImage);
	void SetUncheckedImage(const FSlateBrush* InImage);

protected:
	void CreateMultipleNode(const FText& NameText, const TArray<FText>& OptionNames, int SelectedIdx, const SGameSettingNode_Multiple::FOnChangeOption& Event);
	void CreateToggleNode(const FText& NameText, bool CurrentState, const SGameSettingNode_Toggle::FOnChangeState& Event);

protected:
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

public:
	void OnSelectedSettingNodeType(ESettingNodeType NodeType);
};
