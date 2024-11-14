// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "Ability/Effects/Damage/DamageEffect.h"
#include "ActionPFAbilityBFL.generated.h"

/**
 * 
 */
 class UActionPFAbilitySystemComponent;

UCLASS()
class ACTIONPORTFOLIO_API UActionPFAbilityBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Ability|ActionPF")
	static FGameplayEventData MakeEventDataForAbility(AActor* instigator, TArray<AActor*> TargetArray);

	UFUNCTION(BlueprintPure, Category = "Ability|ActionPF")
	static UActionPFAbilitySystemComponent* GetAbilitySystemComponent(AActor* Target);
};
