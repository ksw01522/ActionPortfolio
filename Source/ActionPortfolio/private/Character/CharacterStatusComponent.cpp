// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterStatusComponent.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Items/ItemManagerSubsystem.h"
#include "Items/ItemBase.h"
#include "Items/Euipment/EquipmentAbility.h"
#include "Ability/ActionPFAttributeSet.h"
#include "Character/Player/InventoryComponent.h"

#include "ActionPortfolio.h"

// Sets default values for this component's properties
UCharacterStatusComponent::UCharacterStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

/*
// Called when the game starts
void UCharacterStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	AActionPortfolioCharacter* OwnerChar = GetOwner<AActionPortfolioCharacter>();
	if (OwnerChar)
	{
		AbilitySystemComponent = Cast<UActionPFAbilitySystemComponent>(OwnerChar->GetComponentByClass(UActionPFAbilitySystemComponent::StaticClass()));
		if (!AbilitySystemComponent.IsValid())
		{
			PFLOG(Error, TEXT("Can't find Ability System Component : %s"), *OwnerChar->GetName());
		}
	}
	else
	{
		PFLOG(Error, TEXT("Can't find Owner Character"));
	}
}

void UCharacterStatusComponent::EquipItem(FName ItemCode, int Idx)
{
	UItemManagerSubsystem* ItemManager = UItemManagerSubsystem::GetManagerInstance();
	const FItemData_Equipment* ItemData = ItemManager->FindItemData_Equipment(ItemCode);

	UnequipItem(Idx);

	UGameplayEffect* BasicStatusEffect = MakeAddStatusEffect(ItemData);
	AbilitySystemComponent->ApplyGameplayEffectToSelf(BasicStatusEffect, 1, AbilitySystemComponent->MakeEffectContext());

	if (ItemData->EquipmentAbilities.GetDefaultObject() != nullptr)
	{
		FGameplayAbilitySpec AbilitySpec(ItemData->EquipmentAbilities, 1, -1, AbilitySystemComponent->GetOwner());
		EquipmentSlots[Idx].EquipAbilitySpecHandle = AbilitySystemComponent->GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

UGameplayEffect* UCharacterStatusComponent::MakeAddStatusEffect(const FItemData_Equipment* ItemData)
{
	//기본 스테이터스 증가 Effect 만들기
		//기본 설정
	UGameplayEffect* AddStatusEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("AddStatusEffect")));
	AddStatusEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	AddStatusEffect->Modifiers.SetNum(4);

	//MaxHP설정
	FGameplayModifierInfo& MaxHPInfo = AddStatusEffect->Modifiers[0];
	MaxHPInfo.Attribute = UActionPFAttributeSet::GetMaxHealthAttribute();
	MaxHPInfo.ModifierOp = EGameplayModOp::Additive;
	MaxHPInfo.ModifierMagnitude = FScalableFloat(ItemData->MaxHP);

	//MaxStamina
	FGameplayModifierInfo& MaxStaminaInfo = AddStatusEffect->Modifiers[1];
	MaxStaminaInfo.Attribute = UActionPFAttributeSet::GetMaxStaminaAttribute();
	MaxStaminaInfo.ModifierOp = EGameplayModOp::Additive;
	MaxStaminaInfo.ModifierMagnitude = FScalableFloat(ItemData->MaxStamina);

	//AttackP
	FGameplayModifierInfo& AttackPInfo = AddStatusEffect->Modifiers[2];
	AttackPInfo.Attribute = UActionPFAttributeSet::GetAttackPAttribute();
	AttackPInfo.ModifierOp = EGameplayModOp::Additive;
	AttackPInfo.ModifierMagnitude = FScalableFloat(ItemData->AttackP);

	//DefenseP
	FGameplayModifierInfo& DefensePInfo = AddStatusEffect->Modifiers[3];
	DefensePInfo.Attribute = UActionPFAttributeSet::GetDefensePAttribute();
	DefensePInfo.ModifierOp = EGameplayModOp::Additive;
	DefensePInfo.ModifierMagnitude = FScalableFloat(ItemData->DefenseP);

	return AddStatusEffect;
}

bool UCharacterStatusComponent::CanEquipItem(FName ItemCode)
{
	if(!AbilitySystemComponent.IsValid()) return false;

	bool bCanEquip = true;

	UItemManagerSubsystem* ItemManager = UItemManagerSubsystem::GetManagerInstance();
	const FItemData_Equipment* ItemData = ItemManager->FindItemData_Equipment(ItemCode);

	if (ItemData->EquipmentAbilities.GetDefaultObject() != nullptr)
	{
		bCanEquip = bCanEquip && AbilitySystemComponent->CanActivateAbility(ItemData->EquipmentAbilities);
	}

	return false;
}

bool UCharacterStatusComponent::TryEquipItem(FName ItemCode)
{
#if WITH_EDITOR
	if (EquipmentSlots.IsEmpty())
	{
		PFLOG(Error, TEXT("EquipmentSlots Is Empty."));
		return false;
	}
#endif

	for (int i = 0; i < EquipmentSlots.Num(); i++)
	{
		if(EquipmentSlots[i].ItemCode != NAME_None) continue;

		return TryEquipItem(ItemCode, i);
	}

	return TryEquipItem(ItemCode, 0);
}

bool UCharacterStatusComponent::TryEquipItem(FName ItemCode, int Idx)
{
	if (!EquipmentSlots.IsValidIndex(Idx))
	{
		PFLOG(Error, TEXT("Try Equip Item Invalidate Index : %d"), Idx);
		return false;
	}

	if(!CanEquipItem(ItemCode)) return false;

	EquipItem(ItemCode, Idx);
	
	return true;
}

bool UCharacterStatusComponent::TryEquipItem(FInventorySlot* InventorySlot)
{
	if(InventorySlot == nullptr || EquipmentSlots.IsEmpty()) return false;

	if (!CanEquipItem(InventorySlot->GetItemCode())) return false;

	bool bResult = false;
	FName CodeInSlot = NAME_None;

	for (int i = 0; i < EquipmentSlots.Num(); i++)
	{
		if (EquipmentSlots[i].ItemCode != NAME_None) continue;

		bResult = TryEquipItem(InventorySlot->GetItemCode(), i);
		break;
	}

	if (!bResult)
	{
		CodeInSlot = EquipmentSlots[0].ItemCode;
		bResult = TryEquipItem(InventorySlot->GetItemCode(), 0);
	}

	if (bResult)
	{
		InventorySlot->SetSlot(CodeInSlot, 1);
	}

	return bResult;
}

bool UCharacterStatusComponent::TryEquipItem(FInventorySlot* InventorySlot, int Idx)
{
	if (!EquipmentSlots.IsValidIndex(Idx))
	{
		PFLOG(Error, TEXT("Try Equip Item Invalidate Index : %d"), Idx);
		return false;
	}
	if (InventorySlot == nullptr) return false;

	if (!CanEquipItem(InventorySlot->GetItemCode())) return false;


	bool bResult = false;
	FName CodeInSlot = NAME_None;

	CodeInSlot = EquipmentSlots[Idx].ItemCode;
	bResult = TryEquipItem(InventorySlot->GetItemCode(), Idx);

	if (bResult)
	{
		InventorySlot->SetSlot(CodeInSlot, 1);
	}

	return bResult;
}

void UCharacterStatusComponent::UnequipItem(int Idx)
{
	if(EquipmentSlots[Idx].ItemCode == NAME_None) return;

	AbilitySystemComponent->CancelAbilityHandle(EquipmentSlots[Idx].EquipAbilitySpecHandle);
	AbilitySystemComponent->RemoveActiveGameplayEffect(EquipmentSlots[Idx].EquipEffectHandle);
	EquipmentSlots[Idx].ItemCode = NAME_None;
}

FName UCharacterStatusComponent::GetEquippedItemCode(int Idx)
{
	return EquipmentSlots[Idx].ItemCode;
}

int UCharacterStatusComponent::GetEmptySlot() const
{
	for(int i = 0; i < EquipmentSlots.Num() ; i++)
	{
		if (EquipmentSlots[i].ItemCode == NAME_None)
		{
			return i;
		}
	}

	return -1;
}



*/