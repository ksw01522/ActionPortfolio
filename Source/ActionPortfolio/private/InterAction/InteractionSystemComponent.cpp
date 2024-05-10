// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractionSystemComponent.h"
#include "Components/ShapeComponent.h"
#include "ActionPortfolio.h"

// Sets default values for this component's properties
UInteractionSystemComponent::UInteractionSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	bEnableInteract = true;
}


// Called when the game starts
void UInteractionSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	TArray<UActorComponent*> SensorComponents = GetOwner()->GetComponentsByTag(UShapeComponent::StaticClass(), FName("InteractionSensor"));

	for (auto& SensorComponent : SensorComponents)
	{
		UShapeComponent* SensorShape = Cast<UShapeComponent>(SensorComponent);
		if (SensorShape == nullptr)
		{
			PFLOG(Error, TEXT("Not Shape has InteractionsSensor Tag from : %s"), *GetOwner()->GetName());
			continue;
		}

		SensorShape->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
		SensorShape->UpdateOverlaps();
	}
}




// Called every frame
void UInteractionSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

bool UInteractionSystemComponent::CanInteract(AActor* InteractActor) const
{
	if(!IsValid(InteractActor)) return false;

	bool bIsCan = CanInteract_CPP(InteractActor);
	
	for (auto Condition : InteractionConditions)
	{
		if(!bIsCan) break;

		bIsCan = Condition->CanInteractCondition(InteractActor);
	}
	
	return bIsCan;
}

void UInteractionSystemComponent::Interact(AActor* InteractActor)
{
	if (!IsValid(InteractActor)) return;

	if(Del_Interact.IsBound()) {Del_Interact.Broadcast(InteractActor); }
	Interact_CPP(InteractActor);
}



bool UCanInteractionCondition::CanInteractCondition(AActor* InteractActor)
{
	return false;
}
