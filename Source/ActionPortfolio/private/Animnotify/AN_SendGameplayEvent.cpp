// Fill out your copyright notice in the Description page of Project Settings.


#include "Animnotify/AN_SendGameplayEvent.h"
#include "Character/ActionPortfolioCharacter.h"

UAN_SendGameplayEvent::UAN_SendGameplayEvent()
{
	CachedNotifyName = TEXT("Send GameplayEvent  A");
}

void UAN_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActionPortfolioCharacter* Char = MeshComp->GetOwner<AActionPortfolioCharacter>();
	if(Char == nullptr) return;

	UAbilitySystemComponent* AbilitySystem = Char->GetAbilitySystemComponent();
	FGameplayEventData Payload;
	Payload.Instigator = AbilitySystem->GetOwnerActor();

	Char->GetAbilitySystemComponent()->HandleGameplayEvent(EventTag , &Payload);
}

#if WITH_EDITOR


void UAN_SendGameplayEvent::BuildNotifyName()
{
	FString TagName = EventTag.GetTagName().ToString();
	int Idx = TagName.Find(".", ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	FString Temp = EventTag.IsValid() ? TagName.RightChop(Idx + 1) : "Empty Tag";
	CachedNotifyName = FString::Printf(TEXT("Send EventTag : %s  A"), *Temp);
}

void UAN_SendGameplayEvent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if(PropertyName == GET_MEMBER_NAME_CHECKED( UAN_SendGameplayEvent, EventTag ) )
	{
		BuildNotifyName();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

FString UAN_SendGameplayEvent::GetNotifyName_Implementation() const
{
	return CachedNotifyName;
}
