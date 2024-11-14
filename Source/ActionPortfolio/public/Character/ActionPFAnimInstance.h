// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "ActionPFAnimInstance.generated.h"

/**
 * 
 */

UCLASS()
class ACTIONPORTFOLIO_API UActionPFAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float Degree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer TagsInASC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> AnimMontageMap;

public:
	virtual void NativeBeginPlay();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UAnimMontage* GetAnimMontageByTag(const FGameplayTag& InTag);
	UAnimMontage* GetAnimMontageByTag(FName InTag);

};
