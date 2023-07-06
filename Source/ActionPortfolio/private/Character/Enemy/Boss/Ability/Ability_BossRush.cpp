// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Boss/Ability/Ability_BossRush.h"
#include "Character/Enemy/Boss/BatteryBomb.h"
#include "Character/Enemy/Boss/CharacterBoss.h"
#include "ActionPortfolio.h"
#include "Character/ActionPFAnimInstance.h"

UAbility_BossRush::UAbility_BossRush()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RUSHAM(TEXT("/Game/ActionPFCharacter/Enemies/Boss/Animation/AM_BossRush.AM_BossRush"));
	if (RUSHAM.Succeeded())
	{
		MeeleeMontage = RUSHAM.Object;
	}
}

void UAbility_BossRush::OnEventReceived(FGameplayEventData EventData)
{
	ACharacterBoss* BossChar = Cast<ACharacterBoss>(GetAvatarActorFromActorInfo());

	if (!IsValid(BossChar))
	{
		PFLOG(Error, TEXT("Can't cast BossCharacter."));
		return;
	}

	if (ABatteryBomb* BatteryBomb = Cast<ABatteryBomb>(EventData.Target))
	{
		BossChar->OnBossCollisionBatteryBomb();
		BatteryBomb->SpawnBombParticle();
	}
	else
	{
		Super::OnEventReceived(EventData);
	}
	
	BossChar->GetMesh()->GetAnimInstance()->Montage_JumpToSection("End", MeeleeMontage);
}
