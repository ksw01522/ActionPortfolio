// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayTagContainer.h"
#include "AbilityTask_GrantTags.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UAbilityTask_GrantTags : public UAbilityTask
{
	GENERATED_BODY()
	
private:
	FGameplayTagContainer TagsForGrant;

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityEnding) override;

public:
	static UAbilityTask_GrantTags* GrantTags(UGameplayAbility* OwningAbility, FName TaskName, FGameplayTagContainer GrantTags);
};
