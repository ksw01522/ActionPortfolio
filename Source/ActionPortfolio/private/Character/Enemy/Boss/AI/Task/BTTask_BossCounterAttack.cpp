// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Boss/AI/Task/BTTask_BossCounterAttack.h"
#include "Character/Enemy/Boss/Ability/Ability_BossRush.h"
#include "Character/Enemy/Boss/BossAIController.h"
#include "Character/Enemy/Boss/CharacterBoss.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ActionPortfolio.h"

UBTTask_BossCounterAttack::UBTTask_BossCounterAttack()
{
	NodeName = "Act Boss Counter Attack";

	TargetSelector.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_BossCounterAttack, TargetSelector));
	TargetSelector.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_BossCounterAttack, TargetSelector), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_BossCounterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	FBTTaskBossCounterAttackNode* MyMemory = CastInstanceNodeMemory<FBTTaskBossCounterAttackNode>(NodeMemory);
	ABossAIController* AIController = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AIController == nullptr) return NodeResult;
	if(!AIController->GetBlackboardComponent()->GetValueAsBool(ABossAIController::IsDamagedKey)) return NodeResult;

	MyMemory->SourceBoss = AIController->GetPawn<ACharacterBoss>();
	if (!MyMemory->SourceBoss.IsValid()) return NodeResult;

	FVector FocusVector;
	if (TargetSelector.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {
		FocusVector = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetSelector.SelectedKeyName);
	}
	else if (TargetSelector.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetSelector.SelectedKeyName));
		if (IsValid(TargetActor)) {
			FocusVector = TargetActor->GetActorLocation();
		}
		else {
			PFLOG(Error, TEXT("Can't find Target Actor."));
		}
	}
	else
	{
		PFLOG(Error, TEXT("Can't Find Target."));
	}

	FVector FocusDir = FocusVector - MyMemory->SourceBoss->GetActorLocation();
	MyMemory->SourceBoss->SetActorRotation(FocusDir.Rotation());
	if (MyMemory->SourceBoss->GetAbilitySystemComponent()->TryActivateAbilityByClass(BossCounterAbility)) NodeResult = EBTNodeResult::InProgress;

	return NodeResult;
}

void UBTTask_BossCounterAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTTaskBossCounterAttackNode* MyMemory = CastInstanceNodeMemory<FBTTaskBossCounterAttackNode>(NodeMemory);
	if (!(MyMemory->SourceBoss.IsValid()))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UActionPFAbilitySystemComponent* AbilitySystem = Cast<UActionPFAbilitySystemComponent>(MyMemory->SourceBoss->GetAbilitySystemComponent());
	if (!AbilitySystem->IsActingAbilityByClass(BossCounterAbility))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

}

EBTNodeResult::Type UBTTask_BossCounterAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTTaskBossCounterAttackNode* MyMemory = CastInstanceNodeMemory<FBTTaskBossCounterAttackNode>(NodeMemory);

	if (MyMemory->SourceBoss.IsValid())
	{
		UActionPFAbilitySystemComponent* AbilitySystem = Cast<UActionPFAbilitySystemComponent>(MyMemory->SourceBoss->GetAbilitySystemComponent());
		if (AbilitySystem->IsActingAbilityByClass(BossCounterAbility))
		{
			MyMemory->SourceBoss->GetAbilitySystemComponent()->CancelAbility(BossCounterAbility.GetDefaultObject());
		}
	}


	return EBTNodeResult::Aborted;
}

void UBTTask_BossCounterAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	FBTTaskBossCounterAttackNode* MyMemory = CastInstanceNodeMemory<FBTTaskBossCounterAttackNode>(NodeMemory);

	if (TaskResult != EBTNodeResult::Failed) {
		MyMemory->BossController->SetIsDamaged(false);
	}
}

void UBTTask_BossCounterAttack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		TargetSelector.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}

}



uint16 UBTTask_BossCounterAttack::GetInstanceMemorySize() const
{
	return sizeof(FBTTaskBossCounterAttackNode);
}
