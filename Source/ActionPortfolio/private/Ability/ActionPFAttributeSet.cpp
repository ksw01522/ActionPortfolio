// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ActionPFAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "ActionPortfolio.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/ActionPFPlayerState.h"

UCharacterAttributeSet::UCharacterAttributeSet()
{
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, CharacterLevel, COND_None, REPNOTIFY_OnChanged);

    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, StaminaRegen, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, AttackP, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, DefenseP, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, IceResistance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, ElectricResistance, COND_None, REPNOTIFY_Always);
}

void UCharacterAttributeSet::OnRep_CharacterLevel(const FGameplayAttributeData& OldLevel)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, CharacterLevel, OldLevel)

}

bool UCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
    return true;
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    if (Data.EvaluatedData.Attribute == GetDamageAttribute())
    {
#if WITH_EDITOR
        PFLOG(Warning, TEXT("Damage : %.1f"), Data.EvaluatedData.Magnitude);
#endif

        float LocalDamage = GetDamage();
        SetDamage(0);

        float BeforeHealth = GetHealth();

        float NewHealth = GetHealth() - LocalDamage;
        SetHealth(NewHealth);

        
        if (GetHealth() <= 0)
        {
            UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

            AActionPortfolioCharacter* Avatar = Cast<AActionPortfolioCharacter>(ASC->GetAvatarActor());

            if (UAbilitySystemComponent* InstigatorASC = (Data.EffectSpec.GetEffectContext().GetInstigatorAbilitySystemComponent()))
            {
                FGameplayEventData KillEventData;
                KillEventData.ContextHandle = FGameplayEffectContextHandle(Data.EffectSpec.GetContext());
                KillEventData.Target = Avatar;

                InstigatorASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("CommonEvent.Kill"), &KillEventData);
            }

            Avatar->CharacterDie(Data);
        }
    }
}

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
    }
    else if (Attribute == GetStaminaAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0, GetMaxStamina());
    }
}

void UCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
#if WITH_EDITOR
    PFLOG(Warning, TEXT("{%s} Changed %s : %.1f -> %.1f"), *GetOwningActor()->GetName(), *Attribute.GetName(), OldValue, NewValue);
#endif

    if (Attribute == GetMaxHealthAttribute())
    {
        AdjustAttributeForMaxChange(Health, MaxHealth, OldValue, NewValue, GetHealthAttribute());
    }

    if (Attribute == GetMaxStaminaAttribute())
    {
        AdjustAttributeForMaxChange(Stamina, MaxStamina, OldValue, NewValue, GetStaminaAttribute());
    }
}

void UCharacterAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float OldMaxValue, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
    UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();

    if (!FMath::IsNearlyEqual(OldMaxValue, NewMaxValue) && AbilityComp)
    {
        float CurrentValue = AffectedAttribute.GetCurrentValue();
        float NewDelta = (OldMaxValue > 0.f) ? (CurrentValue * NewMaxValue / OldMaxValue) - CurrentValue : NewMaxValue;

        AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
    }
}


void UCharacterAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    if (Attribute == GetMaxHealthAttribute() || Attribute == GetMaxStaminaAttribute())
    {
        NewValue = FMath::Max<float>(0, NewValue);
    }
}

void UCharacterAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{

}

void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
}

void UCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UCharacterAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, HealthRegen, OldHealthRegen);
}

void UCharacterAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Stamina, OldStamina);
}

void UCharacterAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxStamina, OldMaxStamina);
}

void UCharacterAttributeSet::OnRep_StaminaRegen(const FGameplayAttributeData& OldStaminaRegen)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, StaminaRegen, OldStaminaRegen);
}

void UCharacterAttributeSet::OnRep_AttackP(const FGameplayAttributeData& OldAttackP)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, AttackP, OldAttackP);
}

void UCharacterAttributeSet::OnRep_DefenseP(const FGameplayAttributeData& OldDefenseP)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, DefenseP, OldDefenseP);
}

void UCharacterAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, FireResistance, OldFireResistance);
}

void UCharacterAttributeSet::OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, IceResistance, OldIceResistance);
}

void UCharacterAttributeSet::OnRep_ElectricResistance(const FGameplayAttributeData& OldElectricResistance)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, ElectricResistance, OldElectricResistance);
}



void UEnemyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UEnemyAttributeSet, BountyXP, COND_None, REPNOTIFY_OnChanged);
    DOREPLIFETIME_CONDITION_NOTIFY(UEnemyAttributeSet, BountyGold, COND_None, REPNOTIFY_OnChanged);
}


void UEnemyAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    if (Attribute == GetBountyXPAttribute() || Attribute == GetBountyGoldAttribute())
    {
        NewValue = FMath::Max<float>(0, NewValue);
    }
}

void UEnemyAttributeSet::OnRep_BountyXP(const FGameplayAttributeData& OldBountyXP)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UEnemyAttributeSet, BountyXP, OldBountyXP);
}

void UEnemyAttributeSet::OnRep_BountyGold(const FGameplayAttributeData& OldBountyGold)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UEnemyAttributeSet, BountyGold, OldBountyGold)
}






void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, XP, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, XPForLevelUp, COND_None, REPNOTIFY_OnChanged);
    DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Gold, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, SkillPoint, COND_None, REPNOTIFY_OnChanged);
}

bool UPlayerAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
    if (Data.EvaluatedData.Attribute == GetXPAttribute() && !CanGainXP())
    {
        return false;
    }

    return true;
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    if (Data.EvaluatedData.Attribute == GetXPAttribute())
    {
        CheckLevelUp();
    }
    else if (Data.EvaluatedData.Attribute == GetGoldAttribute())
    {
        if (AActionPFPlayerState* PlayerState = Cast<AActionPFPlayerState>(GetOwningActor()))
        {
            PlayerState->OnChangeGoldByEffect(Data);
        }
    }
}

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    if (Attribute == GetXPAttribute())
    {
        if(CanGainXP()) { NewValue = 0; }
    }
}

void UPlayerAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
#if WITH_EDITOR
    PFLOG(Warning, TEXT("{%s} Changed %s : %.1f -> %.1f"), *GetOwningActor()->GetName(), *Attribute.GetName(), OldValue, NewValue);
#endif

}

void UPlayerAttributeSet::OnRep_PlayerLevel(const FGameplayAttributeData& OldLevel)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, PlayerLevel, OldLevel)
}

void UPlayerAttributeSet::OnRep_XP(const FGameplayAttributeData& OldXP)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, XP, OldXP)
}

void UPlayerAttributeSet::OnRep_XPForLevelUp(const FGameplayAttributeData& OldXPForLevelUp)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, XPForLevelUp, OldXPForLevelUp)
}

void UPlayerAttributeSet::OnRep_Gold(const FGameplayAttributeData& OldGold)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Gold, OldGold)
}

void UPlayerAttributeSet::OnRep_SkillPoint(const FGameplayAttributeData& OldSkillPoint)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, SkillPoint, OldSkillPoint)
}

void UPlayerAttributeSet::CheckLevelUp()
{
    if(GetXP() < GetXPForLevelUp()) return;

    float OldLevel = GetPlayerLevel();
    float NewLevel = OldLevel;
    float NewXPValue = GetXP();

    float NewXPForLevelUp = GetXPForLevelUp();


    while (0 < NewXPForLevelUp && NewXPForLevelUp <= NewXPValue)
    {
        ++NewLevel;
        NewXPValue -= NewXPForLevelUp;
        FKeyHandle NewKeyHandle = XPForLevelUpCurve->FindKey(NewLevel);
        NewXPForLevelUp = XPForLevelUpCurve->GetKeyValue(NewKeyHandle);
    }

    AActionPFPlayerState* PlayerState = Cast<AActionPFPlayerState>(GetOuter());
    check(PlayerState)

    SetXP(NewXPValue);
    SetXPForLevelUp(NewXPForLevelUp);
    SetPlayerLevel(NewLevel);
    PlayerState->PlayerLevelUp(OldLevel, NewLevel);
}

void UPlayerAttributeSet::SetXPForLevelUpCurve(const FRealCurve* NewCurve)
{
    if(NewCurve == XPForLevelUpCurve) return;

    XPForLevelUpCurve = NewCurve;

    if (XPForLevelUpCurve)
    {
        FKeyHandle NewValueKey = XPForLevelUpCurve->FindKey(GetPlayerLevel());
        float NewValue = XPForLevelUpCurve->GetKeyValue(NewValueKey);
        if (GetOwningAbilitySystemComponent())
        {
            SetXPForLevelUp(NewValue);
        }
        else
        {
            InitXPForLevelUp(NewValue);
        }   
    }
    else
    {
        if (GetOwningAbilitySystemComponent())
        {
            SetXPForLevelUp(0);
        }
        else
        {
            InitXPForLevelUp(0);
        }
    }
}
