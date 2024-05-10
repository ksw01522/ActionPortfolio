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
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> Canvas;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class USkillHotKeyWindow> SkillWindow;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	USkillHotKeyWindow* GetSkillHotkeyWindow() const { return SkillWindow; }
};
