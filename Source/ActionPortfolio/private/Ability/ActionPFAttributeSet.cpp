// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ActionPFAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "ActionPortfolio.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Character/ActionPortfolioCharacter.h"

UActionPFAttributeSet::UActionPFAttributeSet()
{
    StatusEffect = nullptr;
    CharacterLevel = 0;
}

void UActionPFAttributeSet::OnChangedCharacterLevelCallBack()
{
    OnChangedCharacterLevel.Broadcast(CharacterLevel);

    UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
    
    ASC->RemoveActiveGameplayEffect(StatusEffectHandle);

    StatusEffectHandle = ASC->ApplyGameplayEffectToSelf(StatusEffect, CharacterLevel, ASC->MakeEffectContext());
}

void UActionPFAttributeSet::SetCharacterLevel(int NewLevel)
{
    if(CharacterLevel == NewLevel) return;

    CharacterLevel = NewLevel;

    OnChangedCharacterLevelCallBack();
}

void UActionPFAttributeSet::SetStatusEffect(UGameplayEffect* NewStatusEffect)
{
    if(StatusEffect == NewStatusEffect) return;

    StatusEffect = NewStatusEffect;

    UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

    ASC->RemoveActiveGameplayEffect(StatusEffectHandle);

    StatusEffectHandle = ASC->ApplyGameplayEffectToSelf(StatusEffect, CharacterLevel, ASC->MakeEffectContext());
}

void UActionPFAttributeSet::InitializeStatus(int NewLevel, UGameplayEffect* NewStatusEffect)
{
    CharacterLevel = NewLevel;

    StatusEffect = NewStatusEffect;

    OnChangedCharacterLevelCallBack();
}

void UActionPFAttributeSet::SetHealth(float NewVal)
{
    NewVal = FMath::Clamp<float>(NewVal, 0.0f, GetMaxHealth());
    if(FMath::IsNearlyEqual(GetHealth(), NewVal)) return;


    UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
    if (ensure(ASC))
    {
        ASC->SetNumericAttributeBase(GetHealthAttribute(), NewVal);
    }
}

void UActionPFAttributeSet::SetMaxHealth(float NewVal)
{
    NewVal = FMath::Max<float>(0, NewVal);

    UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
    if (ensure(ASC))
    {
        ASC->SetNumericAttributeBase(GetMaxHealthAttribute(), NewVal);
    }

}

void UActionPFAttributeSet::SetStamina(float NewVal)
{
    NewVal = FMath::Clamp<float>(NewVal, 0.0f, GetMaxStamina());
    if (FMath::IsNearlyEqual(GetStamina(), NewVal)) return;


    UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
    if (ensure(ASC))
    {
        ASC->SetNumericAttributeBase(GetStaminaAttribute(), NewVal);
    }

}

void UActionPFAttributeSet::SetMaxStamina(float NewVal)
{
    NewVal = FMath::Max<float>(0, NewVal);

    UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
    if (ensure(ASC))
    {
        ASC->SetNumericAttributeBase(GetMaxStaminaAttribute(), NewVal);
    }
}


bool UActionPFAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
    if(!IsValid( GetOwningAbilitySystemComponent()) ) return false;
    if(GetOwningAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"))) return false;

    return true;
}

void UActionPFAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    if (Data.EvaluatedData.Attribute == GetDamageAttribute())
    {
        float LocalDamage = GetDamage();
        SetDamage(0);


        float NewHealth = GetHealth() - LocalDamage;
        SetHealth(NewHealth);

        AActionPortfolioCharacter* TargetCharacter = Cast<AActionPortfolioCharacter>(Data.Target.GetAvatarActor());

        if (GetHealth() <= 0)
        {
            TargetCharacter->CharacterDie();
        }
    }
}

void UActionPFAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

    

}

void UActionPFAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    if (Attribute == GetMaxHealthAttribute())
    {
        AdjustAttributeForMaxChange(Health, MaxHealth, OldValue, NewValue, GetHealthAttribute());
    }
    else if (Attribute == GetHealthAttribute()) {
        SetHealth(NewValue);
    }
}

void UActionPFAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float OldMaxValue, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
    UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();

    if (!FMath::IsNearlyEqual(OldMaxValue, NewMaxValue) && AbilityComp)
    {
        float CurrentValue = AffectedAttribute.GetCurrentValue();
        float NewDelta = (OldMaxValue > 0.f) ? (CurrentValue * NewMaxValue / OldMaxValue) - CurrentValue : NewMaxValue;

        AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
    }
}


void UActionPFAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{

}

void UActionPFAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{

}
