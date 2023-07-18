// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossCounterAttack.generated.h"

/**
 * 
 */
struct FBTTaskBossCounterAttackNode
{
	TWeakObjectPtr<class ACharacterBoss> SourceBoss;
	TWeakObjectPtr<class ABossAIController> BossController;
};

UCLASS(meta = (DisplayName = "Boss Counter Attack"))
class ACTIONPORTFOLIO_API UBTTask_BossCounterAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_BossCounterAttack();

private:
	UPROPERTY(EditAnywhere, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayAbility_Meelee> BossCounterAbility;

	UPROPERTY(EditAnywhere, Category = "Target", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector TargetSelector;

	virtual uint16 GetInstanceMemorySize() const override;



protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;


#if WITH_EDITOR
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
#endif

};
