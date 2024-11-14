// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationWindow.generated.h"

/**
 * 
 */
UCLASS(Category = "Notification")
class ACTIONPORTFOLIO_API UNotificationNode : public UUserWidget
{
	GENERATED_BODY()

public:
	UNotificationNode(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> TextBlock;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WindowInAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WindowOutAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Notification")
	float MaxNotificationTime;

	float RemainNotifiactionTime;

	bool bIsNoticing;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


public:
	void SetNoticeText(const FText& NewText);
	
	void WindowIn();
	void WindowOut();

	bool IsNoticing() const { return bIsNoticing; };

private:
	UFUNCTION()
	void OnWindowOutAnimFinished();
};

UCLASS(Category = "Notification")
class ACTIONPORTFOLIO_API UNotificationWindow : public UUserWidget
{
	GENERATED_BODY()
public:
	UNotificationWindow(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Window", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UVerticalBox> NotificationBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	int MaxNodeCount;

public:
	void AddNotificationNode(UNotificationNode& InNode);
};
