// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Ability/SkillHotKeyWindow.h"
#include "Ability/Widget/AbilitySlotWidget.h"
#include "ActionPortfolio.h"
#include "Character/Player/PlayerCharacter.h"
#include "Blueprint/WidgetTree.h"
#include "Ability/Slot/AbilitySlot_HotKey.h"
#include "EnhancedInputSubsystems.h"
#include "CustomInputSettingSubsystem.h"
#include "Character/Player/ActionPFPlayerController.h"

FLinkAbilityHotKeySlotNode::FLinkAbilityHotKeySlotNode(UAbilityHotKeySlotWidget* InSlotWidget) : Slot(nullptr), SlotWidget(InSlotWidget)
{
}


void USkillHotKeyWindow::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UWidget*> AllChildWidgets;
	WidgetTree->GetAllWidgets(AllChildWidgets);

	for (auto& ChildWidget : AllChildWidgets)
	{
		if (UAbilityHotKeySlotWidget* HotKeySlot = Cast<UAbilityHotKeySlotWidget>(ChildWidget))
		{
			HotKeySlotNodes.Emplace(HotKeySlot);
		}
	}

	if (AActionPFPlayerController* PC = GetOwningPlayer<AActionPFPlayerController>())
	{
		if (UCustomInputSettingSubsystem* CISS = UCustomInputSettingSubsystem::GetInstance())
		{
			OnChangedMappingHandle = CISS->AddOnChangedMappingEvent(TDelegate<void (const FName&, const FKey&)>::CreateUObject(this, &USkillHotKeyWindow::OnChangedInputMapping));
			OnResetMappingHandle = CISS->AddResetMappingEvent(TDelegate<void (UPlayerMappableInputConfig*)>::CreateUObject(this, &USkillHotKeyWindow::OnResetInputMapping));
		}

		OnChangedPawn(nullptr, PC->GetPawn());
		PC->OnPossessedPawnChanged.AddDynamic(this, &USkillHotKeyWindow::OnChangedPawn);
	}
}

void USkillHotKeyWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ESlateVisibility TempVisibility = GetVisibility();

	if (0 < InDeltaTime)
	{
		if (TempVisibility != ESlateVisibility::Hidden && TempVisibility != ESlateVisibility::Collapsed)
		{
			UpdateAbilityIcon();
		}
	}
}

void USkillHotKeyWindow::BeginDestroy()
{
	if (UCustomInputSettingSubsystem* CISS = UCustomInputSettingSubsystem::GetInstance())
	{
		CISS->RemoveOnChangedMappingEvent(OnChangedMappingHandle);
		CISS->RemoveResetMappingEvent(OnResetMappingHandle);
	}

	if (AActionPFPlayerController* PC = GetOwningPlayer<AActionPFPlayerController>())
	{
		PC->OnPossessedPawnChanged.RemoveAll(this);
	}


	Super::BeginDestroy();
}

void USkillHotKeyWindow::UpdateAbilityIcon()
{
	if (!LinkedASC.IsValid()) return;

	for (auto& Node : HotKeySlotNodes)
	{
		if(!Node.Slot.IsValid()) continue;

		if (const UActionPFGameplayAbility* AbilityCDO = Node.Slot->GetAbilityInSlot().GetDefaultObject())
		{
			AbilityCDO->UpdateAbilityIcon(LinkedASC.Get(), Node.SlotWidget->GetAbilityIcon());
		}
	}
}

void USkillHotKeyWindow::OnChangedInputMapping(const FName& InCode, const FKey& NewKey)
{
	UpdateHotKeyIcon();
}

void USkillHotKeyWindow::OnResetInputMapping(UPlayerMappableInputConfig* InConfig)
{
	UpdateHotKeyIcon();
}

void USkillHotKeyWindow::LinkAbilityHotKeySlots(const TArray<class UAbilitySlot_HotKey*>& InSlots)
{
	UnlinkAbilityHotKeySlots();

	int SlotNum = InSlots.Num();

	for (int i = 0; i < SlotNum; i++)
	{
		UAbilitySlot_HotKey* InSlot = InSlots[i];
		FLinkAbilityHotKeySlotNode& Node = HotKeySlotNodes[i];

		if (Node.Slot.IsValid())
		{
			Node.Slot->GetAbilityChangedDelegate().Remove(Node.Handle);
			Node.Handle.Reset();
		}

		Node.Slot = InSlot;

		//Set AbilityIcon
		if (UActionPFGameplayAbility* AbilityCDO = InSlot->GetAbilityInSlot().GetDefaultObject())
		{
			Node.SlotWidget->SetAbilityIcon(AbilityCDO->CreateAbilityIcon());
		}
		else
		{
			Node.SlotWidget->SetAbilityIcon(nullptr);
		}

		//On Ability Changed In Slot
		Node.Handle = Node.Slot->GetAbilityChangedDelegate().AddLambda([WeakWidget = Node.SlotWidget](UAbilitySlot* InSlot)
			{
				if (!WeakWidget.IsValid()) { return; }

				if (UActionPFGameplayAbility* AbilityCDO = InSlot->GetAbilityInSlot().GetDefaultObject())
				{
					WeakWidget->SetAbilityIcon(AbilityCDO->CreateAbilityIcon());
				}
				else
				{
					WeakWidget->SetAbilityIcon(nullptr);
				}
			});
	}

}

void USkillHotKeyWindow::UnlinkAbilityHotKeySlots()
{
	for (auto& InNode : HotKeySlotNodes)
	{
		if (!InNode.Slot.IsValid()) continue;

		InNode.Slot->GetAbilityChangedDelegate().Remove(InNode.Handle);
		InNode.Handle.Reset();

		InNode.Slot.Reset();
	}
}

void USkillHotKeyWindow::OnChangedPawn(APawn* OldPawn, APawn* NewPawn)
{
	if (NewPawn)
	{
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(NewPawn))
		{
			SetLinkedAbilitySystem(StaticCast<UActionPFAbilitySystemComponent*>(ASI->GetAbilitySystemComponent()));
		}
		else
		{
			SetLinkedAbilitySystem(nullptr);
		}

		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(NewPawn))
		{
			TArray<UAbilitySlot_HotKey*> TempHotKeySlots;
			PlayerCharacter->GetAbilityHotKeySlots(TempHotKeySlots);

			LinkAbilityHotKeySlots(TempHotKeySlots);

			UpdateHotKeyIcon();
		}
	}
	else
	{
		SetLinkedAbilitySystem(nullptr);
	}
}




void USkillHotKeyWindow::UpdateHotKeyIcon()
{
	//Input Subsystem For Hot Key Icon
	UEnhancedInputLocalPlayerSubsystem* ISS = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	for (auto& Node : HotKeySlotNodes)
	{
		if(!Node.Slot.IsValid()) continue;

		//HotKey Icon
		TArray<FKey> HotKeys = ISS->QueryKeysMappedToAction(Node.Slot->GetHotKeyAction());
		for (auto& HotKey : HotKeys)
		{
			if (HotKey.IsGamepadKey())
			{
				Node.SlotWidget->SetHotKey_Gamepad(HotKey);
			}
			else
			{
				Node.SlotWidget->SetHotKey_Keyboard(HotKey);
			}
		}
	}

}

