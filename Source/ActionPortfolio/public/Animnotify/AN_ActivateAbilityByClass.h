// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_ActivateAbilityByClass.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UAN_ActivateAbilityByClass : public UAnimNotify
{
	GENERATED_BODY()
	UAN_ActivateAbilityByClass();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UActionPFGameplayAbility> AbilityClassToActivate;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
};
