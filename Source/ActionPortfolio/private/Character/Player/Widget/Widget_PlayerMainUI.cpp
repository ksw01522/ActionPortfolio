// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Widget/Widget_PlayerMainUI.h"
#include "Character/Player/PlayerCharacter.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Character/Player/ActionPFPlayerController.h"

#include "Items/ItemBase.h"
#include "Items/Widget/InventoryWidget.h"

#include "ActionPortfolio.h"
#include "Components/CanvasPanel.h"

#include "Character/Player/Widget/UserWidget_PlayerState.h"
#include "Character/Player/Ability/SkillHotKeyWindow.h"

#include "UI/NotificationWindow.h"

UWidget_PlayerMainUI::UWidget_PlayerMainUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

EDataValidationResult UWidget_PlayerMainUI::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = IsDataValid(ValidationErrors);

	if (GoldNotificationNodeClass.GetDefaultObject() == nullptr)
	{
		Result = EDataValidationResult::Invalid;
		ValidationErrors.Add(NSLOCTEXT("ActionPF MainUI Validation Error", "Gold Notification null class", "골드 알림 노드 클래스가 null입니다"));
	}

	return Result;
}

void UWidget_PlayerMainUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_PlayerMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UWidget_PlayerMainUI::GainGold(float InGold)
{
	if (!GoldNotificationNode.IsValid() || !GoldNotificationNode->IsNoticing())
	{
		GoldNotificationNode = CreateWidget<UNotificationNode>(this, GoldNotificationNodeClass);
		check(GoldNotificationNode.IsValid());
		CurrentGold = 0;

		GetNotificationWindow()->AddNotificationNode(*GoldNotificationNode);
	}

	CurrentGold += InGold;

	FText NoticeText = FText::Format(NSLOCTEXT("ActionPF Notification", "Gain Gold", "<Gold>{0}G</> 획득 했습니다."), CurrentGold);
	GoldNotificationNode->SetNoticeText(NoticeText);
}

