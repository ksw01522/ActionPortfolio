// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effects/GameplayEffect_Damage.h"
#include "Ability/Effects/DamageExecutionCalculation.h"
#include "Ability/Effects/RigidityExecutionCalculation.h"

UGameplayEffect_Damage::UGameplayEffect_Damage()
{
	FGameplayEffectExecutionDefinition DamageExecution;
	DamageExecution.CalculationClass = UDamageExecutionCalculation::StaticClass();
	FGameplayEffectExecutionScopedModifierInfo DEModifierInfo(FGameplayTag::RequestGameplayTag("Data.Damage"));
	DEModifierInfo.ModifierOp = EGameplayModOp::Override;
	DEModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(0.0f);
	DamageExecution.CalculationModifiers.Add(DEModifierInfo);
	Executions.Add(DamageExecution);

	DEModifierInfo.TargetTags.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Damage"));

	FGameplayEffectExecutionDefinition RigidityExecution;
	RigidityExecution.CalculationClass = URigidityExecutionCalculation::StaticClass();
	FGameplayEffectExecutionScopedModifierInfo REModifierInfo(FGameplayTag::RequestGameplayTag("Data.RigidityTime"));
	REModifierInfo.ModifierOp = EGameplayModOp::Override;
	RigidityExecution.CalculationModifiers.Add(REModifierInfo);
	Executions.Add(RigidityExecution);
}
