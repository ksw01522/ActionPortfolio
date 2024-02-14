// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "CustomInputSettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMINPUTSETTINGRUNTIME_API UCustomInputSettingWidget : public UWidget
{
	GENERATED_BODY()
	
public:
	UCustomInputSettingWidget();

protected:
	TSharedPtr<class SCustomInputSettingWindow> MyWidget;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Node", BlueprintSetter = SetNodePadding, meta = (AllowPrivateAccess = "true"))
	FMargin NodePadding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Node", meta = (AllowPrivateAccess = "true"))
	FSlateBrush NodeTextBorderBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Node", meta = (AllowPrivateAccess = "true"))
	FSlateBrush NodeIconBorderBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Node", meta = (AllowPrivateAccess = "true"))
	FSlateBrush NodeFocusedFrameBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Node", meta = (AllowPrivateAccess = "true"))
	FVector2D IconSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Node", meta = (AllowPrivateAccess = "true"))
	FTextBlockStyle NodeNameTextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Node", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float DistanceBetweenNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Tab", meta = (AllowPrivateAccess = "true"))
	FVector2D TabSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Tab", meta = (AllowPrivateAccess = "true"))
	FTextBlockStyle TabTextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Tab", meta = (AllowPrivateAccess = "true"))
	FSlateBrush SelectedTabBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Tab", meta = (AllowPrivateAccess = "true"))
	FSlateBrush UnselectedTabBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Description", meta = (AllowPrivateAccess = "true"))
	FTextBlockStyle DescriptionTextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style|Description", meta = (AllowPrivateAccess = "true"))
	FVector2D DescriptionLabelSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomInput", BlueprintSetter = SetTargetMappableKeys, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerMappableInputConfig> TargetMappableKeys;

public:
	UFUNCTION(BlueprintSetter)
	void SetNodePadding(FMargin InPadding);
	
	UFUNCTION(BlueprintSetter)
	void SetTargetMappableKeys(UPlayerMappableInputConfig* InKeys);

	

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual void SynchronizeProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual const FText GetPaletteCategory() override;
#endif

#if WITH_ACCESSIBILITY
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override;
#endif
};
