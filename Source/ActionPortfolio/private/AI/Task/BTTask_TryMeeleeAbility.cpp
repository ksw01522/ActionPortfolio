// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_TryMeeleeAbility.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Character/EnemyAIController.h"
#include "Character/Player/ActionPFPlayerController.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/Ability/GameplayAbility_Meelee.h"

UBTTask_TryMeeleeAbility::UBTTask_TryMeeleeAbility()
{
	NodeName = "Try Meelee Ability";
	bNotifyTick = true;
	bStopWhenTaskStop = true;
}

EBTNodeResult::Type UBTTask_TryMeeleeAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	FBTTaskTryAbilityNode* MyMemory = CastInstanceNodeMemory<FBTTaskTryAbilityNode>(NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController == nullptr) return NodeResult;

	MyMemory->SourceCharacter = AIController->GetPawn<AActionPortfolioCharacter>();
	if (!MyMemory->SourceCharacter.IsValid()) return NodeResult;

	if (MyMemory->SourceCharacter->GetAbilitySystemComponent()->TryActivateAbilityByClass(MeeleeAbility)) NodeResult = EBTNodeResult::InProgress;

	return NodeResult;
}

void UBTTask_TryMeeleeAbility::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTTaskTryAbilityNode* MyMemory = CastInstanceNodeMemory<FBTTaskTryAbilityNode>(NodeMemory);
	if (!(MyMemory->SourceCharacter.IsValid()))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UActionPFAbilitySystemComponent* AbilitySystem = Cast<UActionPFAbilitySystemComponent>(MyMemory->SourceCharacter->GetAbilitySystemComponent());
	if (!AbilitySystem->IsActingAbilityByClass(MeeleeAbility))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

}

uint16 UBTTask_TryMeeleeAbility::GetInstanceMemorySize() const
{
	return sizeof(FBTTaskTryAbilityNode);
}

EBTNodeResult::Type UBTTask_TryMeeleeAbility::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTTaskTryAbilityNode* MyMemory = CastInstanceNodeMemory<FBTTaskTryAbilityNode>(NodeMemory);

	if (bStopWhenTaskStop && MyMemory->SourceCharacter.IsValid())
	{
		UActionPFAbilitySystemComponent* AbilitySystem = Cast<UActionPFAbilitySystemComponent>(MyMemory->SourceCharacter->GetAbilitySystemComponent());
		if (AbilitySystem->IsActingAbilityByClass(MeeleeAbility))
		{
			MyMemory->SourceCharacter->GetAbilitySystemComponent()->CancelAbility(MeeleeAbility.GetDefaultObject());
		}
	}


	return EBTNodeResult::Aborted;
}

FString UBTTask_TryMeeleeAbility::GetStaticDescription() const
{
	return CachedDescription;
}

#if WITH_EDITOR

void UBTTask_TryMeeleeAbility::BuildDescription()
{
	if (MeeleeAbility.Get())
	{
		CachedDescription = FString::Printf(TEXT("Try Meelee Ability : %s"), *MeeleeAbility->GetFName().ToString());
	}
	else
	{
		CachedDescription = FString::Printf(TEXT("Try Meelee Ability : None"));
	}
}

void UBTTask_TryMeeleeAbility::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	BuildDescription();
}

void UBTTask_TryMeeleeAbility::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	BuildDescription();
}

#endif