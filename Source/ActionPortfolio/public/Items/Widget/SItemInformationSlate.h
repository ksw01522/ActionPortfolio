// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class ACTIONPORTFOLIO_API SItemInformationSlate : public SCompoundWidget
{
	SLATE_DECLARE_WIDGET(SItemInformationSlate, SCompoundWidget);

public:
	SLATE_BEGIN_ARGS(SItemInformationSlate) :  _MeshCaptureBrush(nullptr), _NameStyle(nullptr), _DescriptionStyle(nullptr)
	{}

	SLATE_ARGUMENT(const FSlateBrush*, MeshCaptureBrush)
	SLATE_ARGUMENT(const FTextBlockStyle*, NameStyle)
	SLATE_ARGUMENT(FText, InitName)
	SLATE_ARGUMENT(const FTextBlockStyle*, DescriptionStyle)
	SLATE_ARGUMENT(FText, InitDescription)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<class SImage> MeshCaptureImage;
	TSharedPtr<class STextBlock> NameTextBlock;
	TSharedPtr<STextBlock> DescriptionTextBlock;

	void SetNameText(const FName& InName);
	void SetDescriptionText(const FString& InDesc);
public:
	
	void UpdateInformation(const class UItemBase* InItem);

	void ClearInformation();

	void SetMeshCaptureBrush(const FSlateBrush* InBrush);
	void SetNameStyle(const FTextBlockStyle* InStyle);
	void SetDescriptionStyle(const FTextBlockStyle* InStyle);
};
