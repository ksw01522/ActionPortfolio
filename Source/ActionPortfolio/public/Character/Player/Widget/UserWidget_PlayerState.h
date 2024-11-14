// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "UserWidget_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UUserWidget_PlayerState : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCustomProgressBar> HPBar;
	FDelegateHandle OnChangedMaxHPHandle;
	FDelegateHandle OnChangedHPHandle;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCustomProgressBar> StaminaBar;
	FDelegateHandle OnChangedMaxStaminaHandle;
	FDelegateHandle OnChangedStaminaHandle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> StateIconBox;

	FDelegateHandle NewOrRemovedStateIconHandle;

	TWeakObjectPtr<class UActionPFAbilitySystemComponent> LinkedASC;

	TMap<FGameplayTag, TObjectPtr<class UStateIconWidget>> StateIconMap;

protected:
	virtual void OnLinkedASC();
	virtual void ClearLinkedASC();

	virtual void NativeConstruct() override;

	void CreateStateIcon(const FGameplayTag& InTag, TSubclassOf<class UStateIconWidget> InWidgetClass, const FSlateBrush& InBrush);

public:
	void LinkAbilitySystemComponent(UActionPFAbilitySystemComponent* NewASC);
	
	UFUNCTION()
	void OnNewOrRemovedStateIcon(const FGameplayTag& InTag, TSubclassOf<class UStateIconWidget> InWidgetClass, const FSlateBrush& InBrush, bool bIsNew);

	void AddStateIcon(const FGameplayTag& InTag, TSubclassOf<class UStateIconWidget> InWidgetClass, const FSlateBrush& InBrush);

	void RemoveStateIcon(const FGameplayTag& InTag, TSubclassOf<class UStateIconWidget> InWidgetClass, const FSlateBrush& InBrush);
};
