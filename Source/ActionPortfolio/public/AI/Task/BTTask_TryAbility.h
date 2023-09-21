// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TryAbility.generated.h"

/**
 * 
 */


struct FBTTaskTryAbilityNode
{
	TWeakObjectPtr<class AActionPortfolioCharacter> SourceCharacter;
	TWeakObjectPtr<class UActionPFAbilitySystemComponent> AbilitySystem;
	TSubclassOf<class UActionPFGameplayAbility> TriedAbility;
};

UCLASS()
class ACTIONPORTFOLIO_API UBTTask_TryAbility : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_TryAbility();

private:
	UPROPERTY(EditAnywhere, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class UActionPFGameplayAbility>> Abilities;

	UPROPERTY()
	FString CachedDescription;

	UPROPERTY(EditAnywhere, Category = "Task", meta = (AllowPrivateAccess = "true"))
	bool bAbilityStopWhenTaskStop;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTTaskTryAbilityNode); };

protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	virtual FString GetStaticDescription() const override { return CachedDescription;};

#if WITH_EDITOR
	virtual void BuildDescription();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
#endif
};
