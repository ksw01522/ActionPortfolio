// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterStatusComponent.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Items/ItemManagerSubsystem.h"
#include "Items/Equipment/Item_Equipment.h"
#include "Items/Equipment/EquipmentAbility.h"
#include "Ability/ActionPFAttributeSet.h"
#include "Character/Player/InventoryComponent.h"

#include "ActionPortfolio.h"



bool UEquipmentSlot::CanItemInSlot(UItemBase* InItem) const
{
	PFLOG(Warning, TEXT(""));

	if(InItem == nullptr) return true;

	if(InItem->GetItemType() != EItemType::Equipment) return false;

	UItemBase_Equipment* EquipItem = Cast<UItemBase_Equipment>(InItem);

#if WITH_EDITOR
	if (EquipItem->GetEquipmentPart() != Part)
	{
		UEnum* PartEnum = StaticEnum<EEquipmentPart>();
		PFLOG(Warning, TEXT("Slot Part : %s, Item Part : %s"), *PartEnum->GetNameStringByValue((int64)Part), *PartEnum->GetNameStringByValue((int64)EquipItem->GetEquipmentPart()));
	}
#endif

	return EquipItem->GetEquipmentPart() == Part;
}

void UEquipmentSlot::ApplyItemEffects()
{
	if(GetItem() == nullptr || !BindedASC.IsValid()) return;

	UItemBase_Equipment* EquipItem = Cast<UItemBase_Equipment>(GetItem());
	check(EquipItem);
	
	FGameplayAbilitySpec AbilitySpec(EquipItem->GetEquipmentAbility());
	AbilitySpecHandle = BindedASC->GiveAbility(AbilitySpec);

	FGameplayEffectSpec BasicStatusUpEffectSpec = EquipItem->MakeAddStatusEffect(BindedASC->MakeEffectContext());
	StatusUpEffectHandle = BindedASC->ApplyGameplayEffectSpecToSelf(BasicStatusUpEffectSpec);
	
}

void UEquipmentSlot::RemoveItemEffects()
{
	if (!BindedASC.IsValid()) return;
	
	BindedASC->ClearAbility(AbilitySpecHandle);
	BindedASC->RemoveActiveGameplayEffect(StatusUpEffectHandle);
	ClearHandles();
}

void UEquipmentSlot::SlotDropTo(UItemSlot* To)
{
	if (To == nullptr) { return; }
	else if (To->GetSlotType() == "Inventory" || To->GetSlotType() == "Equipment")
	{
		if (CanItemInSlot(To->GetItem()) && To->CanItemInSlot(GetItem()))
		{
			UItemBase* ToItem = To->GetItem();
			To->SetItem(GetItem());
			SetItem(ToItem);
		}
	}
}

void UEquipmentSlot::NativeOnChangedItemInSlot(UItemBase* PrevItem)
{
	RemoveItemEffects();

	ApplyItemEffects();

	Super::NativeOnChangedItemInSlot(PrevItem);
}

UAbilitySystemComponent* UEquipmentSlot::GetAbilitySystemComponent() const
{
	return BindedASC.Get();
}

void UEquipmentSlot::BindAbilitySystemComponent(UAbilitySystemComponent* InASC)
{
	if(BindedASC == InASC) return;

	RemoveItemEffects();

	BindedASC = InASC;

	ApplyItemEffects();
}

UCharacterStatusComponent::UCharacterStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	for(EEquipmentPart EquipmentPart : TEnumRange<EEquipmentPart>())
	{
		UEquipmentSlot* NewSlot = NewObject<UEquipmentSlot>(this);
		NewSlot->SetEquipmentPart(EquipmentPart);
		NewSlot->BindAbilitySystemComponent(GetAbilitySystemComponent());

		EquipmentSlots.Add(NewSlot);
	}
}



UEquipmentSlot* UCharacterStatusComponent::GetEquipmentSlot(EEquipmentPart Part) const
{
	for (auto& Slot : EquipmentSlots)
	{
		if (Slot->GetEquipmentPart() == Part)
		{
			return Slot;
		}
	}

	return nullptr;
}

UAbilitySystemComponent* UCharacterStatusComponent::GetAbilitySystemComponent() const
{
	if (AActor* Owner = GetOwner())
	{
		IAbilitySystemInterface* OwnerASI = Cast<IAbilitySystemInterface>(Owner);
		if (OwnerASI)
		{
			return OwnerASI->GetAbilitySystemComponent();
		}
	}
	
	return nullptr;
}

void UCharacterStatusComponent::GetEquipmentSlots(TArray<UEquipmentSlot*>& Out)
{
	Out.Empty();
	int Count = EquipmentSlots.Num();

	for (int i = 0; i < Count; i++)
	{
		Out.Add(EquipmentSlots[i]);
	}
}
