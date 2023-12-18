// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Framework/Text/ITextDecorator.h"

/**
 * 
 */

class UNPCInteract;
class AActionPFPlayerController;



class ACTIONPORTFOLIO_API SNPCInteractButton : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNPCInteractButton)
	{}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override { return true; }


private:
	TSharedPtr<SButton> InteractButton;
	TSharedPtr<STextBlock> InteractNameBlock;


public:
	void BindNPCInteract(AActionPFPlayerController* Player, UNPCInteract* Interact);

};

