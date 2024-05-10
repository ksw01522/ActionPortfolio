// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/GameplayAbility_Meelee.h"
#include "Ability_Meelee_Chain.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAbilityChainNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 InputKey;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UActionPFGameplayAbility> AbilityClass;
};


UCLASS()
class ACTIONPORTFOLIO_API UAbility_Meelee_Chain : public UGameplayAbility_Meelee
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Meelee", meta = (AllowPrivateAccess = "true"))
	TArray<FAbilityChainNode> ChainedAbilityNodes;

protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnEventInput(int32 InputKey);

public:
	FAbilityChainNode* FindChainNode(int32 InputKey) const;

#if WITH_EDITOR
protected:
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;

	virtual void PostCDOCompiled(const FPostCDOCompiledContext& Context) override;
#endif
};
