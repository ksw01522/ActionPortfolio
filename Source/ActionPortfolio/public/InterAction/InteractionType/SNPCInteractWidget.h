// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class ACTIONPORTFOLIO_API SNPCInteractWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNPCInteractWidget) : 
		_NPCInteracts(), _OwnerPlayer()
	{
		_NPCInteracts.Empty();
		_OwnerPlayer = nullptr;
	}

	SLATE_ARGUMENT(TArray<class UNPCInteract*>, NPCInteracts)
	SLATE_ARGUMENT(class AActionPFPlayerController*, OwnerPlayer)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	AActionPFPlayerController* OwnerPlayer;
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

	SLATE_ARGUMENT(UNPCInteract*, NPCInteract)
	SLATE_ARGUMENT(AActionPFPlayerController*, OwnerPlayer)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override { return true; }

	FReply PlayerInteractWithNPC();

protected:
	AActionPFPlayerController* OwnerPlayer;
	UNPCInteract* NPCInteract;
};