// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

/**
 * 
 */
class ACTIONPORTFOLIO_API SAbilityIcon : public SLeafWidget
{
	SLATE_DECLARE_WIDGET(SAbilityIcon, SLeafWidget)

public:
	SAbilityIcon();

	SLATE_BEGIN_ARGS(SAbilityIcon) :
				_IconBrush(nullptr)
	{}

	SLATE_ARGUMENT(const FSlateBrush*, IconBrush)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);


protected:
	virtual FVector2D ComputeDesiredSize(float) const override;

private:
	const FSlateBrush* IconBrush;

	bool bCanAct;
	float RemainCoolTime;
	float CoolTimeDuration;

	TOptional<FVector2D> IconSizeOverride;

public:
	void SetIconSize(FVector2D InSize);


	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	void SetIconBrush(const FSlateBrush* InBrush) { IconBrush = InBrush; }
	void SetCanAbilityAct(bool NewState) {bCanAct = NewState;};
	void SetRemainCoolTime(float NewTime) { RemainCoolTime = NewTime; };
	void SetCoolTimeDuration(float NewTime) { CoolTimeDuration = NewTime; };
};
