// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillHotKeyWindow.generated.h"

/**
 * 
 */
struct FLinkAbilityHotKeySlotNode
{
	FLinkAbilityHotKeySlotNode() : Slot(nullptr), SlotWidget(nullptr)
	{}

	FLinkAbilityHotKeySlotNode(class UAbilityHotKeySlotWidget* InSlotWidget);
	

	TWeakObjectPtr<class UAbilitySlot_HotKey> Slot;

	TWeakObjectPtr<class UAbilityHotKeySlotWidget> SlotWidget;

	FDelegateHandle Handle;
};

UCLASS()
class ACTIONPORTFOLIO_API USkillHotKeyWindow : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void BeginDestroy() override;

private:
	TWeakObjectPtr<class UActionPFAbilitySystemComponent> LinkedASC;

	TArray<FLinkAbilityHotKeySlotNode> HotKeySlotNodes;

	FDelegateHandle OnChangedMappingHandle;
	FDelegateHandle OnResetMappingHandle;

	void UpdateAbilityIcon();

	void OnChangedInputMapping(const FName& InCode, const FKey& NewKey);

	void OnResetInputMapping(class UPlayerMappableInputConfig* InConfig);

	void LinkAbilityHotKeySlots(const TArray<class UAbilitySlot_HotKey*>& InSlots);

	void UnlinkAbilityHotKeySlots();

	void SetLinkedAbilitySystem(UActionPFAbilitySystemComponent* InASC) { LinkedASC = InASC; }

	UFUNCTION()
	void OnChangedPawn(APawn* OldPawn, APawn* NewPawn);

public:
	void UpdateHotKeyIcon();
};
