// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "AnimNotifyState_GrantTag.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UAnimNotifyState_GrantTag : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	//virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FString GetNotifyName_Implementation() const;


private:
	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowprivateAccess = "true"))
	FGameplayTag GrantTag;

	TWeakObjectPtr<class UActionPFAbilitySystemComponent> OwnerAbilitySystem;
};
