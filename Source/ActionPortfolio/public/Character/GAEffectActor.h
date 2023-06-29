// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "GenericTeamAgentInterface.h"
#include "GAEffectActor.generated.h"

UCLASS(Abstract)
class ACTIONPORTFOLIO_API AGAEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGAEffectActor();

private:
	


public:
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TArray<FGameplayEffectSpecHandle> EffectSpecHandles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bEffectToFriendly;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bEffectToNeutral;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bEffectToHostile;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ApplyEffectToTarget(class UAbilitySystemComponent* AbilitySystem, ETeamAttitude::Type TeamAttitude);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void EffectToTarget(class UAbilitySystemComponent* AbilitySystem);
};
