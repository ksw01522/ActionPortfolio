// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
enum class ESettingNodeType : uint8
{
	None = 0,
	Multiple,
	Toggle
};

class ACTIONPORTFOLIO_API SGameSettingNode : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SGameSettingNode, SCompoundWidget)

public:
	SGameSettingNode();

	SLATE_BEGIN_ARGS(SGameSettingNode)
	{}


	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:
	TSharedPtr<class STextBlock> NameTextBlock;
	SHorizontalBox::FSlot* NameBlockSlot;

	TSharedPtr<SImage> FocusedFrame;

	class SGameSettingWindow* OwnerWindow;

	ESettingNodeType SettingType;

public:
	virtual void SetTextStyle(const FTextBlockStyle* InTextStyle);
	virtual void SetFocusedFrame(const FSlateBrush* InBrush);
	virtual void SetNameBlockPadding(FMargin InPadding);
	virtual void SetArrowImage(const FSlateBrush* InImage) {}
	virtual void SetCheckedImage(const FSlateBrush* InImage) {}
	virtual void SetUncheckedImage(const FSlateBrush* InImage) {}

	ESettingNodeType GetSettingType() const { return SettingType; }

protected:
	virtual bool SupportsKeyboardFocus() const override { return true; }

	virtual TOptional<bool> OnQueryShowFocus(const EFocusCause InFocusCause) const override { return false; }

	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override{return FReply::Handled();}
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override{return FReply::Handled();}
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override{return FReply::Handled();}
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override{return FReply::Handled();}
	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override{return FReply::Handled();}
	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override{return FReply::Handled();}
	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override{return FReply::Handled();}
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override{return FReply::Handled();}

};


class ACTIONPORTFOLIO_API SGameSettingNode_Multiple : public SGameSettingNode
{
	SLATE_DECLARE_WIDGET(SGameSettingNode_Multiple, SGameSettingNode)
	
public:
	DECLARE_DELEGATE_OneParam(FOnChangeOption, int);

	SGameSettingNode_Multiple();
	virtual ~SGameSettingNode_Multiple();

	SLATE_BEGIN_ARGS(SGameSettingNode_Multiple) :
		_OwnerWindow(nullptr),
		_TextStyle(nullptr),
		_NameText(FText::GetEmpty()),
		_SelectedIndex(0),
		_ArrowImage(nullptr),
		_FocusedFrame(nullptr)
	{}

	SLATE_ARGUMENT(SGameSettingWindow*, OwnerWindow)

	SLATE_ARGUMENT(const FTextBlockStyle*, TextStyle)
	SLATE_ARGUMENT(FMargin, NameBlockPadding)
	SLATE_ARGUMENT(FText, NameText)

	SLATE_ARGUMENT(TArray<FText>, OptionNames)
	SLATE_ARGUMENT(int, SelectedIndex)
	SLATE_EVENT(FOnChangeOption, OnChangeOption)

	SLATE_ARGUMENT(const FSlateBrush*, ArrowImage)
	SLATE_ARGUMENT(const FSlateBrush*, FocusedFrame)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<STextBlock> OptionTextBlock;
	TSharedPtr<SImage> LeftArrowImage;
	TSharedPtr<SImage> RightArrowImage;

public:
	virtual void SetTextStyle(const FTextBlockStyle* InTextStyle) override;

//Change Option
private:
	FOnChangeOption OnChangeOption;

	int SelectedOptionIndex;
	TArray<FText> DisplayOptionNames;

	void SelectOption(int Index);

public:
	void SetOnChangeOptionEvent(const FOnChangeOption& InEvent);

//Arrow
private:
	FReply OnClickLeftArrow(const FGeometry& Geometry, const FPointerEvent& PointerEvent);
	FReply OnClickRightArrow(const FGeometry& Geometry, const FPointerEvent& PointerEvent);


public:
	virtual void SetArrowImage(const FSlateBrush* InImage) override;

//SWidget Interface
protected:
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
};

class ACTIONPORTFOLIO_API SGameSettingNode_Toggle : public SGameSettingNode
{
	SLATE_DECLARE_WIDGET(SGameSettingNode_Toggle, SGameSettingNode)

public:
	DECLARE_DELEGATE_OneParam(FOnChangeState, bool);

	SGameSettingNode_Toggle();
	virtual ~SGameSettingNode_Toggle();

	SLATE_BEGIN_ARGS(SGameSettingNode_Toggle) :
		_OwnerWindow(nullptr),
		_TextStyle(nullptr),
		_NameText(FText::GetEmpty()),
		_CurrentState(false),
		_CheckedImage(nullptr),
		_UncheckedImage(nullptr),
		_FocusedFrame(nullptr)
		{}

		SLATE_ARGUMENT(SGameSettingWindow*, OwnerWindow)

		SLATE_ARGUMENT(const FTextBlockStyle*, TextStyle)
		SLATE_ARGUMENT(FMargin, NameBlockPadding)
		SLATE_ARGUMENT(FText, NameText)

		SLATE_ARGUMENT(bool, CurrentState)
		SLATE_EVENT(FOnChangeState, OnChangeState)

		SLATE_ARGUMENT(const FSlateBrush*, CheckedImage)
		SLATE_ARGUMENT(const FSlateBrush*, UncheckedImage)

		SLATE_ARGUMENT(const FSlateBrush*, FocusedFrame)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<class SImage> CheckImage;

private:
	const FSlateBrush* CheckedImage;
	const FSlateBrush* UncheckedImage;

	FOnChangeState OnChangeState;

	bool CurrentState;

	void SetState(bool InState);

public:
	void SetOnChangeStateEvent(const FOnChangeState& InEvent);
public:
	virtual void SetCheckedImage(const FSlateBrush* InImage) override;
	virtual void SetUncheckedImage(const FSlateBrush* InImage) override;

protected:

	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
};