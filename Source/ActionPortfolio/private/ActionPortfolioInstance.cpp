// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPortfolioInstance.h"
#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "ActionPortfolio.h"

UActionPortfolioInstance::UActionPortfolioInstance()
{
	SlowDialogueAnimTime = 0.15f;
	NormalDialogueAnimTime = 0.1f;
	FastDialogueAnimTime = 0.5f;
}

void UActionPortfolioInstance::Init()
{
	Super::Init();

	FActionPortfolioWidgetStyle::Initialize();
}

void UActionPortfolioInstance::Shutdown()
{
	Super::Shutdown();

	FActionPortfolioWidgetStyle::Shutdown();
}

float UActionPortfolioInstance::GetDialogueAnimTime() const
{
	switch (DialogueAnimSpeed)
	{
		case EDialogueWidgetAnimSpeed::SLOW:
		return SlowDialogueAnimTime;
		
		case EDialogueWidgetAnimSpeed::NORMAL:
		return NormalDialogueAnimTime;

		case EDialogueWidgetAnimSpeed::FAST:
		return FastDialogueAnimTime;
	}

	return 0.2f;
}
