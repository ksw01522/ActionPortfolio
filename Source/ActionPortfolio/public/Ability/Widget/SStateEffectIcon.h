// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

/**
 * 
 */
class ACTIONPORTFOLIO_API SStateEffectIcon : public SLeafWidget
{
	SLATE_DECLARE_WIDGET(SStateEffectIcon, SLeafWidget);

public:
	SLATE_BEGIN_ARGS(SStateEffectIcon) : _IconBrush(nullptr)
	{}

	SLATE_ARGUMENT(const FSlateBrush*, IconBrush)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);


private:
	const FSlateBrush* IconBrush;

	TOptional<FVector2D> IconSizeOverride;

public:
	void SetIconBrush(const FSlateBrush* InBrush) { IconBrush = InBrush; }


protected:
	virtual FVector2D ComputeDesiredSize(float) const override;

public:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

};
