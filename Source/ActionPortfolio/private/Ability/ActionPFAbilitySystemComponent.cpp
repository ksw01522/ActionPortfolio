// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "Character/ActionPortfolioCharacter.h"
#include "AbilitySystemGlobals.h"
#include "ActionPortfolio.h"
#include "Ability/Widget/SAbilityIcon.h"
#include "Ability/Widget/StateIconWidget.h"


UActionPFAbilitySystemComponent::UActionPFAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionPFAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		bSuppressGameplayCues = true;
	}
}

void UActionPFAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UActionPFAbilitySystemComponent::IsGenericEventInputBound(int32 InputID) const
{
	return GenericEventInputDelegate.Contains(InputID) && GenericEventInputDelegate[InputID].IsBound();
}

void UActionPFAbilitySystemComponent::OnTagUpdated(const FGameplayTag& Tag, bool TagExists)
{
	/*ENetMode NetMode = GetWorld()->GetNetMode();
	FString NetModeName = NetMode == ENetMode::NM_Client ? "Client" : "Server";

	int TagCount = GetTagCount(Tag);

	PFLOG(Warning, TEXT("%s : %s Count : %d"), *NetModeName, *Tag.ToString(), TagCount);*/

}

void UActionPFAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnRemoveAbility(AbilitySpec);


}

void UActionPFAbilitySystemComponent::ClearAbilityWithClass(TSubclassOf<class UGameplayAbility> InAbilityClass)
{
	if (!IsOwnerActorAuthoritative())
	{
		PFLOG(Error, TEXT("Attempted to call ClearAbilityWithClass() on the client. This is not allowed!"));

		return;
	}
	
	for (int Idx = 0; Idx < AbilityPendingAdds.Num(); ++Idx)
	{
		if (AbilityPendingAdds[Idx].Ability.GetClass() == InAbilityClass)
		{
			ClearAbility(AbilityPendingAdds[Idx].Handle);
			return;
		}
	}

	for (int Idx = 0; Idx < ActivatableAbilities.Items.Num(); ++Idx)
	{
		if (ActivatableAbilities.Items[Idx].Ability.GetClass() == InAbilityClass)
		{
			ClearAbility(ActivatableAbilities.Items[Idx].Handle);
			return;
		}
	}
}

void UActionPFAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UActionPFGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UActionPFGameplayAbility>(ActiveAbility));
		}
	}
}

bool UActionPFAbilitySystemComponent::IsActingAbilityByClass(TSubclassOf<UGameplayAbility> CheckAbility)
{
	for (FGameplayAbilitySpec Spec : ActivatableAbilities.Items)
	{
		if (CheckAbility == Spec.Ability->GetClass())
		{
			return Spec.IsActive();
		}
	}

	return false;
}


UActionPFAbilitySystemComponent* UActionPFAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	return Cast<UActionPFAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}

bool UActionPFAbilitySystemComponent::GetCooldownRemainingAndDurationByTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration) const
{
	if (CooldownTags.Num() <= 0) {
		TimeRemaining = 0;
		CooldownDuration = 0;
		return false;
	}

	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
	TArray< TPair<float, float> > DurationAndTimeRemaining = GetActiveEffectsTimeRemainingAndDuration(Query);
	if (DurationAndTimeRemaining.Num() <= 0) {
		return false;
	}

	int32 LongestIDX = 0;
	float LongestTime = DurationAndTimeRemaining[0].Key;
	for (int Idx = 1; Idx < DurationAndTimeRemaining.Num(); Idx++)
	{
		if(DurationAndTimeRemaining[Idx].Key <= LongestTime) continue;

		LongestIDX = Idx;
		LongestTime = DurationAndTimeRemaining[Idx].Key;
	}

	TimeRemaining = DurationAndTimeRemaining[LongestIDX].Key;
	CooldownDuration = DurationAndTimeRemaining[LongestIDX].Value;

	return true;
}

