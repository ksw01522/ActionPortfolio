// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/ActionPFPlayerState.h"
#include "Ability/ActionPFAttributeSet.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "ActionPortfolio.h"
#include "Instance/CharacterDataManager.h"
#include "Character/Player/ActionPFPlayerController.h"

AActionPFPlayerState::AActionPFPlayerState()
{
	PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>("PlayerAttributeSet", false);
}

void AActionPFPlayerState::BeginPlay()
{
	Super::BeginPlay();

	float BeginLevel = GetBeginLevel();
	float BeginXP = GetBeginXP();

	const UCharacterDataManager* DataManager = UCharacterDataManager::GetCharacterDataManager();
	const FRealCurve* XPForLevelUpCurve = DataManager->GetPlayerDataTable()->FindCurve("XPForLevelUp", "");
	float BeginXPForLevelUp = XPForLevelUpCurve->GetKeyValue(XPForLevelUpCurve->FindKey(BeginLevel));

	PlayerAttributeSet->InitPlayerLevel(BeginLevel);
	PlayerAttributeSet->InitXPForLevelUp(BeginXPForLevelUp);
	PlayerAttributeSet->SetXPForLevelUpCurve(XPForLevelUpCurve);
	PlayerAttributeSet->InitXP(BeginXP);

	if (APlayerController* PlayerController = GetPlayerController())
	{
		if (APawn* BeginPawn = PlayerController->GetPawn())
		{
			IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(BeginPawn);
			UActionPFAbilitySystemComponent* ASC = nullptr;

			if (ASI)
			{
				ASC = Cast<UActionPFAbilitySystemComponent>(ASI->GetAbilitySystemComponent());
			}

			SetAbilitySystemComponent(ASC);
		}

		PlayerController->OnPossessedPawnChanged.AddDynamic(this, &AActionPFPlayerState::OnPossessedPawnChanged);
	}
}
	


float AActionPFPlayerState::GetBeginLevel() const
{
	return 2;
}

float AActionPFPlayerState::GetBeginXP() const
{
	return 0.0f;
}

void AActionPFPlayerState::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(NewPawn);
	UActionPFAbilitySystemComponent* ASC = nullptr;

	if (ASI)
	{
		Cast<UActionPFAbilitySystemComponent>(ASI->GetAbilitySystemComponent());
	}

	SetAbilitySystemComponent(ASC);
}

void AActionPFPlayerState::PlayerLevelUp(float OldLv, float NewLv)
{
	if(OldLv == NewLv) return;
}

void AActionPFPlayerState::SetAbilitySystemComponent(UActionPFAbilitySystemComponent* InASC)
{
	if(WeakASC == InASC) return;

	if (WeakASC.IsValid())
	{
		WeakASC->RemoveSpawnedAttribute(PlayerAttributeSet);
	}

	WeakASC = InASC;

	if (WeakASC.IsValid())
	{
		WeakASC->AddSpawnedAttribute(PlayerAttributeSet);
	}
}

UAbilitySystemComponent* AActionPFPlayerState::GetAbilitySystemComponent() const
{
	return WeakASC.Get();
}

void AActionPFPlayerState::OnChangeGoldByEffect(const FGameplayEffectModCallbackData& Data)
{
	if (AActionPFPlayerController* PlayerController = Cast<AActionPFPlayerController>(GetOwningController()))
	{
		PlayerController->OnChangeGoldByEffect(Data);
	}
}
