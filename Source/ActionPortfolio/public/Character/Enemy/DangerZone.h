// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DangerZone.generated.h"

UCLASS()
class ACTIONPORTFOLIO_API ADangerZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADangerZone();

private:
	static float DefaultZoneScaleRadiusRate;

	UPROPERTY()
	class USceneComponent* PivotComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DangerZone", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SideMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DangerZone", meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* Decal;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "DangerZone")
	void SetDangerZoneRadius(float NewRadius);
};
