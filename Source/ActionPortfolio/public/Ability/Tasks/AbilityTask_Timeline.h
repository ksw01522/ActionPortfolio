// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Components/TimelineComponent.h"
#include "AbilityTask_Timeline.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FATTimelineFloatDelegate, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FATTimelineEndDelegate);

UCLASS()
class ACTIONPORTFOLIO_API UAbilityTask_Timeline : public UAbilityTask
{
	GENERATED_BODY()
	
private:
	FTimeline Timeline;
	TObjectPtr<UCurveFloat> CurveFloat;

private:
	void CallFloatEvent(float Value);
	void CallTimelineEndEvent();

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_Timeline* PlayTimeline(
												UGameplayAbility* OwningAbility,
												FName TaskInstanceName
												);

	virtual void Activate() override;
	virtual void ExternalCancel() override;
	//virtual FString GetDebugString() const override;
	virtual void OnDestroy(bool AbilityEnded) override;


	UPROPERTY(BlueprintAssignable)
	FATTimelineFloatDelegate OnFloatEvent;

	UPROPERTY(BlueprintAssignable)
	FATTimelineEndDelegate OnEndEvent;
};
