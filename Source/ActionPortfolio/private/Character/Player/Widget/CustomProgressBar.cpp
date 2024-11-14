// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Widget/CustomProgressBar.h"

void UCustomProgressBar::UpdateCustomProgressBar()
{
	if (MaxValue <= 0)
	{
		SetPercent(0);
	}
	else if (MaxValue <= CurrentValue)
	{
		SetPercent(1);
	}
	else
	{
		SetPercent(CurrentValue / MaxValue);
	}
}

void UCustomProgressBar::SetMaxValue(float InValue)
{
	if(MaxValue == InValue) return;

	float NewValue = InValue <= 0 ? 0 : InValue;

	MaxValue = NewValue;

	UpdateCustomProgressBar();
}

void UCustomProgressBar::SetCurrentValue(float InValue)
{
	if (CurrentValue == InValue) return;

	float NewValue = InValue <= 0 ? 0 : InValue;

	CurrentValue = NewValue;

	UpdateCustomProgressBar();
}
