// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPortfolioAssetManager.h"
#include "AbilitySystemGlobals.h"

void UActionPortfolioAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
