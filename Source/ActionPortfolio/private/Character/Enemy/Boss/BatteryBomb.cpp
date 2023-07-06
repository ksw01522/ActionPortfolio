// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Boss/BatteryBomb.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ABatteryBomb::ABatteryBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UCapsuleComponent>("Collider");
	RootComponent = Collider;
	Collider->SetEnableGravity(false);
	Collider->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Collider->SetCapsuleHalfHeight(156);
	Collider->SetCapsuleRadius(100);

	BatteryMesh = CreateDefaultSubobject<UStaticMeshComponent>("BatteryMesh");
	BatteryMesh->SetupAttachment(Collider);
	BatteryMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	BatteryMesh->SetRelativeScale3D(FVector(3,3,3));
	BatteryMesh->SetRelativeLocation(FVector(30, 0, 0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BATTERYMESH(TEXT("/Game/ModSci_Engineer/Meshes/SM_ElectricDistribBox.SM_ElectricDistribBox"));
	if (BATTERYMESH.Succeeded())
	{
		BatteryMesh->SetStaticMesh(BATTERYMESH.Object);
	}


	static ConstructorHelpers::FObjectFinder<UParticleSystem> BOMBPARTICLE(TEXT("/Game/ActionPFCharacter/Enemies/Boss/FX/P_Boss_BatteryBomb.P_Boss_BatteryBomb"));
	if (BOMBPARTICLE.Succeeded())
	{
		BombParticle = BOMBPARTICLE.Object;
	}
}

// Called when the game starts or when spawned
void ABatteryBomb::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABatteryBomb::SpawnBombParticle()
{	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BombParticle, GetActorTransform());
}

