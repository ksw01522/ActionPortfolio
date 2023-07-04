// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Boss/CharacterBoss.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameplayEffect.h"

ACharacterBoss::ACharacterBoss()
{
	GetCapsuleComponent()->SetCapsuleRadius(64);
	GetCapsuleComponent()->SetCapsuleHalfHeight(120);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -120));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BOSSMESH(TEXT("/Game/ParagonCrunch/Characters/Heroes/Crunch/Skins/Tier_3/BlackSite/Meshes/Crunch_Black_Site.Crunch_Black_Site"));
	if (BOSSMESH.Succeeded()) {
		GetMesh()->SetSkeletalMeshAsset(BOSSMESH.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> BOSSANIMINSTANCE(TEXT("/Game/ActionPFCharacter/Player/Blueprints/ABP_Player.ABP_Player_C"));
	if (BOSSANIMINSTANCE.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(BOSSANIMINSTANCE.Class);
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

	static ConstructorHelpers::FClassFinder<UGameplayEffect> BossDefaultAttribute(TEXT("/Game/ActionPFCharacter/Enemies/Boss/Ability/DefaultAttribute_Boss.DefaultAttribute_Boss_C"));
	if (BossDefaultAttribute.Succeeded())
	{
		DefaultAttributes = BossDefaultAttribute.Class;
	}

	static ConstructorHelpers::FClassFinder<UGameplayEffect> BossDefaultEffect(TEXT("/Game/ActionPFCharacter/Enemies/Boss/Ability/DefaultEffect_Boss.DefaultEffect_Boss_C"));
	if (BossDefaultEffect.Succeeded()) {
		StartupEffects.Add(BossDefaultEffect.Class);
	}
}
