// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ActionPFAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UActionPFAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	TWeakObjectPtr<class AActionPortfolioCharacter> OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	float Degree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	bool bIsInAir;

public:
	virtual void NativeBeginPlay();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	AActionPortfolioCharacter* GetOwnerCharacter();
};
