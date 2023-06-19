// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/AnimNotifyState_GrantTag.h"
#include "Character/ActionPortfolioCharacter.h"
#include "ActionPortfolio.h"
#include "AbilitySystemInterface.h"
#include "Ability/ActionPFAbilitySystemComponent.h"

void UAnimNotifyState_GrantTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (GrantTag == FGameplayTag::EmptyTag || !IsValid(MeshComp) || !IsValid(MeshComp->GetOwner())) return;

	AActionPortfolioCharacter* Char = Cast<AActionPortfolioCharacter>(MeshComp->GetOwner());
	if (!IsValid(Char)) {
		PFLOG(Warning, TEXT("Didn't Call By ActionPFCharacter"));
		return;
	}
	OwnerAbilitySystem = Cast<UActionPFAbilitySystemComponent>(Char->GetAbilitySystemComponent());
	if (!OwnerAbilitySystem.IsValid()) {
		PFLOG(Warning, TEXT("Can't find AbilitySysetmComponent from %s"), *Char->GetActorNameOrLabel());
		return;
	}

	OwnerAbilitySystem->AddLooseGameplayTag(GrantTag);
}

void UAnimNotifyState_GrantTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if(!OwnerAbilitySystem.IsValid()) return;
	OwnerAbilitySystem->RemoveLooseGameplayTag(GrantTag);
}

FString UAnimNotifyState_GrantTag::GetNotifyName_Implementation() const
{
	FString NewNotifyName = FString::Printf(TEXT("GrantTag : %s"), *GrantTag.ToString());

	return NewNotifyName;
}
