// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FocusToTarget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UBTTask_FocusToTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FocusToTarget();


protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector FocusBlackboardKey;

public:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
