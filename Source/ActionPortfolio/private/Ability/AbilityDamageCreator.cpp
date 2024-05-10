// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilityDamageCreator.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "ActionPortfolio.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Engine/Texture.h"
#include "Ability/Widget/SAbilityIcon.h"
#include "Ability/Effects/DamageExecutionCalculation.h"

UAbilityDamageCreator::UAbilityDamageCreator()
{
	DamageEffectClass = UGameplayEffect::StaticClass();
}

#if WITH_EDITOR
void UAbilityDamageCreator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == "DamageTypeTagContainer")
	{	
		bCanVisibleMeelee = DamageTypeTagContainer.HasTagExact(FGameplayTag::RequestGameplayTag("DamageType.Meelee"));
		bCanVisibleFire = DamageTypeTagContainer.HasTagExact(FGameplayTag::RequestGameplayTag("DamageType.Fire"));
		bCanVisibleIce = DamageTypeTagContainer.HasTagExact(FGameplayTag::RequestGameplayTag("DamageType.Ice"));
		bCanVisibleElectric = DamageTypeTagContainer.HasTagExact(FGameplayTag::RequestGameplayTag("DamageType.Electric"));
	}
}
#endif

UGameplayEffect* UAbilityDamageCreator::CreateDamageEffect(UAbilitySystemComponent* SourceASC)
{
	UGameplayEffect* DamageEffect = NewObject<UGameplayEffect>(GetTransientPackage(), DamageEffectClass);

	FGameplayEffectExecutionDefinition DamageExecution;
	DamageExecution.CalculationClass = UDamageExecutionCalculation::StaticClass();

	FGameplayTag MeeleeTag = FGameplayTag::RequestGameplayTag("DamageType.Meelee");
	FGameplayTag FireTag = FGameplayTag::RequestGameplayTag("DamageType.Fire");
	FGameplayTag IceTag = FGameplayTag::RequestGameplayTag("DamageType.Ice");
	FGameplayTag ElectricTag = FGameplayTag::RequestGameplayTag("DamageType.Electric");

	if (DamageTypeTagContainer.HasTagExact(MeeleeTag))
	{
		FGameplayEffectExecutionScopedModifierInfo DEModifierInfo(MeeleeTag);
		DEModifierInfo.ModifierOp = EGameplayModOp::Override;
		DEModifierInfo.ModifierMagnitude = MeeleeDamage;
		DamageExecution.CalculationModifiers.Add(DEModifierInfo);
	}


	if (DamageTypeTagContainer.HasTagExact(FireTag))
	{
		FGameplayEffectExecutionScopedModifierInfo DEModifierInfo(FireTag);
		DEModifierInfo.ModifierOp = EGameplayModOp::Override;
		DEModifierInfo.ModifierMagnitude = FireDamage;
		DamageExecution.CalculationModifiers.Add(DEModifierInfo);
	}

	if (DamageTypeTagContainer.HasTagExact(IceTag))
	{
		FGameplayEffectExecutionScopedModifierInfo DEModifierInfo(IceTag);
		DEModifierInfo.ModifierOp = EGameplayModOp::Override;
		DEModifierInfo.ModifierMagnitude = IceDamage;
		DamageExecution.CalculationModifiers.Add(DEModifierInfo);
	}

	if (DamageTypeTagContainer.HasTagExact(ElectricTag))
	{
		FGameplayEffectExecutionScopedModifierInfo DEModifierInfo(ElectricTag);
		DEModifierInfo.ModifierOp = EGameplayModOp::Override;
		DEModifierInfo.ModifierMagnitude = ElectricDamage;
		DamageExecution.CalculationModifiers.Add(DEModifierInfo);
	}

	DamageEffect->Executions.Add(DamageExecution);

	if (SourceASC != nullptr)
	{
		FGameplayEventData EventData;
		EventData.OptionalObject = DamageEffect;
		SourceASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("CommonEvent.OnDamageCreate"), &EventData);
	}


	for (auto& AddedExecution : DamageEffect->Executions)
	{
		if (AddedExecution.CalculationClass != UDamageExecutionCalculation::StaticClass()) continue;

		for (auto& Damages : AddedExecution.CalculationModifiers)
		{
			FName CueTagName(FString::Printf(TEXT("GameplayCue.%s"), *Damages.TransientAggregatorIdentifier.GetTagName().ToString()));
			FGameplayTag CueTag = FGameplayTag::RequestGameplayTag(CueTagName, false);
			if (CueTag.IsValid()) DamageEffect->GameplayCues.Add(FGameplayEffectCue(CueTag, 0, 0));
		}
	}

		
	

	DamageEffect->ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
	DamageEffect->ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Immune.Damage"));

	return DamageEffect;
}
