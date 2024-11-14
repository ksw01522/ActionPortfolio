// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityChainSystemComponent.h"
#include "AbilityChainGraph.h"
#include "AbilityChainRuntime.h"
#include "AbilityChainNode.h"

// Sets default values for this component's properties
UAbilityChainSystemComponent::UAbilityChainSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAbilityChainSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	ChainGraph = ChainGraphClass.GetDefaultObject();
	if (ChainGraph.IsValid())
	{
		CurrentNode = ChainGraph->GetStartNode();
	}
}


// Called every frame
void UAbilityChainSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

