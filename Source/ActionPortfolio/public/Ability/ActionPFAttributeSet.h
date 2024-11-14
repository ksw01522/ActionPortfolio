// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "ActionPFAbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "ActionPFAttributeSet.generated.h"

/**
 * 
 */

#define ATTRIBUTE_CATEGORY "Base"

#define GAMEPLAYATTRIBUTE_TAG_GETTER(PropertyName) \
	FORCEINLINE FGameplayTag Get##PropertyName##Tag() \
	{ \
		return FGameplayTag::RequestGameplayTag("Attribute." + ATTRIBUTE_CATEGORY + "." + #PropertyName);\
	}

#define GAMEPLAYATTRIBUTE_NAME_GETTER(PropertyName) \
FORCEINLINE static FName Get##PropertyName##Name() \
{\
	return FName(#PropertyName);\
}

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \
	GAMEPLAYATTRIBUTE_NAME_GETTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedCharacterLevel, int, NewLevel);

UCLASS()
class ACTIONPORTFOLIO_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UCharacterAttributeSet();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;
	
public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CharacterLevel)
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, CharacterLevel)

	UPROPERTY(BlueprintReadOnly, Category = "Status", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Status", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Status", ReplicatedUsing = OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HealthRegen)

	UPROPERTY(BlueprintReadOnly, Category = "Status", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Status", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Status", ReplicatedUsing = OnRep_StaminaRegen)
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, StaminaRegen)

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Damage)

	UPROPERTY(BlueprintReadOnly, Category = "Status", ReplicatedUsing = OnRep_AttackP)
	FGameplayAttributeData AttackP;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, AttackP)

	UPROPERTY(BlueprintReadOnly, Category = "Status", ReplicatedUsing = OnRep_DefenseP)
	FGameplayAttributeData DefenseP;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, DefenseP)

	UPROPERTY(BlueprintReadOnly, Category = "Status", ReplicatedUsing = OnRep_FireResistance)
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, FireResistance)

	UPROPERTY(BlueprintReadOnly, Category = "Status", ReplicatedUsing = OnRep_IceResistance)
	FGameplayAttributeData IceResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, IceResistance)

	UPROPERTY(BlueprintReadOnly, Category = "Status", ReplicatedUsing = OnRep_ElectricResistance)
	FGameplayAttributeData ElectricResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ElectricResistance)

protected:
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float OldMaxValue, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

protected:
	UFUNCTION()
	void OnRep_CharacterLevel(const FGameplayAttributeData& OldLevel);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen);

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UFUNCTION()
	void OnRep_StaminaRegen(const FGameplayAttributeData& OldStaminaRegen);

	UFUNCTION()
	void OnRep_AttackP(const FGameplayAttributeData& OldAttackP);

	UFUNCTION()
	void OnRep_DefenseP(const FGameplayAttributeData& OldDefenseP);

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance);

	UFUNCTION()
	void OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance);

	UFUNCTION()
	void OnRep_ElectricResistance(const FGameplayAttributeData& OldElectricResistance);
};
#undef ATTRIBUTE_CATEGORY


UCLASS()
class ACTIONPORTFOLIO_API UEnemyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	//virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	//virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	//virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;


	//óġ �� ����ġ
public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BountyXP)
	FGameplayAttributeData BountyXP;
	ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, BountyXP)

	UFUNCTION()
	void OnRep_BountyXP(const FGameplayAttributeData& OldBountyXP);

	//óġ �� ���
public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BountyXP)
	FGameplayAttributeData BountyGold;
	ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, BountyGold)

	UFUNCTION()
	void OnRep_BountyGold(const FGameplayAttributeData& OldBountyGold);
};




UCLASS()
class ACTIONPORTFOLIO_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	//virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	//virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;



public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PlayerLevel)
	FGameplayAttributeData PlayerLevel;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, PlayerLevel)

	UFUNCTION()
	void OnRep_PlayerLevel(const FGameplayAttributeData& OldLevel);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_XP)
	FGameplayAttributeData XP;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, XP)

	UFUNCTION()
	void OnRep_XP(const FGameplayAttributeData& OldXP);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_XPForLevelUp)
	FGameplayAttributeData XPForLevelUp;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, XPForLevelUp)

	UFUNCTION()
	void OnRep_XPForLevelUp(const FGameplayAttributeData& OldXPForLevelUp);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Gold)
	FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Gold)

	UFUNCTION()
	void OnRep_Gold(const FGameplayAttributeData& OldGold);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SkillPoint)
	FGameplayAttributeData SkillPoint;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, SkillPoint)

	UFUNCTION()
	void OnRep_SkillPoint(const FGameplayAttributeData& OldSkillPoint);

	const FRealCurve* XPForLevelUpCurve = nullptr;

	bool CanGainXP() const { return 0 < GetXPForLevelUp(); };

	void CheckLevelUp();

public:
	void SetXPForLevelUpCurve(const FRealCurve* NewCurve);
};

#undef ATTRIBUTE_ACCESSORS
