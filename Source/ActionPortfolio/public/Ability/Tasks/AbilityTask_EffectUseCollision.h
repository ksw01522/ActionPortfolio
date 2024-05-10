// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_EffectUseCollision.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnAbilityTaskEnd, bool);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FOnEffectShapeBeginOverlapDelegate, const FName&, InTaskInstanceName, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult&, SweepResult);


UCLASS()
class ACTIONPORTFOLIO_API UAbilityTask_EffectUseCollision : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UAbilityTask_EffectUseCollision(const FObjectInitializer& ObjectInitializer);
private:
	UPROPERTY(Transient)
	TObjectPtr<class UPrimitiveComponent> EffectShape = nullptr;

	TArray<FGameplayEffectSpecHandle> EffectSpecHandles;

	TArray<class UAbilitySystemComponent*> AlreadyEffectedSystems;

	FOnAbilityTaskEnd OnTaskEnd;

	float LifeTime = 0;
	bool bRemoveAfterLifeTime = false;

	virtual void TickTask(float DeltaTime) override;

// Effect Shape 관련
public:
	void MakeCapsuleCollision(float Radius, float HalfHeight, const FName& InCollisionProfileName = NAME_None, const FName& ShapeName = NAME_None);
	void MakeBoxCollision(FVector InBoxExtent, const FName& InCollisionProfileName = NAME_None, const FName& ShapeName = NAME_None);
	void MakeSphereCollision( float Radius, const FName& InCollisionProfileName = NAME_None, const FName& ShapeName = NAME_None);
	void SetCustomCollision(UPrimitiveComponent* InCustomCollision) { EffectShape = InCustomCollision;}

	void SetShapeTransform(const FTransform& InTransform);

	void SetCollisionProfileName(const FName& ProfileName, bool bUpdateOverlap = false);

	void AttachToAvatar(const FAttachmentTransformRules& AttachmentRules, const FName& SocketName);

	FOnEffectShapeBeginOverlapDelegate OnEffectShapeBeginOverlapEvent;

// Gameplay Effect 관련
public:
	void AddGameplayEffect(TSubclassOf<UGameplayEffect> InEffectClass);
	void AddGameplayEffects(TArray<TSubclassOf<UGameplayEffect>> InEffectClasses);
	
	void AddGameplayEffect(UGameplayEffect* InEffect);
	void AddGameplayEffects(TArray<UGameplayEffect*> InEffects);

	void SetOnAbilityTaskEndEvent(const FOnAbilityTaskEnd& InEvent) {OnTaskEnd = InEvent;}

private:
	UFUNCTION()
	void OnEffectShapeBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_EffectUseCollision* EffectUseCollision(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		TArray<TSubclassOf<UGameplayEffect>> EffectClasses,
		TArray<UGameplayEffect*> Effects);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SetEffectLifeTime(float InLifeTime) {LifeTime = InLifeTime;}
};
