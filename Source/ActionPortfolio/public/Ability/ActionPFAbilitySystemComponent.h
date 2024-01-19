// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ActionPFAbilitySystemComponent.generated.h"

/**
 * 
 */
class UActionPFGameplayAbility;



UCLASS()
class ACTIONPORTFOLIO_API UActionPFAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UActionPFAbilitySystemComponent();

public:
	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;
public:

	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UActionPFGameplayAbility*>& ActiveAbilities);
	bool IsActingAbilityByClass(TSubclassOf<UGameplayAbility> CheckAbility);

	bool TryActivatePFAbilityByClass(TSubclassOf<class UActionPFGameplayAbility> AbilityClass);

	static UActionPFAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

	bool GetCooldownRemainingAndDurationByTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);

	bool CanActivateAbility(TSubclassOf<UActionPFGameplayAbility> AbilityClass);

};
