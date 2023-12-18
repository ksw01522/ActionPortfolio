// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "Misc/Paths.h"
#include "Fonts/SlateFontInfo.h"
#include "Brushes/SlateBoxBrush.h"
#include "ActionPortfolio.h"
#include "Widgets/Layout/Anchors.h"

TSharedPtr<FSlateStyleSet> FActionPortfolioWidgetStyle::StyleSet = nullptr;

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush(StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( StyleSet->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( StyleSet->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )



void FActionPortfolioWidgetStyle::Initialize()
{
	if (StyleSet.IsValid()) { return; }
	StyleSet = MakeShareable(new FSlateStyleSet("ActionPFWidgetStyleSet"));
	StyleSet->SetContentRoot(FPaths::ProjectDir() / TEXT("Content/WidgetStyle"));

	//Text Size
	const float TextSize_Small = 18;
	const float TextSize_Normal = 24;
	const float TextSize_Big = 30;

	StyleSet->Set(FName("ActionPF.TextSize.Small"), TextSize_Small);
	StyleSet->Set(FName("ActionPF.TextSize.Normal"), TextSize_Normal);
	StyleSet->Set(FName("ActionPF.TextSize.Big"), TextSize_Big);

	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);


	//Dialogue Box Style	
	FSlateFontInfo DialogueDefaultFont(TTF_FONT("NPCInteract/Font/Default", 28));
	FLinearColor DialogueDefaultColor(1, 0.6f, 0, 1);

	FTextBlockStyle DialogueDefaultTextStyle;
	DialogueDefaultTextStyle.SetFont(DialogueDefaultFont);
	DialogueDefaultTextStyle.SetColorAndOpacity(FSlateColor(DialogueDefaultColor));
	DialogueDefaultTextStyle.SetFontSize(TextSize_Normal);

	StyleSet->Set(DialogueStyle::TextStyle::Default, DialogueDefaultTextStyle);

	FSlateBoxBrush* DialogueBoxBrush = new BOX_BRUSH("NPCInteract/BorderDefault", FMargin(0.05f), FSlateColor(FLinearColor(1,1,1)));

	StyleSet->Set(DialogueStyle::BlockStyle::Default, DialogueBoxBrush);


	//Button Style
	FSlateBoxBrush DefaultBTNStyle_Normal = BOX_BRUSH("NPCInteract/Normal", FVector2D(32,32), 8.0f / 32.0f);

	FSlateBoxBrush DefaultBTNStyle_Hovered = BOX_BRUSH("NPCInteract/Hovered", FVector2D(32, 32), 8.0f / 32.0f);

	FSlateBoxBrush DefaultBTNStyle_Pressed = BOX_BRUSH("NPCInteract/Pressed", FVector2D(32, 32), 8.0f / 32.0f);


	const FButtonStyle DefaultBTNStyle = FButtonStyle()
				.SetNormal(DefaultBTNStyle_Normal)
				.SetHovered(DefaultBTNStyle_Hovered)
				.SetPressed(DefaultBTNStyle_Pressed)
				.SetNormalPadding(FMargin(2, 2, 2, 2))
				.SetPressedPadding(FMargin(2, 3, 2, 1));

	StyleSet->Set(ActionPFStyle::ButtonStyle::Default, DefaultBTNStyle);

	FSlateBoxBrush AnswerBTNStyle_Normal = *DialogueBoxBrush;
	FSlateBoxBrush AnswerBTNStyle_Horvered = *DialogueBoxBrush;
	AnswerBTNStyle_Horvered.TintColor = FSlateColor(FLinearColor(0.75f, 0.75f, 0.75f));

	FSlateBoxBrush AnswerBTNStyle_Pressed = *DialogueBoxBrush;
	AnswerBTNStyle_Pressed.TintColor = FSlateColor(FLinearColor(0.4f, 0.4f, 0.4f));

	const FButtonStyle AnswerBTNStyle = FButtonStyle()
					.SetNormal(AnswerBTNStyle_Normal)
					.SetHovered(AnswerBTNStyle_Horvered)
					.SetPressed(AnswerBTNStyle_Pressed)
					.SetNormalPadding(FMargin(2, 2, 2, 2))
					.SetPressedPadding(FMargin(2, 3, 2, 1));

	StyleSet->Set(DialogueStyle::ButtonStyle::AnswerButton, AnswerBTNStyle);

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

FSlateStyleSet* FActionPortfolioWidgetStyle::Get()
{
	return StyleSet.Get();
}


#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
