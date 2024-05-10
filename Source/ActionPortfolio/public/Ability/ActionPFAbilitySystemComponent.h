// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ActionPFAbilitySystemComponent.generated.h"

/**
 * 
 */
class UActionPFGameplayAbility;

DECLARE_MULTICAST_DELEGATE_OneParam(FGenericEventInputDelegate, int32);

UCLASS()
class ACTIONPORTFOLIO_API UActionPFAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UActionPFAbilitySystemComponent();

public:
	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

private:
	TMap<int32, FGenericEventInputDelegate> GenericEventInputDelegate;

protected:
	bool IsGenericEventInputBound(int32 InputID) const;
	

public:
	void ClearAbilityWithClass(TSubclassOf<class UGameplayAbility> InAbilityClass);

	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UActionPFGameplayAbility*>& ActiveAbilities);
	bool IsActingAbilityByClass(TSubclassOf<UGameplayAbility> CheckAbility);

	static UActionPFAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

	bool GetCooldownRemainingAndDurationByTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration) const;

	bool CanActivateAbility(TSubclassOf<UActionPFGameplayAbility> AbilityClass);


	virtual void AbilityLocalInputPressed(int32 InputID) override;
	virtual void AbilityLocalInputReleased(int32 InputID) override;

	FGenericEventInputDelegate& GetGenericEventInputDelegate(int32 InputID);
};
