// Fill out your copyright notice in the Description page of Project Settings.


#include "DialoguerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DialogueRuntime.h"
#include "DialogueBFL.h"
#include "DialogueManager.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UDialoguerComponent::UDialoguerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}



bool UDialoguerComponent::AllocateAnimInstance()
{
	if (AActor* OwnerActor = GetOwner(); IsValid(OwnerActor))
	{
		if (USkeletalMeshComponent* Skel = Cast<USkeletalMeshComponent>(OwnerActor->GetComponentByClass(USkeletalMeshComponent::StaticClass())))
		{
			DialoguerAnimInstance = Skel->GetAnimInstance();
		}
		else {
			LOG_ERROR(TEXT("Dialgouer's Owner doesn't have SkeletalMeshComponent : %s"), *OwnerActor->GetFName().ToString());
			return false;

		}
	}
	else
	{
		LOG_ERROR(TEXT("Dialoguer's Owner is not valid."));
		return false;

	}
	return true;
}

bool UDialoguerComponent::PlayAnimationMontage(UAnimMontage* ToPlayMontage, float PlayLate)
{
	if (!DialoguerAnimInstance.IsValid() && !AllocateAnimInstance()) {
		return false;
	}
	if (ToPlayMontage == nullptr)
	{
		LOG_ERROR(TEXT("To Play Montage is nullptr In Dialoguer %s"), *DialoguerID);
		return false;
	}

	DialoguerAnimInstance->Montage_Play(ToPlayMontage, PlayLate);

	return true;
}

// Called when the game starts
void UDialoguerComponent::BeginPlay()
{
	Super::BeginPlay();
	IDialoguerManagerInterface* DialoguerManager = UDialogueManager::GetDialoguerManager();
	
	DialoguerManager->RegisterDialoguer(this);
	AllocateAnimInstance();
}

void UDialoguerComponent::BeginDestroy()
{
	IDialoguerManagerInterface* DialoguerManager = UDialogueManager::GetDialoguerManager();

	if(DialoguerManager != nullptr) DialoguerManager->UnregisterDialoguer(this);

	Super::BeginDestroy();
}


// Called every frame
void UDialoguerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UDialoguerComponent::OnEnteredDialogue(const FDialogueHandle& Handle)
{
	
}




