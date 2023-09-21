// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Ability_Meelee_Combo.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ActionPortfolio.h"


UAbility_Meelee_Combo::UAbility_Meelee_Combo()
{
	ReactiveEventTask.Reset();
	ComboSelectPolicy = EComboSelectPolicy::Auto;
}

void UAbility_Meelee_Combo::OnEventReceived(FGameplayEventData EventData)
{
	if (!EventData.Instigator || !EventData.Target) return;


	if (InstancingPolicy != EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		bAttackedTarget = true;
	}

	Super::OnEventReceived(EventData);
}

void UAbility_Meelee_Combo::ActNextComboFromAI(FGameplayEventData EventData)
{
	if (MontageSectionNames.Num() <= ++SectionIdx && !bAttackedTarget) return;

	MontageJumpToSection(MontageSectionNames[SectionIdx]);
}

void UAbility_Meelee_Combo::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if(!CommitCheck(Handle, ActorInfo, ActivationInfo)) return;

	if (!ActorInfo->PlayerController.IsValid() && InstancingPolicy != EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		bAttackedTarget = false;

		UAbilityTask_WaitGameplayEvent* WaitComboEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Ability.PublicEvent.AICombo"));
		WaitComboEvent->EventReceived.AddDynamic(this, &UAbility_Meelee_Combo::ActNextComboFromAI);
		WaitComboEvent->ReadyForActivation();
	}
	
	MontageSectionNames.Empty();

	if (ComboSelectPolicy == EComboSelectPolicy::Auto) {
		for (auto MontageSection : MeeleeMontage->CompositeSections) {
			MontageSectionNames.Add(MontageSection.SectionName);
		}
		SectionIdx = 0;

	}
	else if (ComboSelectPolicy == EComboSelectPolicy::Menual)
	{
		MontageSectionNames = MenualComboNameArray;
		SectionIdx = -1;

	}


	Super::ActivateAbility_CPP(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAbility_Meelee_Combo::ReactivateAbility()
{
	if (MontageSectionNames.Num() <= ++SectionIdx) return;

	MontageJumpToSection(MontageSectionNames[SectionIdx]);
}
