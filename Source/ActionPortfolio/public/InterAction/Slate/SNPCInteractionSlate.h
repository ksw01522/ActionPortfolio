// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class ACTIONPORTFOLIO_API SNPCInteractionSlate : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SNPCInteractionSlate, SCompoundWidget)

public:
	SLATE_BEGIN_ARGS(SNPCInteractionSlate)
	{}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual ~SNPCInteractionSlate();

protected:
	virtual bool SupportsKeyboardFocus() const override { return true; }

	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;

	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

private:
	TSharedPtr<class SVerticalBox> BTNBox;

public:
	void AddButton(const TSharedRef<SButton>& InButton);
	void ClearButtons();
};
