// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Tasks/AbilityTask_WaitEventInputPress.h"
#include "Ability/ActionPFAbilitySystemComponent.h"




void UAbilityTask_WaitEventInputPress::OnPressCallBack(int32 InInputID)
{
	if(!Ability || !AbilitySystemComponent.IsValid()) return;

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnPressDel.Broadcast(InInputID);
		if(bActivateOnce) EndTask();
	}
}

void UAbilityTask_WaitEventInputPress::Activate()
{
	if (!Ability || !AbilitySystemComponent.IsValid()) 
	{
		EndTask();
		return;
	}

	UActionPFAbilitySystemComponent* ASC = static_cast<UActionPFAbilitySystemComponent*>(AbilitySystemComponent.Get());

	EventHandle = ASC->GetGenericEventInputDelegate(InputID).AddUObject(this, &UAbilityTask_WaitEventInputPress::OnPressCallBack);
}

void UAbilityTask_WaitEventInputPress::OnDestroy(bool AbilityEnding)
{
	if (AbilitySystemComponent.IsValid())
	{
		UActionPFAbilitySystemComponent* ASC = static_cast<UActionPFAbilitySystemComponent*>(AbilitySystemComponent.Get());
		ASC->GetGenericEventInputDelegate(InputID).Remove(EventHandle);
	}

	Super::OnDestroy(AbilityEnding);
}

UAbilityTask_WaitEventInputPress* UAbilityTask_WaitEventInputPress::WaitEventInputPress(UGameplayAbility* OwningAbility, FName TaskInstanceName, int32 InputID, bool bActivateOnce)
{
	UAbilityTask_WaitEventInputPress* ReturnTask = NewAbilityTask<UAbilityTask_WaitEventInputPress>(OwningAbility, TaskInstanceName);
	ReturnTask->InputID = InputID;
	ReturnTask->bActivateOnce = bActivateOnce;

	return ReturnTask;
}
