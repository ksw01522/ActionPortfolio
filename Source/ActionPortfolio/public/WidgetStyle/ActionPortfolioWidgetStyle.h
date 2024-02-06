// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/AppStyle.h"
#include "Styling/SlateStyle.h"

struct FAnchors;

class FActionPortfolioWidgetStyle
{
public:
	static void Initialize();

	static void Shutdown();

	static const FName& GetStyleSetName();

private:
	static TSharedPtr<FSlateStyleSet> StyleSet;

public:
	static FSlateStyleSet* Get();
};



namespace DialogueStyle
{
	namespace TextStyle
	{
		static const FName Default = "Dialogue.TextStyle.Default";
	};

	namespace BlockStyle
	{
		static const FName Default = "Dialogue.BorderImage.Default";
	}

	namespace ButtonStyle
	{
		static const FName AnswerButton = "Dialogue.ButtonStyle.Answer";
	}
}

namespace ActionPFStyle
{
	namespace ButtonStyle
	{
		static const FName Default = "ActionPFStyle.ButtonStyle.Default";
	}

	namespace DefaultTextSize
	{
		static const FName Small = "ActionPFStyle.TextSize.Small";
		static const FName Normal = "ActionPFStyle.TextSize.Normal";
		static const FName Big = "ActionPFStyle.TextSize.Big";
	}
}

namespace InventoryStyle
{
	static const FName WindowBackground = "InventorySlot.WindowBackground";

	static const FName SlotBackground = "InventorySlot.SlotBackground";

	namespace SlotFrameGrade
	{
		static const FName Common = "InventorySlot.SlotFrameGrade.Common";
		static const FName Uncommon = "InventorySlot.SlotFrameGrade.Uncommon";
		static const FName Rare = "InventorySlot.SlotFrameGrade.Rare";
		static const FName Unique = "InventorySlot.SlotFrameGrade.Unique";
		static const FName Legendary = "InventorySlot.SlotFrameGrade.Legendary";
		static const FName Epic = "InventorySlot.SlotFrameGrade.Epic";
	}

	static const FName CountTextStyle = "InventorySlot.CountTextStyle";
}