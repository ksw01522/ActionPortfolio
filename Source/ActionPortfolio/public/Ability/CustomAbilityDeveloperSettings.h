// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "CustomAbilityDeveloperSettings.generated.h"

/**
 * 
 */

UCLASS(Config = Game, defaultconfig)
class ACTIONPORTFOLIO_API UCustomAbilityDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, GlobalConfig)
	TMap<FGameplayTag, TSoftObjectPtr<class UMaterialInterface>> IconSoftMap;

public:
	TMap<FGameplayTag, TSoftObjectPtr<class UMaterialInterface>> GetIconSoftMap() const { return IconSoftMap; }
};
