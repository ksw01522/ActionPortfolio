// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Widget/SStateEffectIcon.h"
#include "SlateOptMacros.h"


SLATE_IMPLEMENT_WIDGET(SStateEffectIcon)
void SStateEffectIcon::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{}


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SStateEffectIcon::Construct(const FArguments& InArgs)
{
	IconBrush = InArgs._IconBrush;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FVector2D SStateEffectIcon::ComputeDesiredSize(float) const
{
	if ((IconBrush != nullptr))
	{
		return IconSizeOverride.IsSet() ? IconSizeOverride.GetValue() : IconBrush->ImageSize;
	}

	return FVector2D::ZeroVector;
}

int32 SStateEffectIcon::OnPaint(const FPaintArgs & Args, const FGeometry & AllottedGeometry, const FSlateRect & MyCullingRect, FSlateWindowElementList & OutDrawElements, int32 LayerId, const FWidgetStyle & InWidgetStyle, bool bParentEnabled) const
{
	int32 NewLayerId = LayerId;

	const bool bEnabled = ShouldBeEnabled(bParentEnabled);
	const ESlateDrawEffect DrawEffects = bEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

	if ((IconBrush != nullptr) && (IconBrush->DrawAs != ESlateBrushDrawType::NoDrawType))
	{
		const static FLinearColor DisactableColor(0.125, 0.125, 0.125, 1);
		const FLinearColor IconColor = FLinearColor::White;
		const FLinearColor FinalColorAndOpacity(InWidgetStyle.GetColorAndOpacityTint() * IconColor * IconBrush->GetTint(InWidgetStyle));

		FSlateDrawElement::MakeBox(OutDrawElements, NewLayerId, AllottedGeometry.ToPaintGeometry(), IconBrush, DrawEffects, FinalColorAndOpacity);
	}

	return NewLayerId;

}