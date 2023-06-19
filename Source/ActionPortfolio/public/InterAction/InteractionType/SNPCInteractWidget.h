// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Framework/Text/ITextDecorator.h"

/**
 * 
 */

class ACTIONPORTFOLIO_API SNPCInteractWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNPCInteractWidget) : 
		_NPCInteractionSystem(nullptr), _OwnerPlayer(nullptr)
	{ }

	SLATE_ARGUMENT(class UInteractionSystemComponent_NPC*, NPCInteractionSystem)
	SLATE_ARGUMENT(class AActionPFPlayerController*, OwnerPlayer)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	UInteractionSystemComponent_NPC* NPCInteractionSystem;
	AActionPFPlayerController* OwnerPlayer;
	TSharedPtr<class SNPCDialogueBox> NPCDialogueTextBox;
	TSharedPtr<SVerticalBox> NPCInteractBTNBox;
	TSharedPtr<SBorder> BTNBoxBorder;

private:
	FReply MouseButtonDownInDialogueBox(const FGeometry& Geometry, const FPointerEvent& PointerEvent);
	FReply ExitInteractNPC();

public:
	void SetDialoguerName(const FString& NewName);
	void SetDialogueText(const FString& NewText);
	void SetDialogueStyleSet(class UDataTable* StyleSet);
	void SetDialogueDecorator(const TArray<TSubclassOf<class USRichTextBlockDecorator>>& NewDecoClasses);

	bool IsDialogueAnimating() const;

	void SetShowInteractButton(bool bShow);

	void AnimDialogueText();

	void ForceDialogueAnimComplete();

};

class ACTIONPORTFOLIO_API SNPCInteractButton : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNPCInteractButton) :
		_NPCInteract(), _OwnerPlayer()
	{
		_NPCInteract = nullptr;
		_OwnerPlayer = nullptr;
	}

	SLATE_ARGUMENT(class UNPCInteract*, NPCInteract)
	SLATE_ARGUMENT(AActionPFPlayerController*, OwnerPlayer)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override { return true; }

	FReply PlayerInteractWithNPC();

protected:
	AActionPFPlayerController* OwnerPlayer;
	UNPCInteract* NPCInteract;
};

class ACTIONPORTFOLIO_API SNPCDialogueBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNPCDialogueBox)
		:_TargetText(), _TextStyleSet()
	{
		_DecoClasses.Empty();
	}

	SLATE_ARGUMENT(FString, TargetText)
	SLATE_ARGUMENT(TWeakObjectPtr<class UDataTable>, TextStyleSet)
	SLATE_ARGUMENT(TArray<TSubclassOf<class USRichTextBlockDecorator>>, DecoClasses)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual ~SNPCDialogueBox() {
		StyleInstance.Reset();
	}


private:
////////////////////////// Text Block ////////////////////////
	TSharedPtr<class STextBlock> DialoguerNameBlock;
	TSharedPtr<class SRichTextBlock> DialogueTextBlock;

	TWeakObjectPtr<UDataTable> TextStyleSet;
	FTextBlockStyle TextBlockStyle;

	TSharedPtr<class FSlateStyleSet> StyleInstance;

	TArray<TSubclassOf<USRichTextBlockDecorator>> DecoClasses;
	
	TArray<TObjectPtr<class USRichTextBlockDecorator>> InstanceDecorators;

	bool bNeedUpdateStyle = false;
	bool bNeedUpdateDecos = false;
	
private:
	void MakeStyleInstance();
	void MakeDecoInstance(TArray< TSharedRef<class ITextDecorator > >& OutDecorators);

	void UpdateDialogueTextBlock();

public:
	void SetTextStyleSet(class UDataTable* NewTextStyleSet);
	void SetDecorators(const TArray<TSubclassOf<class USRichTextBlockDecorator>>& NewDecoClasses);


/////////////////////////// Text /////////////////////////////
private:
	FString TargetText;
	FString CurrentText;

	bool bIsDecorating = false;
	bool bIsAnimating = false;


public:
	void SetDialogueText(const FString& NewTarget);

	bool IsAnimating() const {return bIsAnimating;}

	void SetDialoguerName(const FString& NewName);

	bool IsDialogueAnimating() const { return bIsAnimating; }
	
	void AnimText();
	void CompleteAnimText();
};