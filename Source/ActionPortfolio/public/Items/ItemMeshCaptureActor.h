// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemMeshCaptureActor.generated.h"

UCLASS()
class ACTIONPORTFOLIO_API AItemMeshCaptureActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemMeshCaptureActor();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBillboardComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed;

	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneCaptureComponent2D> SceneCapture2D;

	TSharedPtr<struct FStreamableHandle> MeshStreamingHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void OnCompleteLoadMesh(TSoftObjectPtr<class UStaticMesh> NewItemMesh);

public:
	void SetItemMesh(TSoftObjectPtr<class UStaticMesh> NewItemMesh);
};
