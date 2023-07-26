// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_CustomTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCustomTickDelegate, float, DeltaTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomTickEndDelegate);

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UAbilityTask_CustomTick : public UAbilityTask
{
	GENERATED_BODY()
	
	UAbilityTask_CustomTick(const FObjectInitializer& ObjectInitializer);

private:
	float Time = 0;
	float TimeStarted;
	float MaxTickTime = -1;

	bool bCustomTickFinished = false;
	bool bWantsTickFinish;

private:
	void OnTickTimeFinished();

public:
	UPROPERTY(BlueprintAssignable)
	FCustomTickDelegate OnCustomTickEvent;

	UPROPERTY(BlueprintAssignable)
	FCustomTickEndDelegate OnCustomTickEndEvent;

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_CustomTick* CustomTick(
							UGameplayAbility* OwningAbility,
							FName TaskInstanceName,
							float MaxTickTime
						);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

};
