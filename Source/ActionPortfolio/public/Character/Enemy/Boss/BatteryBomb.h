// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BatteryBomb.generated.h"

UCLASS()
class ACTIONPORTFOLIO_API ABatteryBomb : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> BatteryMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCapsuleComponent> Collider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bomb", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystem> BombParticle;

public:	
	// Sets default values for this actor's properties
	ABatteryBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SpawnBombParticle();
};
