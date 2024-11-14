// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/ActionPortfolioInstance.h"
#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "ActionPortfolio.h"
#include "DialogueManager.h"
#include "DialogueBFL.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Items/ItemManagerSubsystem.h"
#include "CustomInputHelper.h"
#include "Settings/ActionPFGameUserSettings.h"

TObjectPtr<UActionPortfolioInstance> UActionPortfolioInstance::ActionPFInstance = nullptr;

UActionPortfolioInstance* UActionPortfolioInstance::GetActionPFInstance()
{
	return ActionPFInstance;
}

UActionPortfolioInstance::UActionPortfolioInstance()
{
	SlowDialogueAnimTime = 0.15f;
	NormalDialogueAnimTime = 0.1f;
	FastDialogueAnimTime = 0.05f;

	bBenchmarkForFirstRun = false;
}



void UActionPortfolioInstance::Init()
{
	ActionPFInstance = this;

	Super::Init();

	FActionPortfolioWidgetStyle::Initialize();

	if(!bBenchmarkForFirstRun) BenchmarkForFirstRun();
}

void UActionPortfolioInstance::Shutdown()
{
	FActionPortfolioWidgetStyle::Shutdown();

	ActionPFInstance = nullptr;

	Super::Shutdown();
}

float UActionPortfolioInstance::GetDialogueAnimTime() const
{
	return 0.2f;
}





void UActionPortfolioInstance::BenchmarkForFirstRun()
{
	UActionPFGameUserSettings* UserSetting = UActionPFGameUserSettings::Get();

	UserSetting->RunHardwareBenchmark();
	UserSetting->ApplyHardwareBenchmarkResults();

	bBenchmarkForFirstRun = true;
	SaveConfig();
}

