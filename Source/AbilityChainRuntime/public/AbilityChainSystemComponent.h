// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityChainSystemComponent.generated.h"

class UAbilityChainGraph;
class UAbilityChainNode;

UCLASS( ClassGroup=(AbilityChain), meta=(BlueprintSpawnableComponent) )
class ABILITYCHAINRUNTIME_API UAbilityChainSystemComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "AbilityChain")
	TSubclassOf<UAbilityChainGraph> ChainGraphClass;

	TWeakObjectPtr<const UAbilityChainGraph> ChainGraph;

	TWeakObjectPtr<const UAbilityChainNode> CurrentNode;

public:	
	// Sets default values for this component's properties
	UAbilityChainSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
