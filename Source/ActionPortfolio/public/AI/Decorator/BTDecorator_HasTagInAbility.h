// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_HasTagInAbility.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UBTDecorator_HasTagInAbility : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_HasTagInAbility();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer TagsToCheck;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag", meta = (AllowPrivateAccess = "true"))
	bool bWantMatchAllTags;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	virtual FString GetStaticDescription() const override;
};
