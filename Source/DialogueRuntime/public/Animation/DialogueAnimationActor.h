// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DialogueAnimationActor.generated.h"

UCLASS()
class DIALOGUERUNTIME_API ADialogueAnimationActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Skeleton;

public:	
	// Sets default values for this actor's properties
	ADialogueAnimationActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintGetter)
	USkeletalMeshComponent* GetAnimationSkeleton() const { return Skeleton; }

	void PlayAnimation(UAnimationAsset* Anim);
};
