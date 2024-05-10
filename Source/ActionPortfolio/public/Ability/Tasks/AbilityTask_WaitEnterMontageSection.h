// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "AbilityTask_WaitEnterMontageSection.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnterSectionDelegate, UAnimMontage*, WaitMontage, const FName&, InSectionName);

/**
 * 
 */
 

UCLASS()
class ACTIONPORTFOLIO_API UAbilityTask_WaitEnterMontageSection : public UAbilityTask
{
	GENERATED_BODY()

	
	
public:
	UAbilityTask_WaitEnterMontageSection(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY()
	TObjectPtr<UAnimMontage> WaitMontage;

	FName WaitSectionName;

	TWeakObjectPtr<class UAnimInstance> AnimInstance;

public:
	FOnEnterSectionDelegate OnEnterSection;

private:
	virtual void Activate() override;

	virtual void ExternalCancel() override;

	virtual FString GetDebugString() const override;

	void OnEnterSectionCallBack();

	bool CanActivateWaitSection(TArray<FString>& Errors);

protected:
	virtual void OnDestroy(bool AbilityEnded) override;

	virtual void TickTask(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Use Three Divisions Montage Section",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitEnterMontageSection* WaitEnterMontageSection(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* InWaitMontage, const FName& WaitSectionName);
};
