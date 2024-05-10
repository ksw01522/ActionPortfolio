// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DialogueAnimationActor.h"

// Sets default values
ADialogueAnimationActor::ADialogueAnimationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Skeleton = CreateDefaultSubobject<USkeletalMeshComponent>("AnimationSkeleton");
}

// Called when the game starts or when spawned
void ADialogueAnimationActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADialogueAnimationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADialogueAnimationActor::PlayAnimation(UAnimationAsset* Anim)
{
	Skeleton->PlayAnimation(Anim, false);
}

