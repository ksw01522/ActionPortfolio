// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();


	//////////////////////// Team /////////////////////
public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionPF|AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BT_AI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionPF|AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBlackboardData> BB_AI;

	TArray<TWeakObjectPtr<AActor>> DetectedHostileTargets;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AIPerception)
	TObjectPtr<class UAIPerceptionComponent> AIPerception;

	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Sight> Sight_Config;


	static const FName FocusedHostileTargetKey;
	

protected:
	virtual void BeginPlay() override;

	virtual void ConstructPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus const stimulus);

	virtual void OnHostileTargetDetected(AActor* NewTarget);

	void RemoveDetectedHostileTarget(AActor* Target);

	void FocusOtherTarget();

private:
	void RemoveNotValidHostileTargets();

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetFocusTargetForced(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActionPortfolioCharacter* GetCurrentFocusedTarget() const;
};
