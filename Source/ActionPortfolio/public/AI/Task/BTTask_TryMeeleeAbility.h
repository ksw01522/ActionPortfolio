// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TryMeeleeAbility.generated.h"

/**
 * 
 */
struct FBTTaskTryAbilityNode
{
	TWeakObjectPtr<class AActionPortfolioCharacter> SourceCharacter;
};

UCLASS()
class ACTIONPORTFOLIO_API UBTTask_TryMeeleeAbility : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_TryMeeleeAbility();

private:
	UPROPERTY()
	FString CachedDescription;

	UPROPERTY(EditAnywhere, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayAbility_Meelee> MeeleeAbility;

	UPROPERTY(EditAnywhere, Category = "Task", meta = (AllowPrivateAccess = "true"))
	bool bStopWhenTaskStop;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;



protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual void BuildDescription();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
#endif
};
