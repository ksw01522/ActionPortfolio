// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_FocusToTarget.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "ActionPortfolio.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FocusToTarget::UBTTask_FocusToTarget()
{
	FocusBlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FocusToTarget, FocusBlackboardKey), AActor::StaticClass());
	FocusBlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FocusToTarget, FocusBlackboardKey));

}

void UBTTask_FocusToTarget::InitializeFromAsset(UBehaviorTree& Asset)
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

EBTNodeResult::Type UBTTask_FocusToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FVector TargetLocation;

	if (FocusBlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FocusBlackboardKey.SelectedKeyName);
	}
	else if (FocusBlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		if (AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FocusBlackboardKey.SelectedKeyName))) {
			TargetLocation = TargetActor->GetActorLocation();
		}
		else {
			PFLOG(Error, TEXT("Can't find Target Actor."));
			return EBTNodeResult::Failed;
		}
	}
	else {
		PFLOG(Error, TEXT("Can't find Correct Key Type."));
		return EBTNodeResult::Failed;
	}

	AActor* Instigator = OwnerComp.GetOwner();
	FVector Dir = TargetLocation - Instigator->GetActorLocation();
	Instigator->SetActorRotation(Dir.Rotation());

	return EBTNodeResult::Succeeded;
}
