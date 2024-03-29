// Fill out your copyright notice in the Description page of Project Settings.


#include "DialoguerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DialogueHeaders.h"
#include "DialogueRuntime.h"
#include "DialogueBFL.h"

// Sets default values for this component's properties
UDialoguerComponent::UDialoguerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}



// Called when the game starts
void UDialoguerComponent::BeginPlay()
{
	Super::BeginPlay();

	UDialogueManager* DialogueManager = UDialogueBFL::GetDialogueManager();
	if (IsValid(DialogueManager)) {
		DialogueManager->RegisterDialoguer(this);
	}
}

void UDialoguerComponent::BeginDestroy()
{
	Super::BeginDestroy();

	UDialogueManager* DialogueManager = UDialogueBFL::GetDialogueManager();
	if (IsValid(DialogueManager)) {
		DialogueManager->UnregisterDialoguer(this);
	}
}


// Called every frame
void UDialoguerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UDialoguerComponent::OnEnteredDialogue(const FActingDialogueHandle& Handle)
{
	DialogueHandle = Handle;
}

bool UDialoguerComponent::IsInDialogue()
{
	if(!DialogueHandle.IsValid()) return false;

	UDialogueManager* DialogueMag = UDialogueBFL::GetDialogueManager();
	if(!IsValid(DialogueMag)) return false;

	return DialogueMag->CheckDialogueFromHandle(DialogueHandle);

	
}



