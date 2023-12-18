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

}