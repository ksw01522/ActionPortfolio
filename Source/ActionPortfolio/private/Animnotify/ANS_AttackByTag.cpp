// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/ANS_AttackByTag.h"
#include "Character/ActionPortfolioCharacter.h"
#include "ActionPortfolio.h"
#include "Components/ShapeComponent.h"
#include "AbilitySystemInterface.h"
#include "Ability/ActionPFAbilitySystemComponent.h"


void UANS_AttackByTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!IsValid(MeshComp) || !IsValid(MeshComp->GetOwner())) return;
	OverlappedActors.Empty();

	AActionPortfolioCharacter* Char = Cast<AActionPortfolioCharacter>(MeshComp->GetOwner());
	if (!IsValid(Char)) {
		PFLOG(Warning, TEXT("Didn't Call By ActionPFCharacter"));
		return;
	}
	OwnerAbilitySystem = Cast<UActionPFAbilitySystemComponent>( Char->GetAbilitySystemComponent());
	if (!OwnerAbilitySystem.IsValid()) {
		PFLOG(Warning, TEXT("Can't find AbilitySysetmComponent from %s"), *Char->GetActorNameOrLabel());
		return;
	}
	
	AttackShape = Char->GetAttackShape(AttackTag);
	if (!AttackShape.IsValid()) {
		PFLOG(Warning, TEXT("Can't find AttackShape By AttackTag {%s}"), *AttackTag.ToString());
		return;
	}
	
	OverlappedActors.Add(MeshComp->GetOwner());

	
	AttackShape->OnComponentBeginOverlap.AddDynamic(this, &UANS_AttackByTag::OnAttackShapeOverlapped);
	AttackShape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UANS_AttackByTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if(!AttackShape.IsValid()) return;

	AttackShape->OnComponentBeginOverlap.RemoveAll(this);
	if (!AttackShape->OnComponentBeginOverlap.IsBound()) {
		AttackShape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

FString UANS_AttackByTag::GetNotifyName_Implementation() const
{
	FString FinalName;

	FString TagName = AttackTag.ToString();
	FinalName = FString::Printf(TEXT("Attack : %s   "), *TagName);

	return FinalName;
}

void UANS_AttackByTag::OnAttackShapeOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || OverlappedActors.Contains(OtherActor)) {
		return;
	}

	FGameplayEventData EventData;
	EventData.Target = OtherActor;
	EventData.Instigator = OwnerAbilitySystem->GetOwnerActor();

	OwnerAbilitySystem->HandleGameplayEvent(EventTag , &EventData);
	OverlappedActors.Add(OtherActor);
}

