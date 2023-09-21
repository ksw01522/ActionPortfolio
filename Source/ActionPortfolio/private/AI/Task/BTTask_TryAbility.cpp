// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_TryAbility.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Character/EnemyAIController.h"
#include "Character/Player/ActionPFPlayerController.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "ActionPortfolio.h"

UBTTask_TryAbility::UBTTask_TryAbility()
{
	NodeName = "Try Meelee Ability";
	bNotifyTick = true;
	bAbilityStopWhenTaskStop = true;
}

EBTNodeResult::Type UBTTask_TryAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	if(Abilities.IsEmpty()) return NodeResult;

	FBTTaskTryAbilityNode* MyMemory = CastInstanceNodeMemory<FBTTaskTryAbilityNode>(NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController == nullptr) return NodeResult;

	MyMemory->SourceCharacter = AIController->GetPawn<AActionPortfolioCharacter>();
	if (!MyMemory->SourceCharacter.IsValid()) return NodeResult;

	MyMemory->AbilitySystem = Cast<UActionPFAbilitySystemComponent>(MyMemory->SourceCharacter->GetAbilitySystemComponent());
	if(!MyMemory->AbilitySystem.IsValid()) return NodeResult;

	int32 RandomIDX = FMath::RandRange(0, Abilities.Num() - 1);
	MyMemory->TriedAbility = Abilities[RandomIDX];
	if (MyMemory->TriedAbility.GetDefaultObject() == nullptr) {
		PFLOG(Warning, TEXT("Try Ability is nullptr from %s."), *MyMemory->SourceCharacter->GetFName().ToString());
		return NodeResult;
	}
	
	if (MyMemory->AbilitySystem->TryActivateAbilityByClass(MyMemory->TriedAbility)) NodeResult = EBTNodeResult::InProgress;

	return NodeResult;
}

void UBTTask_TryAbility::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTTaskTryAbilityNode* MyMemory = CastInstanceNodeMemory<FBTTaskTryAbilityNode>(NodeMemory);
	if (!(MyMemory->SourceCharacter.IsValid()))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (!MyMemory->AbilitySystem->IsActingAbilityByClass(MyMemory->TriedAbility))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}

EBTNodeResult::Type UBTTask_TryAbility::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTTaskTryAbilityNode* MyMemory = CastInstanceNodeMemory<FBTTaskTryAbilityNode>(NodeMemory);

	if (bAbilityStopWhenTaskStop && MyMemory->SourceCharacter.IsValid())
	{
		UActionPFAbilitySystemComponent* AbilitySystem = Cast<UActionPFAbilitySystemComponent>(MyMemory->SourceCharacter->GetAbilitySystemComponent());
		if (AbilitySystem->IsActingAbilityByClass(MyMemory->TriedAbility))
		{
			MyMemory->SourceCharacter->GetAbilitySystemComponent()->CancelAbility(MyMemory->TriedAbility.GetDefaultObject());
		}
	}


	return EBTNodeResult::Aborted;
}

#if WITH_EDITOR
void UBTTask_TryAbility::BuildDescription()
{
	if (Abilities.IsEmpty())
	{
		CachedDescription = FString::Printf(TEXT("Can't Find Abilities"));
	}
	else
	{
		FString TempDesc;
		for (auto Ability : Abilities)
		{
			TempDesc.Append(TEXT("\n"));
			if (Ability.GetDefaultObject() != nullptr){
				TempDesc.Append(Ability.GetDefaultObject()->GetFName().ToString());
			}
			else {
				TempDesc.Append(TEXT("Can't Find Ability"));
			}
		}

		CachedDescription = FString::Printf(TEXT("Try Radom Ability From : %s"), *TempDesc);
	}

}

void UBTTask_TryAbility::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	BuildDescription();
}

void UBTTask_TryAbility::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	BuildDescription();
}


#endif