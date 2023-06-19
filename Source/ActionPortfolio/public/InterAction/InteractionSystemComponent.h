// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractDelegate, AActor*, InteractActor);

class UCanInteractionCondition;

UCLASS( ClassGroup=(InteractSystem), Abstract, Blueprintable, BlueprintType)
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	TArray<UCanInteractionCondition*> InteractionConditions;

	UPROPERTY(BlueprintAssignable)
	FInteractDelegate Del_Interact;

private:
	virtual void Interact_CPP(AActor* InteractActor) {}
	virtual bool CanInteract_CPP(AActor* InteractActor) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	bool CanInteract(AActor* InteractActor) const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact(AActor* InteractActor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction", BlueprintGetter)
	const FText& GetInteractionName() const {return InteractionName;}
};

UCLASS(ClassGroup = (InteractSystem), Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class ACTIONPORTFOLIO_API UCanInteractionCondition : public UObject
{
	GENERATED_BODY()

	friend UInteractionSystemComponent;

public:
	UCanInteractionCondition() {};
	virtual ~UCanInteractionCondition() {};

private:
	virtual bool CanInteractCondition(AActor* InteractActor);
};
