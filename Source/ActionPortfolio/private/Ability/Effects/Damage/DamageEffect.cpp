// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/Damage/DamageEffect.h"
#include "Ability/Effects/Damage/DamageExecutionCalculation.h"
#include "Ability/ActionPFAbilitySystemComponent.h"


FDamageDataStruct::FDamageDataStruct()
{
	DamageClass = UDamageEffect_Meelee::StaticClass();
}

UDamageEffect::UDamageEffect()
{
	FGameplayEffectExecutionDefinition DamageExecution;

	//���� ������ �߰�
	FGameplayTag AbsoluteDamageTag = FGameplayTag::RequestGameplayTag("Data.Damage.Absolute");

	FGameplayEffectExecutionScopedModifierInfo ADEMI(AbsoluteDamageTag);
	ADEMI.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat AbsoluteDamageCaller;
	AbsoluteDamageCaller.DataTag = AbsoluteDamageTag;

	ADEMI.ModifierMagnitude = FGameplayEffectModifierMagnitude(AbsoluteDamageCaller);

	DamageExecution.CalculationModifiers.Add(ADEMI);

	//��� �޴� ������ �߰�
	FGameplayTag RelativeDamageTag = FGameplayTag::RequestGameplayTag("Data.Damage.Relative");

	FGameplayEffectExecutionScopedModifierInfo RDEMI(RelativeDamageTag);
	RDEMI.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat RelativeDamageCaller;
	RelativeDamageCaller.DataTag = RelativeDamageTag;

	RDEMI.ModifierMagnitude = FGameplayEffectModifierMagnitude(RelativeDamageCaller);

	DamageExecution.CalculationModifiers.Add(RDEMI);

	Executions.Add(DamageExecution);

}

UDamageEffect_Meelee::UDamageEffect_Meelee()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag("DamageType.Meelee"));

	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Damage.All"));
	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Damage.Meelee"));

	Executions[0].CalculationClass = UDamageExecutionCalculation_Meelee::StaticClass();
}



UDamageEffect_Fire::UDamageEffect_Fire()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag("DamageType.Fire"));

	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Damage.All"));
	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Damage.Fire"));

	Executions[0].CalculationClass = UDamageExecutionCalculation_Fire::StaticClass();
}

UDamageEffect_Ice::UDamageEffect_Ice()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag("DamageType.Ice"));

	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Damage.All"));
	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Damage.Ice"));

	Executions[0].CalculationClass = UDamageExecutionCalculation_Ice::StaticClass();
}

UDamageEffect_Electric::UDamageEffect_Electric()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag("DamageType.Electric"));

	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Damage.All"));
	ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Damage.Electric"));

	Executions[0].CalculationClass = UDamageExecutionCalculation_Electric::StaticClass();

}

FGameplayEffectSpecHandle UDamageEffect::CreateDamageEffectSpec(const FDamageDataStruct& InDamage, const FGameplayEffectContextHandle& ContextHandle)
{
	if(!ContextHandle.IsValid()) {return FGameplayEffectSpecHandle();}

	UAbilitySystemComponent* ASC = ContextHandle.GetInstigatorAbilitySystemComponent();

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(InDamage.DamageClass, 0, ContextHandle);
	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

	//Damage �ݿ�
	Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage.Absolute"), InDamage.Damage_Absolute);
	Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage.Relative"), InDamage.Damage_Relative);

	return SpecHandle;
}
