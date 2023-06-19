// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ActionPortfolioInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EDialogueWidgetAnimSpeed : uint8
{
	SLOW,
	NORMAL,
	FAST
};

UCLASS(Config = ActionPortfolioSetting)
class ACTIONPORTFOLIO_API UActionPortfolioInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UActionPortfolioInstance();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionPF|Widget", meta = (AllowPrivateAccess = "true"))
	float SlowDialogueAnimTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionPF|Widget", meta = (AllowPrivateAccess = "true"))
	float NormalDialogueAnimTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionPF|Widget", meta = (AllowPrivateAccess = "true"))
	float FastDialogueAnimTime;


	UPROPERTY(GlobalConfig)
	EDialogueWidgetAnimSpeed DialogueAnimSpeed;

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	float GetDialogueAnimTime() const;

	void SetDialogueAnimSpeed(EDialogueWidgetAnimSpeed NewSpeed) {
		if(DialogueAnimSpeed == NewSpeed) return;
		DialogueAnimSpeed = NewSpeed;
		SaveConfig();
	}
};
