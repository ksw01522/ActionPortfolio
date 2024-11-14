// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueBoxWidget.generated.h"

/**
 * 
 */


UCLASS()
class ACTIONPORTFOLIO_API UDialogueBoxWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
#if WITH_EDITOR
	//virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual const FText GetPaletteCategory() override { return NSLOCTEXT("ActionPFWidget", "DialogueCategory", "Dialogue"); }
#endif

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> NameBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class URichTextBlock> StringBlock;



public:
	void SetDialoguerName(const FString& NewName);
	void SetDialogueString(const FString& NewString);


protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextAnimation", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float TextAnimationDelay;
	
	float RemainTextAnimationDelay = 0;

	bool bTextAnimating = false;

	FString TargetText;
	FString CurrentText;

	bool bIsDecorating = false;

private:
	void TickTextAnimation(float InDeltaTime);

	void CompleteTextAnimation(bool bForced);

public:
	bool IsTextAnimating() const { return bTextAnimating; }

	void ForceCompleteTextAnimation();

	void ClearDialogueBox();
};