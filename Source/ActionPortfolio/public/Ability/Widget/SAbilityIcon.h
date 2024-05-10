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
				_Ability(nullptr)
	{}

	SLATE_ARGUMENT(class UActionPFGameplayAbility*, Ability)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);


protected:
	virtual FVector2D ComputeDesiredSize(float) const override;

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	const FSlateBrush* IconBrush;

	bool bCanAct;
	float RemainCoolTime;
	float CoolTimeDuration;

	TOptional<FVector2D> IconSizeOverride;

	TWeakObjectPtr<class UActionPFAbilitySystemComponent> AbilitySystem;
	TWeakObjectPtr<UActionPFGameplayAbility> Ability;

	static TSharedPtr<SAbilityIcon> EmptyAbilityIcon;

public:
	void SetIconSize(FVector2D InSize);

	virtual void LinkAbility(UActionPFGameplayAbility* InAbility);
	void LinkAbilitySystem(UActionPFAbilitySystemComponent* InSystem);


	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	static const TSharedPtr<SAbilityIcon>& GetEmptyIcon() {return EmptyAbilityIcon;}
};
