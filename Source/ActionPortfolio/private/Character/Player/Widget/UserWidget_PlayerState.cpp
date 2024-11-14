// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Widget/UserWidget_PlayerState.h"
#include "Ability/ActionPFAttributeSet.h"
#include "Character/Player/Widget/CustomProgressBar.h"
#include "Components/WrapBox.h"
#include "Ability/Widget/StateIconWidget.h"
#include "Character/Player/ActionPFPlayerController.h"


void UUserWidget_PlayerState::LinkAbilitySystemComponent(UActionPFAbilitySystemComponent* NewASC)
{
	if(LinkedASC == NewASC) return;

	ClearLinkedASC();

	LinkedASC = NewASC;

	OnLinkedASC();
}





void UUserWidget_PlayerState::ClearLinkedASC()
{
	if(!LinkedASC.IsValid()) return;

	LinkedASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetMaxHealthAttribute()).Remove(OnChangedMaxHPHandle);
	LinkedASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetHealthAttribute()).Remove(OnChangedHPHandle);

	LinkedASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetMaxStaminaAttribute()).Remove(OnChangedMaxStaminaHandle);
	LinkedASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetStaminaAttribute()).Remove(OnChangedStaminaHandle);

	LinkedASC->GetOnNewOrRemovedStateIconDelegate().Remove(NewOrRemovedStateIconHandle);

	LinkedASC.Reset();

	StateIconBox->ClearChildren();
	StateIconMap.Empty();

}

void UUserWidget_PlayerState::NativeConstruct()
{
	Super::NativeConstruct();

	if (AActionPFPlayerController* PlayerController = GetOwningPlayer<AActionPFPlayerController>())
	{
		UActionPFAbilitySystemComponent* ASC = Cast<UActionPFAbilitySystemComponent>(PlayerController->GetAbilitySystemComponent());

		LinkAbilitySystemComponent(ASC);
	}
}

void UUserWidget_PlayerState::OnLinkedASC()
{
	if (!LinkedASC.IsValid()) return;

	TWeakObjectPtr<UCustomProgressBar> WeakHPBar = HPBar;
	
	bool Result;
	HPBar->SetMaxValue(LinkedASC->GetGameplayAttributeValue(UCharacterAttributeSet::GetMaxHealthAttribute(), Result));
	HPBar->SetCurrentValue(LinkedASC->GetGameplayAttributeValue(UCharacterAttributeSet::GetHealthAttribute(), Result));

	OnChangedMaxHPHandle = LinkedASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetMaxHealthAttribute())
									.AddLambda([WeakHPBar](const FOnAttributeChangeData& NewData){
														if(!WeakHPBar.IsValid()) return;

														WeakHPBar->SetMaxValue(NewData.NewValue);
														});
	OnChangedHPHandle = LinkedASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetHealthAttribute())
									.AddLambda([WeakHPBar](const FOnAttributeChangeData& NewData) {
														if (!WeakHPBar.IsValid()) return;

														WeakHPBar->SetCurrentValue(NewData.NewValue);
														});



	TWeakObjectPtr<UCustomProgressBar> WeakStaminaBar = StaminaBar;
	
	StaminaBar->SetMaxValue(LinkedASC->GetGameplayAttributeValue(UCharacterAttributeSet::GetMaxStaminaAttribute(), Result));
	StaminaBar->SetCurrentValue(LinkedASC->GetGameplayAttributeValue(UCharacterAttributeSet::GetStaminaAttribute(), Result));

	OnChangedMaxStaminaHandle = LinkedASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetMaxStaminaAttribute())
											.AddLambda([WeakStaminaBar](const FOnAttributeChangeData& NewData) {
														if (!WeakStaminaBar.IsValid()) return;

														WeakStaminaBar->SetMaxValue(NewData.NewValue);
														});
	OnChangedStaminaHandle = LinkedASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetStaminaAttribute())
											.AddLambda([WeakStaminaBar](const FOnAttributeChangeData& NewData) {
														if (!WeakStaminaBar.IsValid()) return;

														WeakStaminaBar->SetCurrentValue(NewData.NewValue);
														});

	
	TArray<FStateIconStruct> IconStructs = LinkedASC->GetStateIconArray();

	for (auto& IconStruct : IconStructs)
	{
		AddStateIcon(IconStruct.StateTag, IconStruct.WidgetClass, IconStruct.ImageBrush);
	}

	NewOrRemovedStateIconHandle = LinkedASC->GetOnNewOrRemovedStateIconDelegate().AddUObject(this, &UUserWidget_PlayerState::OnNewOrRemovedStateIcon);
}

void UUserWidget_PlayerState::OnNewOrRemovedStateIcon(const FGameplayTag& InTag, TSubclassOf<class UStateIconWidget> InWidgetClass, const FSlateBrush& InBrush, bool bIsNew)
{
	if (bIsNew)
	{
		AddStateIcon(InTag, InWidgetClass, InBrush);
	}
	else
	{
		RemoveStateIcon(InTag,InWidgetClass, InBrush);
	}
}

void UUserWidget_PlayerState::AddStateIcon(const FGameplayTag& InTag, TSubclassOf<class UStateIconWidget> InWidgetClass, const FSlateBrush& InBrush)
{
	if(StateIconMap.Contains(InTag)) return;

	UStateIconWidget* NewStateIcon = CreateWidget<UStateIconWidget>(this, InWidgetClass);
	NewStateIcon->SetIconBrush(InBrush);

	StateIconBox->AddChildToWrapBox(NewStateIcon);
	StateIconMap.Add(InTag, NewStateIcon);
}

void UUserWidget_PlayerState::RemoveStateIcon(const FGameplayTag& InTag, TSubclassOf<class UStateIconWidget> InWidgetClass, const FSlateBrush& InBrush)
{
	if (!StateIconMap.Contains(InTag)) return;

	StateIconMap[InTag]->RemoveFromParent();
	StateIconMap.Remove(InTag);
}
