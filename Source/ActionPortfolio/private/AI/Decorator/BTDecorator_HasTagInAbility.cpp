// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_HasTagInAbility.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Character/EnemyAIController.h"

UBTDecorator_HasTagInAbility::UBTDecorator_HasTagInAbility()
{
	NodeName = "Has Tag In AbilitySystem";
}

bool UBTDecorator_HasTagInAbility::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AActionPortfolioCharacter* Character = OwnerComp.GetAIOwner()->GetPawn<AActionPortfolioCharacter>();

	if (bWantMatchAllTags){
		return Character->GetAbilitySystemComponent()->HasAllMatchingGameplayTags(TagsToCheck);
	}
	else{
		return Character->GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(TagsToCheck);
	}
}

FString UBTDecorator_HasTagInAbility::GetStaticDescription() const
{
	if (IsInversed())
	{
		if (bWantMatchAllTags)
		{
			return "Has All GameplayTags?";
		}
		else
		{
			return "Has Any GameplayTags?";
		}

	}
	else
	{
		if (bWantMatchAllTags)
		{
			return "Not Has All GameplayTags?";
		}
		else
		{
			return "Not Has Any GameplayTags?";
		}
	}

}
