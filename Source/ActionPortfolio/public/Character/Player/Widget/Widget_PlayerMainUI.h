// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_PlayerMainUI.generated.h"

/**
 * 
 */
 enum class EItemType : uint8;
 enum class EItemGrade : uint8;

UCLASS(Abstract)
class ACTIONPORTFOLIO_API UWidget_PlayerMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UWidget_PlayerMainUI(const FObjectInitializer& ObjectInitializer);


#if WITH_EDITOR
protected:
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> Canvas;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class USkillHotKeyWindow> SkillWindow;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UUserWidget_PlayerState> StateWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UNotificationWindow> NotificationWindow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UNotificationNode> GoldNotificationNodeClass;

	TWeakObjectPtr<UNotificationNode> GoldNotificationNode;

	float CurrentGold;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	USkillHotKeyWindow* GetSkillHotkeyWindow() const { return SkillWindow; }

	UNotificationWindow* GetNotificationWindow() const { return NotificationWindow; }

	void GainGold(float InGold);
};
