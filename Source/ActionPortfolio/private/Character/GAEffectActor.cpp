// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GAEffectActor.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Character/ActionPortfolioCharacter.h"
// Sets default values
AGAEffectActor::AGAEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGAEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGAEffectActor::ApplyEffectToTarget(UAbilitySystemComponent* AbilitySystem, ETeamAttitude::Type TeamAttitude)
{
	for (auto EffectSpecHandle : EffectSpecHandles)
	{
		AbilitySystem->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}


}

// Called every frame
void AGAEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGAEffectActor::EffectToTarget(UAbilitySystemComponent* AbilitySystem)
{
	if(!IsValid(AbilitySystem) || EffectSpecHandles.IsEmpty()) return;

	AActor* InstigatorActor = EffectSpecHandles[0].Data.Get()->GetContext().GetInstigator();
	AActor* Target = AbilitySystem->GetOwnerActor();
	IGenericTeamAgentInterface* InstigatorTemaAgent = Cast<IGenericTeamAgentInterface>(InstigatorActor);

	ETeamAttitude::Type TeamAttitude = InstigatorTemaAgent->GetTeamAttitudeTowards(*Target);

	switch (TeamAttitude)
	{
	case ETeamAttitude::Friendly:
		if(!bEffectToFriendly) return;
		break;
	case ETeamAttitude::Neutral:
		if(!bEffectToNeutral) return;
		break;
	case ETeamAttitude::Hostile:
		if(!bEffectToHostile) return;
		break;
	}

	ApplyEffectToTarget(AbilitySystem, TeamAttitude);
}

