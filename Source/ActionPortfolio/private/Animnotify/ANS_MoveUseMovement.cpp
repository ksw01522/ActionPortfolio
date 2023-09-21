// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/ANS_MoveUseMovement.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/ActionPortfolioCharacter.h"

void UANS_MoveUseMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActionPortfolioCharacter* Char = MeshComp->GetOwner<AActionPortfolioCharacter>();
	if(!IsValid(Char)) return;

	FinalDirection = MoveDirection;
	FinalDirection.Z = 0;

	FinalDirection.Normalize();

	FinalDirection = FinalDirection.X * Char->GetActorForwardVector() + FinalDirection.Y * Char->GetActorRightVector();

	UCharacterMovementComponent* MovementComponent = Char->GetCharacterMovement();
	if (IsValid(MovementComponent)) {
		MovementComponent->Velocity = FinalDirection * MoveSpeed;
		MovementComponent->MaxWalkSpeed = MoveSpeed;
	}
}

void UANS_MoveUseMovement::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	AActionPortfolioCharacter* Char = MeshComp->GetOwner<AActionPortfolioCharacter>();
	if (!IsValid(Char)) return;

	UCharacterMovementComponent* MovementComponent = Char->GetCharacterMovement();
	if (!IsValid(MovementComponent)) {
		return;
	}

	MovementComponent->AddInputVector(FinalDirection, false);
}

void UANS_MoveUseMovement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AActionPortfolioCharacter* Char = MeshComp->GetOwner<AActionPortfolioCharacter>()) {
		Char->ResetMovement();
		Char->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}
}
