// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Widget/SItemInformationSlate.h"
#include "SlateOptMacros.h"
#include "Items/ItemManagerSubsystem.h"
#include "Widgets/Images/SImage.h"
#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "Items/ItemBase.h"
#include "Items/ItemManagerSubsystem.h"
#include "Widgets/Layout/SScaleBox.h"

SLATE_IMPLEMENT_WIDGET(SItemInformationSlate)
void SItemInformationSlate::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SItemInformationSlate::Construct(const FArguments& InArgs)
{
	UItemManagerSubsystem* IMS = UItemManagerSubsystem::GetManagerInstance();

	FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();
	
	const FSlateBrush* MeshCaptureBrush = InArgs._MeshCaptureBrush;
	if(MeshCaptureBrush == nullptr) StyleSet->GetBrush(InventoryStyle::InformationSlate::MeshCaptureStyle);

	const FTextBlockStyle* NameStyle = InArgs._NameStyle;
	if(NameStyle == nullptr) NameStyle = &StyleSet->GetWidgetStyle<FTextBlockStyle>(InventoryStyle::InformationSlate::NameTextStyle);
	
	const FTextBlockStyle* DescriptionStyle = InArgs._DescriptionStyle;
	if(DescriptionStyle == nullptr) DescriptionStyle = &StyleSet->GetWidgetStyle<FTextBlockStyle>(InventoryStyle::InformationSlate::DescriptionTextStyle);

	ChildSlot
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SNew(SVerticalBox)
		.Visibility(EVisibility::HitTestInvisible)
		+SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.AutoHeight()
		.Padding(30)
		[
			
			SAssignNew(MeshCaptureImage, SImage)
			.Image(MeshCaptureBrush)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)
		.Padding(20)
		[
			SAssignNew(NameTextBlock, STextBlock)
			.TextStyle(NameStyle)
			.Justification(ETextJustify::Center)
			.Text(InArgs._InitName)
		]
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)
		.Padding(20)
		.FillHeight(1)
		[
			SAssignNew(DescriptionTextBlock, STextBlock)
			.TextStyle(DescriptionStyle)
			.Justification(ETextJustify::Center)
			.Text(InArgs._InitDescription)
		]
	];
	
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SItemInformationSlate::SetNameText(const FName& InName)
{
	NameTextBlock->SetText(FText::FromName( InName));
}

void SItemInformationSlate::SetDescriptionText(const FString& InDesc)
{
	DescriptionTextBlock->SetText(FText::FromString(InDesc));
}

void SItemInformationSlate::UpdateInformation(const UItemBase* InItem)
{
	if (InItem != nullptr)
	{
		UItemManagerSubsystem* IMS = UItemManagerSubsystem::GetManagerInstance();

		IMS->SetCaptureItemMesh(InItem);

		FName ItemName = InItem->GetItemName();
		SetNameText(ItemName);

		FString Desc = InItem->GetItemDescription();
		SetDescriptionText(Desc);
	}
	else
	{
		ClearInformation();
	}
}

void SItemInformationSlate::ClearInformation()
{
	UItemManagerSubsystem* IMS = UItemManagerSubsystem::GetManagerInstance();

	IMS->ClearCaptureItemMesh();

	NameTextBlock->SetText(FText::GetEmpty());
	DescriptionTextBlock->SetText(FText::GetEmpty());
}

void SItemInformationSlate::SetMeshCaptureBrush(const FSlateBrush* InBrush)
{
	MeshCaptureImage->SetImage(InBrush);
}

void SItemInformationSlate::SetNameStyle(const FTextBlockStyle* InStyle)
{
	NameTextBlock->SetTextStyle(InStyle);
}

void SItemInformationSlate::SetDescriptionStyle(const FTextBlockStyle* InStyle)
{
	DescriptionTextBlock->SetTextStyle(InStyle);
}
