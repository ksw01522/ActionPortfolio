// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/AN_ActivateAbilityByClass.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "Character/ActionPortfolioCharacter.h"
#include "ActionPortfolio.h"

UAN_ActivateAbilityByClass::UAN_ActivateAbilityByClass()
{
	
}

void UAN_ActivateAbilityByClass::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(AbilityClassToActivate.GetDefaultObject() == nullptr) return;

	AActionPortfolioCharacter* Char = Cast<AActionPortfolioCharacter>(MeshComp->GetOwner());
	if (IsValid(Char))
	{
		Char->GetAbilitySystemComponent()->TryActivateAbilityByClass(AbilityClassToActivate);
	}
}
