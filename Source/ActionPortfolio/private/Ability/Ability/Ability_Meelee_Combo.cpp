// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Ability_Meelee_Combo.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ActionPortfolio.h"

void UAbility_Meelee_Combo::OnReactiveEvent(FGameplayEventData EventData)
{
	if (bActivateFromAI && !bCanNextComboFromAI)
	{
		return;
	}

	if (MontageSectionNames.Num() <= ++SectionIdx) return;

	MontageJumpToSection(MontageSectionNames[SectionIdx]);
}

void UAbility_Meelee_Combo::OnMeeleeComboEnded(UGameplayAbility* Ability)
{
	if (ReactiveEventTask.IsValid())
	{
		ReactiveEventTask->EndTask();
	}
}

void UAbility_Meelee_Combo::OnEventReceived(FGameplayEventData EventData)
{
	if (EventData.Instigator.IsNull() || EventData.Target.IsNull()) return;


	if (bActivateFromAI && InstancingPolicy != EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		bCanNextComboFromAI = true;
	}

	Super::OnEventReceived(EventData);
}

void UAbility_Meelee_Combo::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if(!CommitCheck(Handle, ActorInfo, ActivationInfo)) return;

	if (!ActorInfo->PlayerController.IsValid() && InstancingPolicy != EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		bActivateFromAI = true;
		bCanNextComboFromAI = false;
	}

	if (MeeleeMontage != nullptr) {
		for (auto MontageSection : MeeleeMontage->CompositeSections) {
			MontageSectionNames.Add(MontageSection.SectionName);
		}
	}

	SectionIdx = 0;

	if (GetReactivateEventTag().IsValid()) {
		ReactiveEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetReactivateEventTag(), nullptr, bRactiveEventTriggerOnce, bReactiveEventTagMatchExact);
		ReactiveEventTask->EventReceived.AddDynamic(this, &UAbility_Meelee_Combo::OnReactiveEvent);
		OnGameplayAbilityEnded.AddUObject(this, &UAbility_Meelee_Combo::OnMeeleeComboEnded);
		ReactiveEventTask->ReadyForActivation();
	}


	Super::ActivateAbility_CPP(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
