// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/AN_SendGameplayEvent.h"
#include "Character/ActionPortfolioCharacter.h"

void UAN_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActionPortfolioCharacter* Char = MeshComp->GetOwner<AActionPortfolioCharacter>();
	if(Char == nullptr) return;

	UAbilitySystemComponent* AbilitySystem = Char->GetAbilitySystemComponent();
	FGameplayEventData Payload;
	Payload.Instigator = AbilitySystem->GetOwnerActor();

	Char->GetAbilitySystemComponent()->HandleGameplayEvent(EventTag , &Payload);
}
