// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOn/LockOnTargetComponent.h"
#include "LockOn/LockOnSubsystem.h"

// Sets default values for this component's properties
ULockOnTargetComponent::ULockOnTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockOnTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ULockOnSubsystem* LockOnSS = GetWorld()->GetSubsystem<ULockOnSubsystem>();
	check(LockOnSS);

	LockOnSS->RegisterLockOnTarget(this);
}

void ULockOnTargetComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	UWorld* World = GetWorld();
	if(World == nullptr) return;

	ULockOnSubsystem* LockOnSS = World->GetSubsystem<ULockOnSubsystem>();
	
	if (LockOnSS)
	{
		LockOnSS->UnregisterLockOnTarget(this);
	}
	
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}


// Called every frame
void ULockOnTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

