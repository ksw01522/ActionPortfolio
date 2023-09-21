// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DangerZone.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"

float ADangerZone::DefaultZoneScaleRadiusRate = 0.02;

// Sets default values
ADangerZone::ADangerZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PivotComponent = CreateDefaultSubobject<USceneComponent>("Pivot");
	RootComponent = PivotComponent;

	SideMesh = CreateDefaultSubobject<UStaticMeshComponent>("SideMesh");
	SideMesh->SetupAttachment(RootComponent);
	SideMesh->SetRelativeLocation(FVector(0,0,5));
	SideMesh->SetRelativeScale3D(FVector(2,2,0.5));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SIDEMESH(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (SIDEMESH.Succeeded())
	{
		SideMesh->SetStaticMesh(SIDEMESH.Object);
		
		static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_SIDEMESH(TEXT("/Game/ActionPFCharacter/Enemies/Mat_DangerZone_Inst.Mat_DangerZone_Inst"));
		if (MAT_SIDEMESH.Succeeded())
		{
			SideMesh->SetMaterial(0, MAT_SIDEMESH.Object);
		}
	}
	SideMesh->SetCollisionProfileName("NoCollision", false);
	SideMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	Decal->SetupAttachment(RootComponent);
	Decal->DecalSize = FVector(20,100,100);
	Decal->SetRelativeRotation(FRotator(90, 0, 0));
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_DECAL(TEXT("/Game/ActionPFCharacter/Enemies/Mat_DangerZoneDecal_Inst.Mat_DangerZoneDecal_Inst"));
	if (MAT_DECAL.Succeeded())
	{
		Decal->SetMaterial(0, MAT_DECAL.Object);
	}

}

// Called when the game starts or when spawned
void ADangerZone::BeginPlay()
{

	Super::BeginPlay();
	
}

// Called every frame
void ADangerZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADangerZone::SetDangerZoneRadius(float NewRadius)
{
	float NewRatedRadius = NewRadius* DefaultZoneScaleRadiusRate;
	
	SideMesh->SetRelativeScale3D(FVector(NewRatedRadius, NewRatedRadius, SideMesh->GetRelativeScale3D().Z));
	Decal->DecalSize = FVector(Decal->DecalSize.X, NewRadius, NewRadius);
	Decal->MarkRenderStateDirty();
}

