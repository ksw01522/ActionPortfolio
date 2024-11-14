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
		CurrentCollisionCountForStun = 0;
	}
}
