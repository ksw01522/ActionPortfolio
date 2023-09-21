// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Widget_AbilityNode.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Components/Textblock.h"

#include "ActionPortfolio.h"


void UWidget_AbilityNode::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_AbilityNode::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!bIsAllocatedAbility || !AbilitySystem.IsValid()) return;

	float CoolDuration = -1;
	float CoolRemaining = -1;

	bool bAbilityCool = AbilitySystem->GetCooldownRemainingAndDurationByTag(CooldownTags, CoolRemaining, CoolDuration);
	if (bAbilityCool)
	{
		IconImage->SetColorAndOpacity(InCoolIconColor);

		CoolTimeProgressBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CoolTimeTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (0 < CoolDuration)
		{
			float CoolPercent = CoolRemaining / CoolDuration;
			CoolTimeProgressBar->SetPercent(CoolPercent);
		}
		FString CoolRemainingString = FString::Printf(TEXT("%.1f"), CoolRemaining);
		CoolTimeTextBlock->SetText(FText::FromString(CoolRemainingString));
	}
	else
	{
		IconImage->SetColorAndOpacity(FLinearColor::White);

		CoolTimeProgressBar->SetVisibility(ESlateVisibility::Hidden);
		CoolTimeTextBlock->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWidget_AbilityNode::ResetNode()
{
	bIsAllocatedAbility = false;

	IconImage->SetVisibility(ESlateVisibility::Hidden);
	IconImage->SetBrushFromTexture(nullptr);

	CoolTimeProgressBar->SetVisibility(ESlateVisibility::Hidden);
	CoolTimeTextBlock->SetVisibility(ESlateVisibility::Hidden);

	AbilityClass = nullptr;
	CooldownTags.Reset(1);
}

void UWidget_AbilityNode::LinkWithAbilitySystem(UActionPFAbilitySystemComponent* NewAbilitySystem)
{
	AbilitySystem = NewAbilitySystem;
	if (!AbilitySystem.IsValid())
	{
		PFLOG(Warning, TEXT("Linked Ability System Is Not valid"));
		ResetNode();
	}
}

bool UWidget_AbilityNode::AllocateAbility(TSubclassOf<class UActionPFGameplayAbility> AllocatedAbility)
{
	ResetNode();

	if (AllocatedAbility.GetDefaultObject() == nullptr)
	{
		PFLOG(Warning, TEXT("Can't Allocate Ability In Icon Node : New AllocatedAbility Is Null"));
		return false;
	}

	if (!AbilitySystem.IsValid())
	{
		PFLOG(Warning, TEXT("Linked Ability System Is Not valid"));
		return false;
	}

	TSoftObjectPtr<UTexture2D> NewIconTexture = AllocatedAbility.GetDefaultObject()->GetAbilityIconTexture();
	IconImage->SetBrushFromSoftTexture(NewIconTexture);
	IconImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	CooldownTags = *AllocatedAbility.GetDefaultObject()->GetCooldownTags();
	bIsAllocatedAbility = true;

	return true;
}
