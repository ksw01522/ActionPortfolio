// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NotificationWindow.h"
#include "Components/RichTextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "ActionPortfolio.h"
#include "Components/VerticalBox.h"

UNotificationNode::UNotificationNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), MaxNotificationTime(3), bIsNoticing(false)
{
	
}

void UNotificationNode::NativeConstruct()
{
	Super::NativeConstruct();

	if (WindowOutAnim)
	{
		FWidgetAnimationDynamicEvent OnOutAnimFinihed;
		OnOutAnimFinihed.BindDynamic(this, &UNotificationNode::OnWindowOutAnimFinished);
		BindToAnimationFinished(WindowOutAnim, OnOutAnimFinihed);
	}
	
}

void UNotificationNode::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsNoticing)
	{
		RemainNotifiactionTime -= InDeltaTime;
		if (RemainNotifiactionTime <= 0)
		{
			WindowOut();
		}
	}
}

void UNotificationNode::SetNoticeText(const FText& NewText)
{
	TextBlock->SetText(NewText);
}

void UNotificationNode::WindowIn()
{
	if(bIsNoticing) return;

	PlayAnimation(WindowInAnim);

	RemainNotifiactionTime = MaxNotificationTime;
	bIsNoticing = true;
}

void UNotificationNode::WindowOut()
{
	if(!bIsNoticing) return;

	StopAnimation(WindowInAnim);
	PlayAnimation(WindowOutAnim);
	bIsNoticing = false;
}

void UNotificationNode::OnWindowOutAnimFinished()
{
#if WITH_EDITOR
	PFLOG(Warning, TEXT("On Notice Out Anim Finished : {%s}"), *TextBlock->GetText().ToString());
#endif
	RemoveFromParent();
}

UNotificationWindow::UNotificationWindow(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), MaxNodeCount(5)
{

}

void UNotificationWindow::AddNotificationNode(UNotificationNode& InNode)
{	
	UVerticalBoxSlot* NewSlot = NotificationBox->AddChildToVerticalBox(&InNode);
	InNode.WindowIn();

	int CurrentCount = NotificationBox->GetChildrenCount();
	if (MaxNodeCount < CurrentCount)
	{
		TArray<UWidget*> Nodes = NotificationBox->GetAllChildren();
		for (int i = MaxNodeCount; i < CurrentCount; i++)
		{
			UNotificationNode* Notification = StaticCast<UNotificationNode*>(Nodes[i]);
			Notification->WindowOut();
		}
	}
}
