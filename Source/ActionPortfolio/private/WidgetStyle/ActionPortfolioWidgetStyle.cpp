// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "Misc/Paths.h"
#include "Fonts/SlateFontInfo.h"
#include "Brushes/SlateBoxBrush.h"
#include "ActionPortfolio.h"
#include "Widgets/Layout/Anchors.h"
#include "Items/ItemDeveloperSetting.h"

TSharedPtr<FSlateStyleSet> FActionPortfolioWidgetStyle::StyleSet = nullptr;

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush(StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( StyleSet->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( StyleSet->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )



void FActionPortfolioWidgetStyle::Initialize()
{
#if WITH_EDITOR
	if (StyleSet.IsValid()) {
		Shutdown();
	}
#endif
	if (StyleSet.IsValid()) { return; }
	StyleSet = MakeShareable(new FSlateStyleSet("ActionPFWidgetStyleSet"));
	StyleSet->SetContentRoot(FPaths::ProjectDir() + TEXT("Content/WidgetStyle"));

	//Initialize Dialogue Widget
	{
		//Text Size
		const float TextSize_Small = 18;
		const float TextSize_Normal = 24;
		const float TextSize_Big = 30;

		StyleSet->Set(ActionPFStyle::DefaultTextSize::Small, TextSize_Small);
		StyleSet->Set(ActionPFStyle::DefaultTextSize::Normal, TextSize_Normal);
		StyleSet->Set(ActionPFStyle::DefaultTextSize::Big, TextSize_Big);


		//Dialogue Box Style	
		FSlateFontInfo DialogueDefaultFont(TTF_FONT("NPCInteract/Font/Default", 28));
		FLinearColor DialogueDefaultColor(1, 0.6f, 0, 1);

		FTextBlockStyle DialogueDefaultTextStyle;
		DialogueDefaultTextStyle.SetFont(DialogueDefaultFont);
		DialogueDefaultTextStyle.SetColorAndOpacity(FSlateColor(DialogueDefaultColor));
		DialogueDefaultTextStyle.SetFontSize(TextSize_Normal);

		StyleSet->Set(DialogueStyle::TextStyle::Default, DialogueDefaultTextStyle);

		FSlateBoxBrush* DialogueBoxBrush = new BOX_BRUSH("NPCInteract/BorderDefault", FMargin(0.05f), FSlateColor(FLinearColor(1, 1, 1)));

		StyleSet->Set(DialogueStyle::BlockStyle::Default, DialogueBoxBrush);

		//대답 버튼 스타일
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
	}

	//Initialize Interaction Widget
	{	
		//Text Style
		const float TextSize = 24;

		FSlateFontInfo DefaultFont(TTF_FONT("NPCInteract/Font/Default", 28));
		FLinearColor DefaultColor(1, 0.6f, 0, 1);

		FTextBlockStyle DefaultTextStyle;
		DefaultTextStyle.SetFont(DefaultFont);
		DefaultTextStyle.SetColorAndOpacity(FSlateColor(DefaultColor));
		DefaultTextStyle.SetFontSize(TextSize);

		StyleSet->Set(InteractionStyle::TextStyle::Default, DefaultTextStyle);

		//Button Style
		FSlateBoxBrush DefaultBTNStyle_Normal = BOX_BRUSH("NPCInteract/Normal", FVector2D(32, 32), 8.0f / 32.0f);
		FSlateBoxBrush DefaultBTNStyle_Hovered = BOX_BRUSH("NPCInteract/Hovered", FVector2D(32, 32), 8.0f / 32.0f);
		FSlateBoxBrush DefaultBTNStyle_Pressed = BOX_BRUSH("NPCInteract/Pressed", FVector2D(32, 32), 8.0f / 32.0f);

		//기본 버튼 스타일
		const FButtonStyle DefaultBTNStyle = FButtonStyle()
			.SetNormal(DefaultBTNStyle_Normal)
			.SetHovered(DefaultBTNStyle_Hovered)
			.SetPressed(DefaultBTNStyle_Pressed)
			.SetNormalPadding(FMargin(2, 2, 2, 2))
			.SetPressedPadding(FMargin(2, 3, 2, 1));

		StyleSet->Set(InteractionStyle::ButtonStyle::Default, DefaultBTNStyle);

		FVector2D ButtonSize(250, 80);
		StyleSet->Set(InteractionStyle::ButtonStyle::DefaultSize, ButtonSize);

		FMargin ButtonPadding(0,10);
		StyleSet->Set(InteractionStyle::ButtonStyle::Padding, ButtonPadding);
	}

	//Initialize Inventory Widget
	{
		//Inventory 관련 초기화
		FVector2D InventorySlotSize(64, 64);

		//Slot Item Grade Frame
		{
			FSlateImageBrush* InventorySlotBrush = new IMAGE_BRUSH("Inventory/InventorySlot", InventorySlotSize);
			StyleSet->Set(InventoryStyle::SlotBackground, InventorySlotBrush);

			FSlateImageBrush* CommonFrameBrush = new IMAGE_BRUSH("Inventory/ItemFrame/Common", InventorySlotSize);
			StyleSet->Set(InventoryStyle::SlotFrameGrade::Common, CommonFrameBrush);

			FSlateImageBrush* UncommonFrameBrush = new IMAGE_BRUSH("Inventory/ItemFrame/Uncommon", InventorySlotSize);
			StyleSet->Set(InventoryStyle::SlotFrameGrade::Uncommon, UncommonFrameBrush);

			FSlateImageBrush* RareFrameBrush = new IMAGE_BRUSH("Inventory/ItemFrame/Rare", InventorySlotSize);
			StyleSet->Set(InventoryStyle::SlotFrameGrade::Rare, RareFrameBrush);

			FSlateImageBrush* UniqueFrameBrush = new IMAGE_BRUSH("Inventory/ItemFrame/Unique", InventorySlotSize);
			StyleSet->Set(InventoryStyle::SlotFrameGrade::Unique, UniqueFrameBrush);

			FSlateImageBrush* LegendaryFrameBrush = new IMAGE_BRUSH("Inventory/ItemFrame/Legendary", InventorySlotSize);
			StyleSet->Set(InventoryStyle::SlotFrameGrade::Legendary, LegendaryFrameBrush);

			FSlateImageBrush* EpicFrameBrush = new IMAGE_BRUSH("Inventory/ItemFrame/Epic", InventorySlotSize);
			StyleSet->Set(InventoryStyle::SlotFrameGrade::Epic, EpicFrameBrush);
		}

		//인벤토리 슬롯 아이템 카운트 텍스트 블록
		{
			FSlateFontInfo CountTextBlockFont(TTF_FONT("NPCInteract/Font/Default", 12));
			FLinearColor CountTextBlockColor(0.7f, 0.4f, 0, 1);

			FTextBlockStyle CountTextBlockStyle;
			CountTextBlockStyle.SetFont(CountTextBlockFont);
			CountTextBlockStyle.SetColorAndOpacity(CountTextBlockColor);
			CountTextBlockStyle.SetShadowOffset(FVector2D(0.1f, 0.1f));

			StyleSet->Set(InventoryStyle::CountTextStyle, CountTextBlockStyle);

			
			UMaterialInterface* MeshCaptureMaterial = GetDefault<UItemDeveloperSetting>()->GetMeshCaptureMaterial().LoadSynchronous();
			FSlateImageBrush* MeshCaptureBrush = new FSlateImageBrush(MeshCaptureMaterial, FVector2D(64,64));

			StyleSet->Set(InventoryStyle::InformationSlate::MeshCaptureStyle, MeshCaptureBrush);

			FSlateFontInfo NameTextBlockFont(TTF_FONT("NPCInteract/Font/Default", 12));
			FTextBlockStyle NameTextBlockStyle;
			NameTextBlockStyle.SetFont(NameTextBlockFont);
			NameTextBlockStyle.SetShadowOffset(FVector2D(0.1f, 0.1f));

			StyleSet->Set(InventoryStyle::InformationSlate::NameTextStyle, NameTextBlockStyle);

			FSlateFontInfo DescriptionTextBlockFont(TTF_FONT("NPCInteract/Font/Default", 12));
			FTextBlockStyle DescriptionTextBlockStyle;
			DescriptionTextBlockStyle.SetFont(DescriptionTextBlockFont);
			DescriptionTextBlockStyle.SetShadowOffset(FVector2D(0.1f, 0.1f));

			StyleSet->Set(InventoryStyle::InformationSlate::DescriptionTextStyle, DescriptionTextBlockStyle);
		}
	}

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
