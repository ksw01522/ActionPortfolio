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
	BossRushBox = CreateDefaultSubobject<UBoxComponent>("BossRushBox");

	LeftPunchBox->SetupAttachment(GetMesh(), "hand_l");
	RightPunchBox->SetupAttachment(GetMesh(), "hand_r");
	BossRushBox->SetupAttachment(GetMesh());

	LeftPunchBox->SetRelativeLocation(FVector(40, 0, 0));
	RightPunchBox->SetRelativeLocation(FVector(-40, 0, 0));
	BossRushBox->SetRelativeLocation(FVector(20, 150, 155));

	LeftPunchBox->SetBoxExtent(FVector(64, 48, 48));
	RightPunchBox->SetBoxExtent(FVector(64, 48, 48));
	BossRushBox->SetBoxExtent(FVector(72,128,64));

	LeftPunchBox->ComponentTags.Add("LeftPunch");
	RightPunchBox->ComponentTags.Add("RightPunch");
	BossRushBox->ComponentTags.Add("BossRush");

	AttackShapeTags.Add("LeftPunch");
	AttackShapeTags.Add("RightPunch");
	AttackShapeTags.Add("BossRush");

	LeftPunchBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightPunchBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BossRushBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FClassFinder<UGameplayEffect> BossDefaultAttribute(TEXT("/Game/ActionPFCharacter/Enemies/Boss/Ability/DefaultAttribute_Boss.DefaultAttribute_Boss_C"));
	if (BossDefaultAttribute.Succeeded())
	{
		DefaultAttributes = BossDefaultAttribute.Class;
	}

	static ConstructorHelpers::FClassFinder<UGameplayEffect> BossDefaultEffect(TEXT("/Game/ActionPFCharacter/Enemies/Boss/Ability/DefaultEffect_Boss.DefaultEffect_Boss_C"));
	if (BossDefaultEffect.Succeeded()) {
		StartupEffects.Add(BossDefaultEffect.Class);
	}

	CollisionCountForStun = 2;
	CurrentCollisionCountForStun = 0;
}

void ACharacterBoss::OnBossCollisionBatteryBomb()
{
	CurrentCollisionCountForStun++;

	if (CollisionCountForStun <= CurrentCollisionCountForStun) {
		CharacterRigidity(StunTime);
		CurrentCollisionCountForStun = 0;
	}
}
