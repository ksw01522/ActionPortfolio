// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/CharacterEnemy.h"
#include "Components/ProgressBar.h"
#include "Ability/ActionPFAttributeSet.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Instance/CharacterDataManager.h"
#include "Ability/Effects/AttributeEffect.h"
#include "ActionPortfolio.h"
#include "GameplayEffect.h"
#include "Ability/Effects/GameplayEffect_Bounty.h"
#include "GameplayEffectExtension.h"


ACharacterEnemy::ACharacterEnemy()
{
	EnemyAttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>("EnemyAttributeSet");

	UActionPFAbilitySystemComponent* ASC = Cast<UActionPFAbilitySystemComponent>(GetAbilitySystemComponent());
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	ASC->AddAttributeSetSubobject<UEnemyAttributeSet>(EnemyAttributeSet);
	
	SetGenericTeamId(FGenericTeamId(2));
}

void ACharacterEnemy::BeginPlay()
{
	Super::BeginPlay();
}

float ACharacterEnemy::GetBountyXP() const
{
	return EnemyAttributeSet->GetBountyXP();
}

float ACharacterEnemy::GetBountyGold() const
{
	return EnemyAttributeSet->GetBountyGold();
}

void ACharacterEnemy::InitializeAttributes()
{
	const UCharacterDataManager* DataManager = UCharacterDataManager::GetCharacterDataManager();
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

	const FCharacterAttribute* CharacterData = DataManager->GetCharacterData(GetCharacterCode());
	check(CharacterData->AttributeType == "Enemy");

	const FEnemyAttribute* EnemyData = static_cast<const FEnemyAttribute*>(CharacterData);

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();

	FGameplayEffectSpec EffectSpec(GetDefault<UAttributeEffect_Enemy>(), ContextHandle, 1);

	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetCharacterLevelName(), EnemyData->Level);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetMaxHealthName(), EnemyData->MaxHealth);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetHealthRegenName(), EnemyData->HealthRegen);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetMaxStaminaName(), EnemyData->MaxStamina);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetStaminaRegenName(), EnemyData->StaminaRegen);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetAttackPName(), EnemyData->AttackP);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetDefensePName(), EnemyData->DefenseP);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetFireResistanceName(), EnemyData->FireResistance);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetIceResistanceName(), EnemyData->IceResistance);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetElectricResistanceName(), EnemyData->ElectricResistance);

	EffectSpec.SetSetByCallerMagnitude(UEnemyAttributeSet::GetBountyXPName(), EnemyData->BountyXP);
	EffectSpec.SetSetByCallerMagnitude(UEnemyAttributeSet::GetBountyGoldName(), EnemyData->BountyGold);

	ASC->ApplyGameplayEffectSpecToSelf(EffectSpec);
}

void ACharacterEnemy::OnCharacterDie(const FGameplayEffectModCallbackData& Data)
{
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();

	FGameplayEffectSpec BountySpec(GetDefault<UGameplayEffect_Bounty>(), ContextHandle);
	BountySpec.SetSetByCallerMagnitude(UPlayerAttributeSet::GetXPName(), EnemyAttributeSet->GetBountyXP());
	BountySpec.SetSetByCallerMagnitude(UPlayerAttributeSet::GetGoldName(), EnemyAttributeSet->GetBountyGold());

	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(BountySpec, Data.EffectSpec.GetEffectContext().GetInstigatorAbilitySystemComponent());
}


