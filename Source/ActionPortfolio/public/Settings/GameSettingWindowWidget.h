// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SGameSettingWindow.h"
#include "GameSettingWindowWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ACTIONPORTFOLIO_API UGameSettingWindowWidget : public UWidget
{
	GENERATED_BODY()
	
protected:
	TSharedPtr<class SGameSettingWindow> MySettingWindow;

protected:
	//virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual void SynchronizeProperties() override;

#if WITH_EDITOR
	//virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual const FText GetPaletteCategory() override { return NSLOCTEXT("ActionPFUserSetting", "SettingCategory", "CustomSetting"); }
#endif

#if WITH_ACCESSIBILITY
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override;
#endif

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	FTextBlockStyle TextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	FMargin NameBlockPadding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	FSlateBrush FocusedFrameBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	FTextBlockStyle DescTextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	FVector2D DescLabelSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Multiple")
	FSlateBrush ArrowImageBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toggle")
	FSlateBrush CheckedImageBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toggle")
	FSlateBrush UncheckedImageBrush;

public:
	void SetOnBackKeyDownEvent(const SGameSettingWindow::FOnBackKeyDown& InEvent);
};

UCLASS()
class ACTIONPORTFOLIO_API UGameSettingWindowWidget_Graphic : public UGameSettingWindowWidget
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

};