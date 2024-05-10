// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/GameSettingWindowWidget.h"
#include "Settings/SGameSettingWindow.h"
#include "Settings/SGameSettingWindow_Graphic.h"

void UGameSettingWindowWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MySettingWindow.Reset();
}

void UGameSettingWindowWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MySettingWindow.IsValid())
	{
		MySettingWindow->SetTextStyle(&TextStyle);
		MySettingWindow->SetNameblockPadding(NameBlockPadding);
		MySettingWindow->SetFocusedFrame(&FocusedFrameBrush);
		MySettingWindow->SetArrowImage(&ArrowImageBrush);
		MySettingWindow->SetCheckedImage(&CheckedImageBrush);
		MySettingWindow->SetUncheckedImage(&UncheckedImageBrush);
		MySettingWindow->SetDescTextStyle(&DescTextStyle);
		MySettingWindow->SetDescKeyLabelSize(DescLabelSize);
	}
}

TSharedPtr<SWidget> UGameSettingWindowWidget::GetAccessibleWidget() const
{
	return MySettingWindow;
}

void UGameSettingWindowWidget::SetOnBackKeyDownEvent(const SGameSettingWindow::FOnBackKeyDown& InEvent)
{
	MySettingWindow->SetOnBackKeyDownEvent(InEvent);
}

TSharedRef<SWidget> UGameSettingWindowWidget_Graphic::RebuildWidget()
{
	return 
	SAssignNew(MySettingWindow, SGameSettingWindow_Graphic)
	.TextStyle(&TextStyle)
	.NameBlockPadding(NameBlockPadding)
	.FocusedFrameBrush(&FocusedFrameBrush)
	.ArrowImageBrush(&ArrowImageBrush)
	.CheckedImageBrush(&CheckedImageBrush)
	.UncheckedImageBrush(&UncheckedImageBrush)
	.DescTextStyle(&DescTextStyle)
	.DescLabelSize(DescLabelSize);
}
