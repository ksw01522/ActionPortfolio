// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityChainGraphFactory.h"
#include "AbilityChainGraph.h"

UAbilityChainGraphFactory::UAbilityChainGraphFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UAbilityChainGraph::StaticClass();
}

UObject* UAbilityChainGraphFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UAbilityChainGraph>(InParent, Class, Name, Flags | RF_Transactional);
}

FText UAbilityChainGraphFactory::GetDisplayName() const
{
	return NSLOCTEXT("AbilityChainEditor", "AbilityChain Graph Factory Name", "어빌리티 체인");
}

FString UAbilityChainGraphFactory::GetDefaultNewAssetName() const
{
	return TEXT("Ability Chain");
}
