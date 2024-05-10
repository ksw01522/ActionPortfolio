// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/EnemyA/ChracterEnemyA.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameplayEffect.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActionPortfolio.h"

AChracterEnemyA::AChracterEnemyA()
{
	GetCapsuleComponent()->SetCapsuleRadius(64);
	GetCapsuleComponent()->SetCapsuleHalfHeight(120);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -120));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyASkeletalMesh(TEXT("/Game/ParagonCrunch/Characters/Heroes/Crunch/Skins/Tier_3/CrashSite/Meshes/Crunch_Crash_Site.Crunch_Crash_Site"));
	if (EnemyASkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(EnemyASkeletalMesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UGameplayEffect> EnemyADefaultAttribute(TEXT("/Game/ActionPFCharacter/Enemies/EnemyA/Ability/DefaultAttribute_EnemyA.DefaultAttribute_EnemyA_C"));
	if (EnemyADefaultAttribute.Succeeded())
	{
		DefaultAttributes = EnemyADefaultAttribute.Class;
	}
}

void AChracterEnemyA::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

