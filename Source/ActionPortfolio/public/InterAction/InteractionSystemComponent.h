// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionSystemComponent.generated.h"


UCLASS( ClassGroup=(InteractSystem), Abstract)
class ACTIONPORTFOLIO_API UInteractionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionSystemComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", BlueprintGetter = GetInteractionName)
	FText InteractionName;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category = "Interaction")
	bool IsCanInteract(AActor* InteractActor) const;
	virtual bool IsCanInteract_Implementation(AActor* InteractActor) const {return false;}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void Interact(AActor* InteracActor);
	virtual void Interact_Implementation(AActor* InteractActor) {}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction", BlueprintGetter)
	const FText& GetInteractionName() const {return InteractionName;}
};
