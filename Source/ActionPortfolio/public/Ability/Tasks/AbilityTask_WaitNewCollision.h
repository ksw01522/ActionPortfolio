// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitNewCollision.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitNewCollisionDelegate, const FGameplayAbilityTargetDataHandle&, TargetData);
/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UAbilityTask_WaitNewCollision : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAbilityTask_WaitNewCollision(const FObjectInitializer& ObjectInitializer);
private:
	UPROPERTY(Transient)
	TObjectPtr<class UPrimitiveComponent> CollisionShape = nullptr;

	TArray<class AActor*> AlreadyOverlapActors;

	float LifeTime = 0;
	bool bRemoveAfterLifeTime = false;

	bool bActEventOnce = false;

	ECollisionEnabled::Type CollisionEnabled;

	virtual void TickTask(float DeltaTime) override;

public:
	FWaitNewCollisionDelegate OnOverlapDel;
	
public:
	void MakeCapsuleCollision(float Radius, float HalfHeight, const FName& InCollisionProfileName = NAME_None);
	void MakeBoxCollision(FVector InBoxExtent, const FName& InCollisionProfileName = NAME_None);
	void MakeSphereCollision(float Radius, const FName& InCollisionProfileName = NAME_None);
	void SetCustomCollision(UPrimitiveComponent* InCustomCollision) { CollisionShape = InCustomCollision; }

	void SetShapeTransform(const FTransform& InTransform);

	void SetCollisionProfileName(const FName& ProfileName, bool bUpdateOverlap = false);

	void AttachToAvatar(const FAttachmentTransformRules& AttachmentRules, const FName& SocketName);

	void AddIgnoreActor(AActor* InActor);

private:
	UFUNCTION()
	void OnTaskBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SetCollisionLifeTime(float InLifeTime) { LifeTime = InLifeTime; }

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitNewCollision* WaitNewCollision(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		bool bActEventOnce,
		UPrimitiveComponent* CustomCollision = nullptr);
};
