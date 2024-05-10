// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "ActionPFAbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "ActionPFAttributeSet.generated.h"

/**
 * 
 */

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedCharacterLevel, int, NewLevel);

UCLASS()
class ACTIONPORTFOLIO_API UActionPFAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UActionPFAttributeSet();

private:
	UPROPERTY()
	int CharacterLevel;

	void OnChangedCharacterLevelCallBack();

	UPROPERTY(Transient)
	TObjectPtr<UGameplayEffect> StatusEffect;

	FActiveGameplayEffectHandle StatusEffectHandle;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Status")
	int GetCharacterLevel() const { return CharacterLevel; }

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetCharacterLevel(int NewLevel);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetStatusEffect(UGameplayEffect* NewStatusEffect);

	void InitializeStatus(int NewLevel, UGameplayEffect* NewStatusEffect);

	FOnChangedCharacterLevel OnChangedCharacterLevel;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	FGameplayAttributeData Health;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UActionPFAttributeSet, Health)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Health)
	void SetHealth(float NewVal);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Health)

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	FGameplayAttributeData MaxHealth;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UActionPFAttributeSet, MaxHealth)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxHealth)
	void SetMaxHealth(float NewVal);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	FGameplayAttributeData Stamina;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UActionPFAttributeSet, Stamina)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Stamina)
	void SetStamina(float NewVal);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	FGameplayAttributeData MaxStamina;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UActionPFAttributeSet, MaxStamina)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxStamina)
	void SetMaxStamina(float NewVal);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UActionPFAttributeSet, Damage)

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	FGameplayAttributeData AttackP;
	ATTRIBUTE_ACCESSORS(UActionPFAttributeSet, AttackP)

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	FGameplayAttributeData DefenseP;
	ATTRIBUTE_ACCESSORS(UActionPFAttributeSet, DefenseP)

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UActionPFAttributeSet, FireResistance)

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	FGameplayAttributeData IceResistance;
	ATTRIBUTE_ACCESSORS(UActionPFAttributeSet, IceResistance)

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	FGameplayAttributeData ElectricResistance;
	ATTRIBUTE_ACCESSORS(UActionPFAttributeSet, ElectricResistance)


protected:
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float OldMaxValue, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

public:
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;
};

#undef ATTRIBUTE_ACCESSORS