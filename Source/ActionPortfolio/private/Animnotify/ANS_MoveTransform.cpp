// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/ANS_MoveTransform.h"
#include "Character/EnemyAIController.h"
#include "Character/ActionPortfolioCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void UANS_MoveTransform::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	bCanUseThis = true;
	ActingTime = 0;
	this->TotalAnimDuration = TotalDuration;
	MeshOwnerActor = MeshComp->GetOwner();
	if (!MeshOwnerActor.IsValid()) {
		bCanUseThis = false;
		return;
	}
	FRotator GoalRotation;

	FVector ActorLocation = MeshOwnerActor->GetActorLocation();
	FRotator ActorRotation = MeshOwnerActor->GetActorRotation();

	OriginLocation = ActorLocation;

	switch (TransformPolicy)
	{
	case EMoveTransformPolicy::Relative:
		GoalLocation = ActorLocation + RelativeTargetLocation;
		GoalRotation = ActorRotation + RelativeTargetRotation;
		MeshOwnerActor->SetActorRotation(GoalRotation);
		break;

	case EMoveTransformPolicy::TargetFromBlackboard:
		if (APawn* OwnerPawn = Cast<APawn>(MeshOwnerActor.Get()))
		{
			if (AAIController* AIController = OwnerPawn->GetController<AAIController>())
			{
				Blackboard = AIController->GetBlackboardComponent();
			}
		}

		if (Blackboard.IsValid()){
			AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(BBTargetName));
			if (!IsValid(TargetActor)){
				bCanUseThis = false;
				return;
			}
			

			GoalLocation = TargetActor->GetActorLocation();
			GoalLocation.Z = ActorLocation.Z;

			FVector DeltaLocation = GoalLocation - ActorLocation;
			if (0 < MoveDistance)
			{
				
				if (MoveDistance * MoveDistance < DeltaLocation.SquaredLength())
				{
					FVector TempVector;
					TempVector = DeltaLocation.GetSafeNormal();

					GoalLocation = TempVector * MoveDistance;
				}
			}

			GoalRotation = DeltaLocation.Rotation();
			MeshOwnerActor->SetActorRotation(GoalRotation);
		}
		else {
			bCanUseThis = false;
		}
		break;
	}
}

void UANS_MoveTransform::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if(!MeshOwnerActor.IsValid() || !bCanUseThis) return;

	ActingTime += FrameDeltaTime;

	FVector TempLocation = FMath::Lerp<FVector>(MeshOwnerActor->GetActorLocation(), GoalLocation, ActingTime / TotalAnimDuration);

	MeshOwnerActor->TeleportTo(TempLocation, MeshOwnerActor->GetActorRotation());
}
