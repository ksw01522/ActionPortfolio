// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Framework/Text/ITextDecorator.h"

/**
 * 
 */
class ACTIONPORTFOLIO_API SDialogueSlate : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialogueSlate) :
		_OwnerPlayer(nullptr)
	{ }
	SLATE_ARGUMENT(class AActionPFPlayerController*, OwnerPlayer)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<class SNPCDialogueBox> DialogueBox;
	TWeakObjectPtr<AActionPFPlayerController> OwnerPlayer;

	FReply MouseButtonDownInDialogueBox(const FGeometry& Geometry, const FPointerEvent& PointerEvent);

public:
	void SetDialoguerName(const FString& NewName);
	void SetDialogueText(const FString& NewText);
	void SetDialogueStyleSet(class UDataTable* StyleSet);
	void SetDialogueDecorator(const TArray<TSubclassOf<class USRichTextBlockDecorator>>& NewDecoClasses);

	bool IsDialogueAnimating() const;

	void AnimDialogueText();

	void ForceDialogueAnimComplete();

};
