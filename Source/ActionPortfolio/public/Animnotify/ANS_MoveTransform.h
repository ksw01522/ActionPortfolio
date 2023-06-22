// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_MoveTransform.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMoveTransformPolicy : uint8
{
	Relative,
	TargetFromBlackboard
};

UCLASS()
class ACTIONPORTFOLIO_API UANS_MoveTransform : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	EMoveTransformPolicy TransformPolicy;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true", EditConditionHides, EditCondition = "TransformPolicy == EMoveTransformPolicy::Relative"))
	FVector RelativeTargetLocation;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true", EditConditionHides, EditCondition = "TransformPolicy == EMoveTransformPolicy::Relative"))
	FRotator RelativeTargetRotation;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true", EditConditionHides, EditCondition = "TransformPolicy == EMoveTransformPolicy::TargetFromBlackboard"))
	FName BBTargetName;

	// If MoveDistance <= 0, then Move To TargetTransform
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true", EditConditionHides, EditCondition = "TransformPolicy == EMoveTransformPolicy::TargetFromBlackboard"))
	float MoveDistance;

	float ActingTime;

	float TotalAnimDuration;
	TWeakObjectPtr<AActor> MeshOwnerActor;
	TWeakObjectPtr<class UBlackboardComponent> Blackboard;

	FVector OriginLocation;
	FVector GoalLocation;
	bool bCanUseThis;

public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	//virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
};
