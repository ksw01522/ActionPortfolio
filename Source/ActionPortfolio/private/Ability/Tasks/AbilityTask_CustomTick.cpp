
// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Tasks/AbilityTask_CustomTick.h"

UAbilityTask_CustomTick::UAbilityTask_CustomTick(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = 1;
}

UAbilityTask_CustomTick* UAbilityTask_CustomTick::CustomTick(UGameplayAbility* OwningAbility, FName TaskInstanceName, float MaxTickTime)
{
	UAbilityTask_CustomTick* ReturnTask = NewAbilityTask<UAbilityTask_CustomTick>(OwningAbility, TaskInstanceName);

	ReturnTask->MaxTickTime = MaxTickTime;
	ReturnTask->TimeStarted = 0;
	ReturnTask->bCustomTickFinished = true;
	ReturnTask->bWantsTickFinish = 0 < MaxTickTime;

	return ReturnTask;
}

void UAbilityTask_CustomTick::Activate()
{
	Super::Activate();
	if (!OnCustomTickEvent.IsBound())
	{
		return;
	}

	bCustomTickFinished = false;
	TimeStarted = GetWorld()->GetTimeSeconds();
	Time = 0;
}

void UAbilityTask_CustomTick::TickTask(float DeltaTime)
{
	if (bCustomTickFinished) {
		return;
	}

	if(ShouldBroadcastAbilityTaskDelegates())
				{ OnCustomTickEvent.Broadcast(DeltaTime); }

	if (bWantsTickFinish)
	{
		Time += DeltaTime;
		if (MaxTickTime < Time)
		{
			OnTickTimeFinished();
		}
	}
	
}



void UAbilityTask_CustomTick::OnTickTimeFinished()
{
	bCustomTickFinished = true;
	if (OnCustomTickEndEvent.IsBound() && ShouldBroadcastAbilityTaskDelegates()) OnCustomTickEndEvent.Broadcast();
	EndTask();
}