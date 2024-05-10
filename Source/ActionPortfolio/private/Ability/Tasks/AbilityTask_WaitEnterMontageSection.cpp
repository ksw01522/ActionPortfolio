// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Tasks/AbilityTask_WaitEnterMontageSection.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "Character/ActionPortfolioCharacter.h"
#include "ActionPortfolio.h"

UAbilityTask_WaitEnterMontageSection::UAbilityTask_WaitEnterMontageSection(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}


void UAbilityTask_WaitEnterMontageSection::Activate()
{
	if (!Ability || !AbilitySystemComponent.IsValid())
	{
		EndTask();
		return;
	}

	AnimInstance = Ability->GetCurrentActorInfo()->GetAnimInstance();

	TArray<FString> Errors;
	if (!CanActivateWaitSection(Errors))
	{
		for (auto& ErrorString : Errors)
		{
			PFLOG(Error, TEXT("%s"), *ErrorString);
		}

		EndTask();
		return;
	}
}

void UAbilityTask_WaitEnterMontageSection::ExternalCancel()
{
	Super::ExternalCancel();
}

FString UAbilityTask_WaitEnterMontageSection::GetDebugString() const
{
	return FString::Printf(TEXT("Wait Section Name : %s"), *WaitSectionName.ToString()); 
}

void UAbilityTask_WaitEnterMontageSection::OnEnterSectionCallBack()
{
	OnEnterSection.Broadcast(WaitMontage, WaitSectionName);

	EndTask();
}

bool UAbilityTask_WaitEnterMontageSection::CanActivateWaitSection(TArray<FString>& Errors)
{
	if (!WaitMontage)
	{
		Errors.Add(FString::Printf(TEXT("Can't find Wait Montage from : %s"), *Ability->GetName()));
		
		
	}
	else if (!WaitMontage->GetSectionIndex(WaitSectionName))
	{
		Errors.Add(FString::Printf(TEXT("Can't find Section Name %s from : %s"), *WaitSectionName.ToString(), *WaitMontage->GetName()));
	}
	else if (!AnimInstance.IsValid())
	{
		Errors.Add(TEXT("Can't Find AnimInstance"));
	}
	

	return Errors.IsEmpty();
}

void UAbilityTask_WaitEnterMontageSection::OnDestroy(bool AbilityEnded)
{

	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_WaitEnterMontageSection::TickTask(float DeltaTime)
{
	FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(WaitMontage);

	if (!MontageInstance)
	{
		EndTask();
		return;
	}

	if (MontageInstance->GetCurrentSection() == WaitSectionName)
	{
		OnEnterSectionCallBack();
	}
}

UAbilityTask_WaitEnterMontageSection* UAbilityTask_WaitEnterMontageSection::WaitEnterMontageSection(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* InWaitMontage, const FName& WaitSectionName)
{
	UAbilityTask_WaitEnterMontageSection* ReturnTask = NewAbilityTask<UAbilityTask_WaitEnterMontageSection>(OwningAbility, TaskInstanceName);

	ReturnTask->WaitMontage = InWaitMontage;
	ReturnTask->WaitSectionName = WaitSectionName;

	return ReturnTask;
}


	



