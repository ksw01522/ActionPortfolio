// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Tasks/AbilityTask_Timeline.h"



UAbilityTask_Timeline* UAbilityTask_Timeline::PlayTimeline(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	UAbilityTask_Timeline* ReturnTask = NewAbilityTask<UAbilityTask_Timeline>(OwningAbility, TaskInstanceName);

	return ReturnTask;
}

void UAbilityTask_Timeline::Activate()
{
	if (Ability == nullptr || CurveFloat == nullptr)
	{
		EndTask();
		return;
	}

	if (OnFloatEvent.IsBound())
	{
		FOnTimelineFloat FloatTimeline;
		FloatTimeline.BindUFunction(this, "CallFloatEvent");
	}

	if (OnEndEvent.IsBound())
	{
		FOnTimelineEvent FinishedEvent;
		FinishedEvent.BindUFunction(this, "CallTimelineEndEvent");
		Timeline.SetTimelineFinishedFunc(FinishedEvent);
	}
}

void UAbilityTask_Timeline::ExternalCancel()
{
	if (Timeline.IsPlaying()) {
		Timeline.Stop();
	}
	Super::ExternalCancel();
}



void UAbilityTask_Timeline::OnDestroy(bool AbilityEnded)
{
	if (Timeline.IsPlaying()) {
		Timeline.Stop();
	}

	Super::OnDestroy(AbilityEnded);


}
void UAbilityTask_Timeline::CallFloatEvent(float Value)
{
	OnFloatEvent.Broadcast(Value);
}

void UAbilityTask_Timeline::CallTimelineEndEvent()
{
	OnEndEvent.Broadcast();
}
