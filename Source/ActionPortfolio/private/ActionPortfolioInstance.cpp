// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPortfolioInstance.h"
#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "ActionPortfolio.h"
#include "DialogueManager.h"
#include "DialogueBFL.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Settings/GameSettingSubsystem.h"
#include "Engine/DataTable.h"
#include "Items/ItemManagerSubsystem.h"
#include "CustomInputHelper.h"


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
}



void UActionPortfolioInstance::Init()
{
	ActionPFInstance = this;

	Super::Init();

	FActionPortfolioWidgetStyle::Initialize();

	PFLOG(Warning, TEXT("Game Instance Init."));
}

void UActionPortfolioInstance::Shutdown()
{
	FActionPortfolioWidgetStyle::Shutdown();

	ActionPFInstance = nullptr;

	Super::Shutdown();
}

float UActionPortfolioInstance::GetDialogueAnimTime() const
{
	switch (GetSubsystem<UGameSettingSubsystem>(this)->CurrentDialogueAnimSpeed)
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

ELanguage UActionPortfolioInstance::GetCurrentLanguage() const
{
	return GetSubsystem<UGameSettingSubsystem>(this)->CurrentLanguage;
}

TArray<TSoftObjectPtr<UDataTable>> UActionPortfolioInstance::GetItemDataTables() const
{
	TArray<TSoftObjectPtr<UDataTable>> ReturnTables;
	ReturnTables.Reserve(3);
	ReturnTables.Add(ItemDataTable_Equipment);
	ReturnTables.Add(ItemDataTable_Consumption);
	ReturnTables.Add(ItemDataTable_Material);

	return MoveTemp(ReturnTables);
}

