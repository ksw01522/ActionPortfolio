// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectExtension.h"
#include "ActionPFPlayerState.generated.h"


UCLASS()
class ACTIONPORTFOLIO_API AActionPFPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AActionPFPlayerState();

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<class UActionPFAbilitySystemComponent> WeakASC;

	UPROPERTY()
	TObjectPtr<class UPlayerAttributeSet> PlayerAttributeSet;

	float GetBeginLevel() const;
	float GetBeginXP() const;

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

public:
	void PlayerLevelUp(float OldLv, float NewLv);

	void SetAbilitySystemComponent(UActionPFAbilitySystemComponent* InASC);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void OnChangeGoldByEffect(const FGameplayEffectModCallbackData& Data);
};
