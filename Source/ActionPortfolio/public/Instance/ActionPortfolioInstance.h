// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Settings/CustomStructForSetting.h"
#include "ActionPortfolioInstance.generated.h"

/**
 * 
 */
 class UDataTable;


UCLASS(Config = Game)
class ACTIONPORTFOLIO_API UActionPortfolioInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UActionPortfolioInstance();
private:
	static TObjectPtr<UActionPortfolioInstance> ActionPFInstance;
public:
	static UActionPortfolioInstance* GetActionPFInstance();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionPF|Widget", meta = (AllowPrivateAccess = "true"))
	float SlowDialogueAnimTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionPF|Widget", meta = (AllowPrivateAccess = "true"))
	float NormalDialogueAnimTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionPF|Widget", meta = (AllowPrivateAccess = "true"))
	float FastDialogueAnimTime;


public:
	virtual void Init() override;
	virtual void Shutdown() override;

	float GetDialogueAnimTime() const;




private:
	UPROPERTY(GlobalConfig)
	bool bBenchmarkForFirstRun;

	void BenchmarkForFirstRun();
};
