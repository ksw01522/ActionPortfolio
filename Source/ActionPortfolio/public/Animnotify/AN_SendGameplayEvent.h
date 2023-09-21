// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AN_SendGameplayEvent.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Send Gameplay Event"))
class ACTIONPORTFOLIO_API UAN_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

	UAN_SendGameplayEvent();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEvent", meta = (AllowPrivateAccess = "true"))
	FGameplayTag EventTag;

	UPROPERTY()
	FString CachedNotifyName;

#if WITH_EDITOR
	void BuildNotifyName();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

	FString GetNotifyName_Implementation() const;
};
