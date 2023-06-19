// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Tasks/AbilityTask_PlayMontageAndWaitEvent.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemGlobals.h"
#include "Animation/AnimInstance.h"
#include "ActionPortfolio.h"

UAbilityTask_PlayMontageAndWaitEvent::UAbilityTask_PlayMontageAndWaitEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Rate = 1.f;
	bStopWhenAbilityEnds = false;
}

UAbilityTask_PlayMontageAndWaitEvent* UAbilityTask_PlayMontageAndWaitEvent::PlayMontageAndWaitEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName,
	UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UAbilityTask_PlayMontageAndWaitEvent* ReturnOBJ = NewAbilityTask<UAbilityTask_PlayMontageAndWaitEvent>(OwningAbility, TaskInstanceName);
	ReturnOBJ->MontageToPlay = MontageToPlay;
	ReturnOBJ->EventTags = EventTags;
	ReturnOBJ->Rate = Rate;
	ReturnOBJ->StartSection = StartSection;
	ReturnOBJ->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	ReturnOBJ->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;

	return ReturnOBJ;
}



void UAbilityTask_PlayMontageAndWaitEvent::OnDestroy(bool AbilityEnded)
{
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
		if (AbilityEnded && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}

	UActionPFAbilitySystemComponent* ActionPFASC = GetTargetASC();
	if (ActionPFASC)
	{
		ActionPFASC->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
	}

	Super::OnDestroy(AbilityEnded);
}


UActionPFAbilitySystemComponent* UAbilityTask_PlayMontageAndWaitEvent::GetTargetASC()
{
	return Cast<UActionPFAbilitySystemComponent>(AbilitySystemComponent);
}


void UAbilityTask_PlayMontageAndWaitEvent::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	bool bPlayedMontage = false;
	UActionPFAbilitySystemComponent* ActionPFAbilitySystemComponent = GetTargetASC();

	if (ActionPFAbilitySystemComponent)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			// Bind to event callback
			EventHandle = ActionPFAbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UAbilityTask_PlayMontageAndWaitEvent::OnGameplayEvent));

			if (ActionPFAbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection) > 0.f)
			{
				if (!ShouldBroadcastAbilityTaskDelegates())
				{
					return;
				}

				CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UAbilityTask_PlayMontageAndWaitEvent::OnAbilityCancelled);

				BlendingOutDelegate.BindUObject(this, &UAbilityTask_PlayMontageAndWaitEvent::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UAbilityTask_PlayMontageAndWaitEvent::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
				if (Character)
				{
					Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
				}

				bPlayedMontage = true;
			}
		}
		else
		{
			PFLOG(Warning, TEXT("UAbilityTask_PlayMontageAndWaitEvent call to PlayMontage failed!"));
		}
	}
	else
	{
		PFLOG(Warning, TEXT("UAbilityTask_PlayMontageAndWaitEvent called on invalid AbilitySystemComponent"));
	}

	if (!bPlayedMontage)
	{
		PFLOG(Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay), *InstanceName.ToString());
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayEventData());
		}
	}

	SetWaitingOnAvatar();
}

void UAbilityTask_PlayMontageAndWaitEvent::ExternalCancel()
{
	check(AbilitySystemComponent.Get());

	OnAbilityCancelled();

	Super::ExternalCancel();
}

bool UAbilityTask_PlayMontageAndWaitEvent::StopPlayingMontage()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo)
	{
		return false;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}

	if (AbilitySystemComponent.IsValid() && Ability)
	{
		if (AbilitySystemComponent->GetAnimatingAbility() == Ability
			&& AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
		{
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			AbilitySystemComponent->CurrentMontageStop();
			return true;
		}
	}

	return false;
}

FString UAbilityTask_PlayMontageAndWaitEvent::GetDebugString() const
{
	UAnimMontage* PlayingMontage = nullptr;
	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? MontageToPlay : AnimInstance->GetCurrentActiveMontage();
		}
	}

	return FString::Printf(TEXT("PlayMontageAndWaitEvent. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}

void UAbilityTask_PlayMontageAndWaitEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (Ability && Ability->GetCurrentMontage() == MontageToPlay)
	{
		if (Montage == MontageToPlay)
		{
			AbilitySystemComponent->ClearAnimatingAbility(Ability);

			ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
			if (Character)
			{
				Character->SetAnimRootMotionTranslationScale(1.f);
			}

		}
	}

	if (bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.Broadcast(FGameplayEventData());
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnBlendOut.Broadcast(FGameplayEventData());
		}
	}
}

void UAbilityTask_PlayMontageAndWaitEvent::OnAbilityCancelled()
{
	if (StopPlayingMontage())
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayEventData());
		}
	}
}

void UAbilityTask_PlayMontageAndWaitEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleted.Broadcast(FGameplayEventData());
		}
	}

	EndTask();
}

void UAbilityTask_PlayMontageAndWaitEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData TempData = *Payload;
		TempData.EventTag = EventTag;

		EventReceived.Broadcast(TempData);
	}
}

