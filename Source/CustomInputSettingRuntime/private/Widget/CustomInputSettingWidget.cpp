// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CustomInputSettingWidget.h"
#include "Widget/SCustomInputSetting.h"
#include "PlayerMappableInputConfig.h"
#include "CustomInputSettingRuntime.h"


UCustomInputSettingWidget::UCustomInputSettingWidget() : NodePadding(5), DistanceBetweenNode(1)
{
}



TSharedRef<SWidget> UCustomInputSettingWidget::RebuildWidget()
{
	SAssignNew(MyWidget, SCustomInputSettingWindow)
	.NodePadding(NodePadding)
	.TextBorderBrush(&NodeTextBorderBrush)
	.IconBorderBrush(&NodeIconBorderBrush)
	.FocusedFrameBrush(&NodeFocusedFrameBrush)
	.NodeNameBlockStyle(&NodeNameTextStyle)
	.DistanceBetweenNode(DistanceBetweenNode);

	return MyWidget.ToSharedRef();
}



void UCustomInputSettingWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyWidget.Reset();
}

void UCustomInputSettingWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();


	if(!MyWidget.IsValid()) return;

	MyWidget->SetNodePadding(NodePadding);
	MyWidget->SetNodeNameBlockStyle(&NodeNameTextStyle);
	MyWidget->SetMappableKeys(TargetMappableKeys.Get());
	MyWidget->SetDistanceBetweenNode(DistanceBetweenNode);
}

#if WITH_EDITOR

void UCustomInputSettingWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

const FText UCustomInputSettingWidget::GetPaletteCategory()
{
	return NSLOCTEXT("CustomInputSetting", "CustomInputCategory", "CustomInput");
}

#endif

#if WITH_ACCESSIBILITY

TSharedPtr<SWidget> UCustomInputSettingWidget::GetAccessibleWidget() const
{
	return MyWidget;
}

#endif


void UCustomInputSettingWidget::SetNodePadding(FMargin InPadding)
{
	if(NodePadding == InPadding) return;

	NodePadding = InPadding;

	if (MyWidget.IsValid())
	{
		MyWidget->SetNodePadding(InPadding);
	}
}

void UCustomInputSettingWidget::SetTargetMappableKeys(UPlayerMappableInputConfig* InKeys)
{
	if(TargetMappableKeys == InKeys) return;

	if (MyWidget.IsValid())
	{
		MyWidget->SetMappableKeys(TargetMappableKeys.Get());
	}
}
