// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemMeshCaptureActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Components/BillboardComponent.h"

// Sets default values
AItemMeshCaptureActor::AItemMeshCaptureActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBillboardComponent>("Root");
	SetRootComponent(Root);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	ItemMesh->SetupAttachment(RootComponent);

	SceneCapture2D = CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCapture2D");
	SceneCapture2D->ShowOnlyComponent(ItemMesh);
	SceneCapture2D->bCaptureOnMovement = false;
	SceneCapture2D->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AItemMeshCaptureActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemMeshCaptureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ItemMesh->AddRelativeRotation(FRotator(0, RotateSpeed * DeltaTime, 0));
}

void AItemMeshCaptureActor::SetItemMesh(TSoftObjectPtr<class UStaticMesh> NewItemMesh)
{
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	if (MeshStreamingHandle.IsValid())
	{
		MeshStreamingHandle->CancelHandle();
		MeshStreamingHandle.Reset();
	}

	if (NewItemMesh.IsNull())
	{
		ItemMesh->SetStaticMesh(nullptr);
	}
	else if(NewItemMesh.IsValid())
	{
		OnCompleteLoadMesh(NewItemMesh);
	}
	else
	{
		ItemMesh->SetStaticMesh(nullptr);

		MeshStreamingHandle = Streamable.RequestAsyncLoad(NewItemMesh.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AItemMeshCaptureActor::OnCompleteLoadMesh, NewItemMesh));
	}
}

void AItemMeshCaptureActor::OnCompleteLoadMesh(TSoftObjectPtr<class UStaticMesh> NewItemMesh)
{
	ItemMesh->SetStaticMesh(NewItemMesh.Get());
}

