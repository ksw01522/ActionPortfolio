// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GameplayCueNotifyTypes.h"
#include "GameplayCueNotify_StateEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API AGameplayCueNotify_StateEffect : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
public:
	AGameplayCueNotify_StateEffect();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> EffectNiagara;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif // #if WITH_EDITOR

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify)
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify, Meta = (InlineEditConditionToggle))
	uint32 bOverrideLocation : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify, Meta = (InlineEditConditionToggle))
	uint32 bOverrideRotation : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify, Meta = (InlineEditConditionToggle))
	uint32 bOverrideScale : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify, Meta = (EditCondition = "bOverrideLocation"))
	FVector LocationOverride;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify, Meta = (EditCondition = "bOverrideRotation"))
	FRotator RotationOverride;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify, Meta = (EditCondition = "bOverrideScale"))
	FVector ScaleOverride;


protected:
	virtual bool Recycle() override;

	virtual void ReuseAfterRecycle() override;

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) override;

	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

	virtual bool WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
};
