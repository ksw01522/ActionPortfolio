// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GAEffectActor.h"
#include "GAEffectBoxActor.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API AGAEffectBoxActor : public AGAEffectActor
{
	GENERATED_BODY()
	AGAEffectBoxActor();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EffectCollider", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<class UArrowComponent> ArrowComponent;
#endif

	TArray<TWeakObjectPtr<AActor>> OverlappedActors; 
protected:
	virtual void BeginPlay() override;


protected:
	UFUNCTION()
	void BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
