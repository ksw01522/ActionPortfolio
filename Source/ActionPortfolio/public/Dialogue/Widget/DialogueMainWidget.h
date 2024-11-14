// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueMainWidget.generated.h"

/**
 * 
 */


UCLASS()
class ACTIONPORTFOLIO_API UDialogueMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
#if WITH_EDITOR
	//virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual const FText GetPaletteCategory() override { return NSLOCTEXT("ActionPFWidget", "DialogueCategory", "Dialogue"); }
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UDialogueBoxWidget> DialogueBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UPanelWidget> AnswerPanel;


protected:
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	
};
