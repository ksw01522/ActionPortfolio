// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_MoveWithFocusOther.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Character/ActionPortfolioCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "ActionPortfolio.h"

UBTTask_MoveWithFocusOther::UBTTask_MoveWithFocusOther()
{
	NodeName = "Move With Focus Other.";

	bNotifyTick = true;

	FocusBlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveWithFocusOther, FocusBlackboardKey), AActor::StaticClass());
	FocusBlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveWithFocusOther, FocusBlackboardKey));
}

void UBTTask_MoveWithFocusOther::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		FocusBlackboardKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}

}

EBTNodeResult::Type UBTTask_MoveWithFocusOther::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (NodeResult == EBTNodeResult::InProgress) {
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

		if(FocusBlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
		{
		}
		else if (FocusBlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {
		}

		OwnerComp.GetAIOwner()->GetPawn<AActionPortfolioCharacter>()->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	return NodeResult;
}

void UBTTask_MoveWithFocusOther::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	OwnerComp.GetAIOwner()->GetPawn<AActionPortfolioCharacter>()->ResetMovement();

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_MoveWithFocusOther::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AActionPortfolioCharacter* Instigator = OwnerComp.GetAIOwner()->GetPawn<AActionPortfolioCharacter>();
	if (Instigator == nullptr) return;

	if (FocusBlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FocusBlackboardKey.SelectedKeyName));

		if (TargetActor)
		{
			FVector Temp = TargetActor->GetActorLocation() - Instigator->GetActorLocation();
			Temp.Z = 0;
			Temp.Normalize();
			Instigator->SetActorRotation(Temp.Rotation());
		}
	}
	else if (FocusBlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		FVector Temp = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FocusBlackboardKey.SelectedKeyName) - Instigator->GetActorLocation();
		Temp.Z = 0;
		Temp.Normalize();
		Instigator->SetActorRotation(Temp.Rotation());
	}

}
