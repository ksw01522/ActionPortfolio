// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "ANS_AttackByTag.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UANS_AttackByTag : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	//virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FString GetNotifyName_Implementation() const;


private:
	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowprivateAccess = "true"))
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowprivateAccess = "true"))
	FName AttackTag;

	TWeakObjectPtr<class UShapeComponent> AttackShape;
	TWeakObjectPtr<class UActionPFAbilitySystemComponent> OwnerAbilitySystem;
	UPROPERTY()
	TArray<TWeakObjectPtr<class AActor>> OverlappedActors;
private:
	UFUNCTION()
	void OnAttackShapeOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
