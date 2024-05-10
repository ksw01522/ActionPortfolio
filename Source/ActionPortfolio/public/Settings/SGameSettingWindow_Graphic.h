// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Settings/SGameSettingWindow.h"

/**
 * 
 */
class ACTIONPORTFOLIO_API SGameSettingWindow_Graphic : public SGameSettingWindow
{
	SLATE_DECLARE_WIDGET(SGameSettingWindow_Graphic, SGameSettingWindow);

public:
	SLATE_BEGIN_ARGS(SGameSettingWindow_Graphic) :
			_TextStyle(nullptr),
			_FocusedFrameBrush(nullptr),
			_NameBlockPadding(FMargin(5)),
			_ArrowImageBrush(nullptr),
			_CheckedImageBrush(nullptr),
			_UncheckedImageBrush(nullptr),
			_DescTextStyle(nullptr)
	{}

	SLATE_ARGUMENT(const FTextBlockStyle*, TextStyle)
	SLATE_ARGUMENT(const FSlateBrush*, FocusedFrameBrush)
	SLATE_ARGUMENT(FMargin, NameBlockPadding)

	SLATE_ARGUMENT(const FSlateBrush*, ArrowImageBrush)

	SLATE_ARGUMENT(const FSlateBrush*, CheckedImageBrush)
	SLATE_ARGUMENT(const FSlateBrush*, UncheckedImageBrush)

	SLATE_ARGUMENT(const FTextBlockStyle*, DescTextStyle)
	SLATE_ARGUMENT(FVector2D, DescLabelSize)

	SLATE_EVENT(FOnBackKeyDown, OnBackKeyDown)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
