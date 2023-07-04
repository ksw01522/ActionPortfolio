// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/EnemyA/ChracterEnemyA.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameplayEffect.h"

AChracterEnemyA::AChracterEnemyA()
{
	GetCapsuleComponent()->SetCapsuleRadius(64);
	GetCapsuleComponent()->SetCapsuleHalfHeight(120);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -120));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyASkeletalMesh(TEXT("/Game/ParagonCrunch/Characters/Heroes/Crunch/Skins/Teir_1/Military/Meshes/Crunch_Military.Crunch_Military"));
	if (EnemyASkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(EnemyASkeletalMesh.Object);
	}

	LeftPunchBox = CreateDefaultSubobject<UBoxComponent>("LeftPunchBox");
	RightPunchBox = CreateDefaultSubobject<UBoxComponent>("RightPunchBox");

	LeftPunchBox->SetupAttachment(GetMesh(), "hand_l");
	RightPunchBox->SetupAttachment(GetMesh(), "hand_r");

	LeftPunchBox->SetRelativeLocation(FVector(40, 0, 0));
	RightPunchBox->SetRelativeLocation(FVector(-40, 0, 0));

	LeftPunchBox->SetBoxExtent(FVector(64, 48, 48));
	RightPunchBox->SetBoxExtent(FVector(64, 48, 48));

	LeftPunchBox->ComponentTags.Add("LeftPunch");
	RightPunchBox->ComponentTags.Add("RightPunch");

	AttackShapeTags.Add("LeftPunch");
	AttackShapeTags.Add("RightPunch");

	static ConstructorHelpers::FClassFinder<UGameplayEffect> EnemyADefaultAttribute(TEXT("/Game/ActionPFCharacter/Enemies/EnemyA/Ability/DefaultAttribute_EnemyA.DefaultAttribute_EnemyA_C"));
	if (EnemyADefaultAttribute.Succeeded())
	{
		DefaultAttributes = EnemyADefaultAttribute.Class;
	}
}

