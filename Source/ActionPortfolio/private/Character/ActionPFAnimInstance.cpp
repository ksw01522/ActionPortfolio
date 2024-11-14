// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ActionPFAnimInstance.h"
#include "Character/ActionPortfolioCharacter.h"
#include "ActionPortfolio.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Ability/ActionPFAbilitySystemComponent.h"

void UActionPFAnimInstance::NativeBeginPlay()
{



}

void UActionPFAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	AActionPortfolioCharacter* PFCharacter = Cast<AActionPortfolioCharacter>(GetOwningActor());
	if(PFCharacter == nullptr) return;

	UActionPFAbilitySystemComponent* ASC = StaticCast<UActionPFAbilitySystemComponent*>(PFCharacter->GetAbilitySystemComponent());
	TagsInASC.Reset();
	ASC->GetOwnedGameplayTags(TagsInASC);
	
	FVector Vel = PFCharacter->GetVelocity();
	Speed = Vel.Size();

	if (FMath::IsNearlyZero(Speed)) {
		Degree = 0;
	}
	else
	{
		FVector Forward = PFCharacter->GetActorForwardVector();
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

UAnimMontage* UActionPFAnimInstance::GetAnimMontageByTag(const FGameplayTag& InTag)
{
	UAnimMontage* ReturnAnim = AnimMontageMap.FindRef(InTag);

	if (ReturnAnim == nullptr)
	{
		PFLOG(Warning, TEXT("%s don't have %s Tag Rigidity Anim"), *GetOwningActor()->GetName(), *InTag.ToString());
	}

	return ReturnAnim;
}

UAnimMontage* UActionPFAnimInstance::GetAnimMontageByTag(FName InTag)
{
	return GetAnimMontageByTag(FGameplayTag::RequestGameplayTag(InTag, true));
}
