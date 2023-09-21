// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/AppStyle.h"
#include "Styling/SlateStyle.h"


class FActionPortfolioWidgetStyle
{
public:
	static void Initialize();
	static void Shutdown();

	static const FName& GetStyleSetName();

private:
	static TSharedPtr<FSlateStyleSet> StyleSet;

public:
	static FSlateStyleSet* GetActionPFWidgetStyleSet();
	static FButtonStyle* MakeDefaultButtonStyle();
};

namespace DialoguerNameStyle 
{
	namespace Font
	{
		static const FName Default = "Dialogue.Name.Font.Default";
	};

	namespace Color
	{
		static const FName Default = "Dialogue.Name.Color.Default";
	};
};

namespace CustomUIStyle
{
	namespace BorderImage
	{
		static const FName Default = "Dialogue.BorderImage.Default";
	}

	namespace Button
	{
		static const FName Normal = "NPCInteract.Button.Normal";
		static const FName Hovered = "NPCInteract.Button.Hovered";
		static const FName Pressed = "NPCInteract.Button.Pressed";
	}
}