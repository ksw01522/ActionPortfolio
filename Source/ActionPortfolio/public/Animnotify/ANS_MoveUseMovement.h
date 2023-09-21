// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_MoveUseMovement.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UANS_MoveUseMovement : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	FVector MoveDirection;

	FVector FinalDirection;


public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

};
