// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Boss/BatteryBomb.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABatteryBomb::ABatteryBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BatteryMesh = CreateDefaultSubobject<UStaticMeshComponent>("BatteryMesh");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BATTERYMESH(TEXT("/Game/ModSci_Engineer/Meshes/SM_ElectricDistribBox.SM_ElectricDistribBox"));
	if (BATTERYMESH.Succeeded())
	{
		BatteryMesh->SetStaticMesh(BATTERYMESH.Object);
	}

	Collider = CreateDefaultSubobject<UCapsuleComponent>("Collider");

	static ConstructorHelpers::FClassFinder<UParticleSystem> BOMBPARTICLE(TEXT("/Game/ActionPFCharacter/Enemies/Boss/FX/P_Boss_BatteryBomb.P_Boss_BatteryBomb_C"));
	if (BOMBPARTICLE.Succeeded())
	{
		BombParticle = BOMBPARTICLE.Class;
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

