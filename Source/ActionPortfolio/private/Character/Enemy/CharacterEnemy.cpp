// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/CharacterEnemy.h"
#include "Components/ProgressBar.h"

void ACharacterEnemy::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (HPBarWidget)
	{
		HPBarWidget->SetPercent(GetHealthPercent());
	}
}

void ACharacterEnemy::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	if (HPBarWidget)
	{
		HPBarWidget->SetPercent(GetHealthPercent());
	}
}
