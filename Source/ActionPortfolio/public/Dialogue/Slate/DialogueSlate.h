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

DECLARE_DELEGATE_OneParam(FOnDialogueAnimComplete, bool);

/**
 * 
 */
class ACTIONPORTFOLIO_API SDialogueMainSlate : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialogueMainSlate)
		: _AnswerButtonPadding(0, 5)
	{}

	SLATE_ARGUMENT(FMargin, AnswerButtonPadding)

	SLATE_EVENT(FSimpleDelegate, OnActNextDel)

	SLATE_EVENT(FOnDialogueAnimComplete, OnDialogueAnimComplete)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<class SDialogueBox> DialogueBox;
	TSharedPtr<class SVerticalBox> AnswerBox;

	FMargin AnswerButtonPadding;

	FSimpleDelegate OnActNextDel;

protected:
	virtual bool SupportsKeyboardFocus() const override { return true; }

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

private:
	void ActNextDialogue();

public:
	void SetOnActNextDelegate(const FSimpleDelegate& OnDel);
	void ClearOnActNextDelegate();

	void CreateAnswerButton(const FString InAnswerString, const FOnClicked& ClickMethod);

	void ClearAnswerBox();

	SDialogueBox* GetDialogueBox() const { return DialogueBox.Get(); }
};

class ACTIONPORTFOLIO_API SDialogueBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialogueBox){}

	SLATE_EVENT(FOnDialogueAnimComplete, OnAnimComplete)

	SLATE_END_ARGS()

	SDialogueBox();

	void Construct(const FArguments& InArgs);

	virtual ~SDialogueBox() {
		StyleInstance.Reset();
	}

protected:
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

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

	/////////////////////////// Text /////////////////////////////
private:
	FString TargetText;
	FString CurrentText;

	bool bIsDecorating = false;

	void SetDialoguerName(const FString& InName);
	void SetDialogueString(const FString& InString);
	void SetSlateDecorators(const TArray<TSubclassOf<class USRichTextBlockDecorator>>& Decos);

public:
	void SetDialogueBox(const FString& InName, const FString& InString, const TArray<TSubclassOf<class USRichTextBlockDecorator>>& Decos);

	void ClearDialogueBox();


////////////////////////////// Animating //////////////////////
private:
	float CurrentTime;
	float AnimatingDelay;

	FOnDialogueAnimComplete OnAnimComplete;

public:
	bool IsTextAnimating() const { return GetCanTick(); }

	void CompleteAnimText(bool bForced);
};

