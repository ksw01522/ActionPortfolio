// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "ItemInformationWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UItemInformationWidget : public UWidget
{
	GENERATED_BODY()
private:
	TSharedPtr<class SItemInformationSlate> Slate;

	UPROPERTY(EditAnywhere, Category = "Style", meta = (AllowPrivateAccess = "true"))
	FSlateBrush MeshCaptureBrush;

	UPROPERTY(EditAnywhere, Category = "Style", meta = (AllowPrivateAccess ="true"))
	FTextBlockStyle NameStyle;

	UPROPERTY(EditAnywhere, Category = "Style", meta = (AllowPrivateAccess = "true"))
	FTextBlockStyle DescriptionStyle;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual void SynchronizeProperties() override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

#if WITH_ACCESSIBILITY
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override;
#endif

public:
	void UpdateItemInformation(const class UItemBase* InItem);
};