bool UActionPFAbilitySystemComponent::CanActivateAbility(TSubclassOf<UActionPFGameplayAbility> AbilityClass)
{
	const FGameplayAbilitySpec* Spec = FindAbilitySpecFromClass(AbilityClass);

	if(Spec == nullptr) return false;

	return Spec->Ability->CanActivateAbility(Spec->Handle, AbilityActorInfo.Get());
}

void UActionPFAbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	if (IsGenericConfirmInputBound(InputID))
	{
		LocalInputConfirm();
		return;
	}

	if (IsGenericCancelInputBound(InputID))
	{
		LocalInputCancel();
		return;
	}

	if (IsGenericEventInputBound(InputID))
	{
		GenericEventInputDelegate[InputID].Broadcast(InputID);
		return;
	}

	// ---------------------------------------------------------

	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.InputID == InputID)
		{
			if (Spec.Ability)
			{
				Spec.InputPressed = true;
				if (Spec.IsActive())
				{
					if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
					{
						ServerSetInputPressed(Spec.Handle);
					}

					AbilitySpecInputPressed(Spec);

					// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
				}
				else
				{
					// Ability is not active, so try to activate it
					TryActivateAbility(Spec.Handle);
				}
			}
		}
	}
}

void UActionPFAbilitySystemComponent::AbilityLocalInputReleased(int32 InputID)
{

	Super::AbilityLocalInputReleased(InputID);
}

void UActionPFAbilitySystemComponent::AbilityLocalInputPressedByClass(const TSubclassOf<UActionPFGameplayAbility>& InClass)
{
	const UGameplayAbility* const InAbilityCDO = InClass.GetDefaultObject();
	if(InAbilityCDO == nullptr) return;

	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability == InAbilityCDO)
		{
			Spec.InputPressed = true;
			if (Spec.IsActive())
			{
				if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
				{
					ServerSetInputPressed(Spec.Handle);
				}

				AbilitySpecInputPressed(Spec);

				// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
			}
			else
			{
				// Ability is not active, so try to activate it
				TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void UActionPFAbilitySystemComponent::AbilityLocalInputReleassedByClass(const TSubclassOf<UActionPFGameplayAbility>& InClass)
{
	const UGameplayAbility* const InAbilityCDO = InClass.GetDefaultObject();
	if (InAbilityCDO == nullptr) return;

	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability == InAbilityCDO)
		{
			Spec.InputPressed = false;
			if (Spec.IsActive())
			{
				if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
				{
					ServerSetInputReleased(Spec.Handle);
				}

				AbilitySpecInputReleased(Spec);

				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
			}

		}
	}
}

FGenericEventInputDelegate& UActionPFAbilitySystemComponent::GetGenericEventInputDelegate(int32 InputID)
{
	return GenericEventInputDelegate.FindOrAdd(InputID);
}


void UActionPFAbilitySystemComponent::AddStateIcon(const FGameplayTag& InTag, TSubclassOf<UStateIconWidget> InWidgetClass, const FSlateBrush& InBrush)
{
	for (auto& IconStruct : StateIconArray)
	{
		if (IconStruct.StateTag == InTag)
		{
			return;
		}
	}

	StateIconArray.Emplace(InTag, InWidgetClass, InBrush);
	OnNewOrRemovedStateIcon.Broadcast(InTag, InWidgetClass, InBrush, true);
}

void UActionPFAbilitySystemComponent::RemoveStateIcon(const FGameplayTag& InTag)
{
	for (int i = 0; i < StateIconArray.Num(); i++)
	{
		if (StateIconArray[i].StateTag != InTag) { continue; }
		
		FGameplayTag TempTag = StateIconArray[i].StateTag;
		TSubclassOf<UStateIconWidget> TempWidgetClass = StateIconArray[i].WidgetClass;
		FSlateBrush TempBrush = StateIconArray[i].ImageBrush;

		StateIconArray.RemoveAt(i);

		OnNewOrRemovedStateIcon.Broadcast(TempTag, TempWidgetClass, TempBrush, false);

		return;
	}

}




