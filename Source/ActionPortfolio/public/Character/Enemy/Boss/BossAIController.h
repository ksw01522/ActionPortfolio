// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EnemyAIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API ABossAIController : public AEnemyAIController
{
	GENERATED_BODY()
	
	ABossAIController();

public:
	static const FName IsDamagedKey;


protected:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void SetIsDamaged(bool NewState);
	bool IsDamaged() const;

	UFUNCTION(BlueprintCallable, Category = "Boss")
	void FocusPlayer();
};
