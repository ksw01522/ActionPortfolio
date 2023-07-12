// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPortfolioInstance.h"
#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "ActionPortfolio.h"
#include "DialogueManager.h"
#include "DialogueBFL.h"
#include "Kismet/KismetInternationalizationLibrary.h"

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

void UActionPortfolioInstance::SetCurrentLanguage(ELanguage NewLanguage)
{
	if(Language == NewLanguage) return;
	Language = NewLanguage;
	UDialogueManager* DialogueManager = UDialogueBFL::GetDialogueManager();

	switch (Language)
	{
	case ELanguage::Korean:
		DialogueManager->SetCurrentLanguage(EDialogueLanguage::Korean);
		UKismetInternationalizationLibrary::SetCurrentCulture("ko-KR");
		break;
	case ELanguage::English:
		DialogueManager->SetCurrentLanguage(EDialogueLanguage::English);
		UKismetInternationalizationLibrary::SetCurrentCulture("en");
		break;
	default:
		break;
	}

	SaveConfig();
}
