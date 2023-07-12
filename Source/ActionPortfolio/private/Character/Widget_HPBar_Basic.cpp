// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Widget_HPBar_Basic.h"
#include "Components/ProgressBar.h"
#include "Character/ActionPortfolioCharacter.h"
#include "ActionPortfolio.h"

void UWidget_HPBar_Basic::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar->PercentDelegate.BindDynamic(this, &UWidget_HPBar_Basic::GetHPBarPercent);
	HPBar->SynchronizeProperties();
}

float UWidget_HPBar_Basic::GetHPBarPercent()
{
	if (RegistedCharacter.IsValid()) {
		float MaxHP = RegistedCharacter->GetMaxHealth();
		float CurrentHP = RegistedCharacter->GetHealth();
		if (MaxHP == 0) {
			return 0.0f;
		}

		return CurrentHP / MaxHP;
	}
	else {
		return 0.0f;
	}
}

void UWidget_HPBar_Basic::RegistCharacter(AActionPortfolioCharacter* NewCharacter)
{
	RegistedCharacter = NewCharacter;
}
