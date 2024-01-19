// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_AbilityNode.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ACTIONPORTFOLIO_API UWidget_AbilityNode : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USizeBox> NodeSizeBox;

	UPROPERTY(meta = (BindWidget))
	class UBorder* IconBorder; 

	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* CoolTimeProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CoolTimeTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilityNode", meta = (AllowPrivateAccess = "true"))
	FLinearColor InCoolIconColor;


	TWeakObjectPtr<class UActionPFAbilitySystemComponent> AbilitySystem;
	TSubclassOf<class UActionPFGameplayAbility> AbilityClass;

	FGameplayTagContainer CooldownTags;

protected:
	bool bIsAllocatedAbility;
	bool bCanActivateAbility;
	

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void ResetNode();

public:
	void LinkWithAbilitySystem(UActionPFAbilitySystemComponent* NewAbilitySystem);
	
	bool AllocateAbility(TSubclassOf<class UActionPFGameplayAbility> AllocatedAbility);
	

};
