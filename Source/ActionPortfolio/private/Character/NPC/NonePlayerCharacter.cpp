// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/NonePlayerCharacter.h"

// Sets default values
ANonePlayerCharacter::ANonePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANonePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANonePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANonePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANonePlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

