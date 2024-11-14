// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Cue/GameplayCueNotify_StateIcon.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "ActionPortfolio.h"
#include "Ability/Widget/StateIconWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Ability/Widget/StateIconWidget.h"

UGameplayCueNotify_StateIcon::UGameplayCueNotify_StateIcon()
{
	static ConstructorHelpers::FClassFinder<UStateIconWidget> DefaultStateIconWidget(TEXT("/Game/ActionPFCharacter/Common/Ability/GameplayCue/StateIcon/WidgetBP_StateIcon.WidgetBP_StateIcon_C"));
	if (DefaultStateIconWidget.Succeeded())
	{
		WidgetClass = DefaultStateIconWidget.Class;
	}
	
}

bool UGameplayCueNotify_StateIcon::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	return false;
}

bool UGameplayCueNotify_StateIcon::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	return false;
}

bool UGameplayCueNotify_StateIcon::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	ENetMode NetMode = GetWorld()->GetNetMode();

	if (NetMode == ENetMode::NM_DedicatedServer)
	{
		return false;
	}

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MyTarget))
	{
		UActionPFAbilitySystemComponent* ASC = StaticCast<UActionPFAbilitySystemComponent*>(ASI->GetAbilitySystemComponent());

		if (0 < ASC->GetGameplayTagCount(GameplayCueTag)) {
			ASC->AddStateIcon(GameplayCueTag, WidgetClass, IconBrush);
		}
	}

	return false;
}

bool UGameplayCueNotify_StateIcon::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	ENetMode NetMode = GetWorld()->GetNetMode();

	if (NetMode == ENetMode::NM_DedicatedServer)
	{
		return false;
	}

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MyTarget))
	{
		UActionPFAbilitySystemComponent* ASC = StaticCast<UActionPFAbilitySystemComponent*>(ASI->GetAbilitySystemComponent());

		if (ASC->GetGameplayTagCount(GameplayCueTag) <= 0) {
			ASC->RemoveStateIcon(GameplayCueTag);
		}
	}

	return false;
}
