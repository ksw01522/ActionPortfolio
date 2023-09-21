// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "Misc/Paths.h"
#include "Fonts/SlateFontInfo.h"
#include "Brushes/SlateBoxBrush.h"
#include "ActionPortfolio.h"

TSharedPtr<FSlateStyleSet> FActionPortfolioWidgetStyle::StyleSet = nullptr;

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush(StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( StyleSet->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( StyleSet->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )



void FActionPortfolioWidgetStyle::Initialize()
{
	if (StyleSet.IsValid()) { return; }

	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);

	StyleSet = MakeShareable(new FSlateStyleSet("ActionPFWidgetStyleSet"));

	StyleSet->SetContentRoot(FPaths::ProjectDir() / TEXT("Content/WidgetStyle"));


	//Dialogue Box Style	
	StyleSet->Set(DialoguerNameStyle::Font::Default, TTF_FONT("NPCInteract/Font/Default", 28));
	
	StyleSet->Set(DialoguerNameStyle::Color::Default, FLinearColor(1, 0.6f, 0, 1));

	FSlateBoxBrush* DialogueBoxBrush = new BOX_BRUSH("NPCInteract/BorderDefault", FMargin(0.07));
	DialogueBoxBrush->TintColor = FLinearColor(1,1,1,1);

	StyleSet->Set(CustomUIStyle::BorderImage::Default, DialogueBoxBrush);

	//Button Style
	StyleSet->Set(CustomUIStyle::Button::Normal, new BOX_BRUSH("NPCInteract/Normal", FMargin(0.1f)));
	StyleSet->Set(CustomUIStyle::Button::Hovered, new BOX_BRUSH("NPCInteract/Hovered", FMargin(0.1f)));
	StyleSet->Set(CustomUIStyle::Button::Pressed, new BOX_BRUSH("NPCInteract/Pressed", FMargin(0.1f)));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FActionPortfolioWidgetStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

const FName& FActionPortfolioWidgetStyle::GetStyleSetName()
{
	return StyleSet->GetStyleSetName();
}

FSlateStyleSet* FActionPortfolioWidgetStyle::GetActionPFWidgetStyleSet()
{
	return StyleSet.Get();
}

FButtonStyle* FActionPortfolioWidgetStyle::MakeDefaultButtonStyle()
{
	FSlateStyleSet* ActionPFSlateStyleSet = FActionPortfolioWidgetStyle::GetActionPFWidgetStyleSet();

	FButtonStyle* DefaultBTNStyle = new FButtonStyle();
	DefaultBTNStyle->SetNormal(*ActionPFSlateStyleSet->GetBrush(CustomUIStyle::Button::Normal));
	DefaultBTNStyle->SetHovered(*ActionPFSlateStyleSet->GetBrush(CustomUIStyle::Button::Hovered));
	DefaultBTNStyle->SetPressed(*ActionPFSlateStyleSet->GetBrush(CustomUIStyle::Button::Pressed));

	return DefaultBTNStyle;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
