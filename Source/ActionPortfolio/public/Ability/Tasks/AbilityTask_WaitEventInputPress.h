// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitEventInputPress.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FWaitEventInputPressDelegate, int32);

UCLASS()
class ACTIONPORTFOLIO_API UAbilityTask_WaitEventInputPress : public UAbilityTask
{
	GENERATED_BODY()
public:

private:
	int32 InputID;

	FDelegateHandle EventHandle;

	bool bActivateOnce;

public:
	FWaitEventInputPressDelegate OnPressDel;

private:
	void OnPressCallBack(int32 InInputID);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityEnding) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitEventInputPress* WaitEventInputPress(UGameplayAbility* OwningAbility, FName TaskInstanceName, int32 InputID, bool bActivateOnce);
};
