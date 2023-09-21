// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ActionPFAnimInstance.h"
#include "Character/ActionPortfolioCharacter.h"
#include "ActionPortfolio.h"
#include "GameFramework/CharacterMovementComponent.h"

bool UActionPFAnimInstance::IsRigidity()
{
	AActionPortfolioCharacter* Character = GetOwnerCharacter();

	if(Character == nullptr) return false;

	return Character->IsRigidity();
} 

void UActionPFAnimInstance::NativeBeginPlay()
{
	if (!OwnerCharacter.IsValid()) {
		OwnerCharacter = Cast<AActionPortfolioCharacter>(GetOwningActor());
	}


}

void UActionPFAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!GetOwnerCharacter()) return;

	FVector Vel = OwnerCharacter->GetVelocity();
	Speed = Vel.Size();

	bIsInAir = OwnerCharacter->GetCharacterMovement()->IsFalling();

	if (FMath::IsNearlyZero(Speed)) {
		Degree = 0;
	}
	else
	{
		FVector Forward = OwnerCharacter->GetActorForwardVector();
		Forward.Z = 0;
		Forward.Normalize();
		Vel.Normalize();
		
		double DotProd = FVector::DotProduct(Vel, Forward);
		Degree = FMath::RadiansToDegrees( FMath::Acos(DotProd));
		FVector Cross = FVector::CrossProduct(Forward, Vel);

		double temp = FVector::DotProduct(Cross, FVector::UpVector);
		if (temp < 0) Degree *= -1;
	}
}

AActionPortfolioCharacter* UActionPFAnimInstance::GetOwnerCharacter()
{
	if (!OwnerCharacter.IsValid()) {
		OwnerCharacter = Cast<AActionPortfolioCharacter>(GetOwningActor());
	}

	return OwnerCharacter.Get();
}
