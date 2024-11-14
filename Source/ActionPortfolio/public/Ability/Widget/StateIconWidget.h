// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StateIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UStateIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Image", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> IconImage;

	

public:
	void SetIconImage(UMaterialInterface* NewImage);
	void SetIconBrush(const FSlateBrush& InBrush);

	void SetStateEffectStack(int NewStack);

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif


};
