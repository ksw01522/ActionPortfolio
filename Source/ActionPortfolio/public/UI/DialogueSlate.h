// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/Input/SButton.h"
#include "Framework/Text/ITextDecorator.h"

class STextBlock;
class UDataTable;
class SRichTextBlock;
class USRichTextBlockDecorator;
class ITextDecorator;


struct FDialogueElement;

/**
 * 
 */


class ACTIONPORTFOLIO_API SDialogueBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialogueBox){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual ~SDialogueBox() {
		StyleInstance.Reset();
	}

	DECLARE_MULTICAST_DELEGATE(FOnMouseButtonDownInDialogueBox);

private:
	////////////////////////// Text Block ////////////////////////
	TSharedPtr<STextBlock> DialoguerNameBlock;
	TSharedPtr<SRichTextBlock> DialogueTextBlock;

	TObjectPtr<UDataTable> TextStyleSet;
	FTextBlockStyle TextBlockStyle;

	TSharedPtr<FSlateStyleSet> StyleInstance;

	TArray<TSubclassOf<USRichTextBlockDecorator>> DecoClasses;

	TArray<TObjectPtr<USRichTextBlockDecorator>> InstanceDecorators;

	

private:
	void MakeStyleInstance();
	void MakeDecoInstance(TArray< TSharedRef<ITextDecorator > >& OutDecorators);

	void UpdateDialogueTextBlock();

	FReply MouseButtonDownInDialogueBox(const FGeometry& Geometry, const FPointerEvent& PointerEvent);

public:
	FOnMouseButtonDownInDialogueBox OnButtonDown;

	/////////////////////////// Text /////////////////////////////
private:
	FString TargetText;
	FString CurrentText;

	bool bIsDecorating = false;
	bool bIsAnimating = false;

public:
	void SetDialogue(const FDialogueElement& DialogueElement);

	bool IsTextAnimating() const { return bIsAnimating; }

	void AnimText();
	void CompleteAnimText();

	void ClearDialogueText();
};

class ACTIONPORTFOLIO_API SAnswerButton : public SButton
{
public:
	SLATE_BEGIN_ARGS(SAnswerButton) {}

	SLATE_EVENT(FOnClicked, OnClicked);

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<STextBlock> AnswerTextBlock;

public:
	void SetAnswer(const FDialogueElement& DialogueElement);
};
