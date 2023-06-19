// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/InteractableNPC.h"
#include "Interaction/InteractionType/InteractionSystemComponent_NPC.h"
#include "Components/SphereComponent.h"
#include "DialoguerComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "ActionPortfolio.h"

AInteractableNPC::AInteractableNPC()
{
	InteractionSystem = CreateDefaultSubobject<UInteractionSystemComponent_NPC>("InteractionNPC");

	InteractionSensor = CreateDefaultSubobject<USphereComponent>("InteractionSesnsor");
	InteractionSensor->SetupAttachment(RootComponent);
	InteractionSensor->SetCollisionProfileName("InteractionSensor", false);
	InteractionSensor->SetEnableGravity(false);
	InteractionSensor->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	InteractionSensor->SetCanEverAffectNavigation(false);

	DialoguerComponet = CreateDefaultSubobject<UDialoguerComponent>("NPCDialoguer");
}

void AInteractableNPC::OnInteractionSensorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (!IsValid(PlayerCharacter) || !InteractionSystem->CanInteract(OtherActor)) return;

	PlayerCharacter->AddInteraction(InteractionSystem);
}

void AInteractableNPC::OnInteractionSensorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->RemoveInteraction(InteractionSystem);
}

UInteractionSystemComponent* AInteractableNPC::GetInteractionSystemComponent()
{
	return InteractionSystem;
}

void AInteractableNPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InteractionSensor->OnComponentBeginOverlap.AddDynamic(this, &AInteractableNPC::OnInteractionSensorBeginOverlap);
	InteractionSensor->OnComponentEndOverlap.AddDynamic(this, &AInteractableNPC::OnInteractionSensorEndOverlap);
}
