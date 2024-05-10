// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Widget/SAbilityIcon.h"
#include "SlateOptMacros.h"
#include "Brushes/SlateImageBrush.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "ActionPortfolio.h"

TSharedPtr<SAbilityIcon> SAbilityIcon::EmptyAbilityIcon = nullptr;

SLATE_IMPLEMENT_WIDGET(SAbilityIcon)
void SAbilityIcon::PrivateRegisterAttributes(FSlateAttributeInitializer& Initailizer)
{

}


SAbilityIcon::SAbilityIcon()
{
	SetCanTick(true);
	IconBrush = nullptr;
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAbilityIcon::Construct(const FArguments& InArgs)
{
	Ability = InArgs._Ability;
	SetVisibility(EVisibility::HitTestInvisible);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAbilityIcon::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (!Ability.IsValid())
	{
		IconBrush = nullptr;
		return;
	}

	IconBrush = Ability->GetAbilityIconBrush(AbilitySystem);
	if (AbilitySystem.IsValid())
	{
		if(AbilitySystem->GetCooldownRemainingAndDurationByTag(*Ability->GetCooldownTags(), RemainCoolTime, CoolTimeDuration))
		{ }
	
		bCanAct = AbilitySystem->CanActivateAbility(Ability->GetClass());
	}
	
			
	
}

FVector2D SAbilityIcon::ComputeDesiredSize(float) const
{
	if ((IconBrush != nullptr))
	{
		return IconSizeOverride.IsSet() ? IconSizeOverride.GetValue() : IconBrush->ImageSize;
	}

	return FVector2D::ZeroVector;
}



void SAbilityIcon::SetIconSize(FVector2D InSize)
{
	IconSizeOverride = InSize;
}

void SAbilityIcon::LinkAbility(UActionPFGameplayAbility* InAbility)
{
	Ability = InAbility;
	if (Ability.IsValid())
	{
		IconBrush = Ability->GetAbilityIconBrush(AbilitySystem);
	}
	else
	{
		IconBrush = nullptr;
	}
}

void SAbilityIcon::LinkAbilitySystem(UActionPFAbilitySystemComponent* InSystem)
{
	AbilitySystem = InSystem;
}

int32 SAbilityIcon::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if(!AbilitySystem.IsValid() || !Ability.IsValid()) return LayerId;

	int32 NewLayerId = LayerId;

	const bool bEnabled = ShouldBeEnabled(bParentEnabled);
	const ESlateDrawEffect DrawEffects = bEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

	if ((IconBrush != nullptr) && (IconBrush->DrawAs != ESlateBrushDrawType::NoDrawType))
	{
		const static FLinearColor DisactableColor(0.125, 0.125, 0.125, 1);
		const FLinearColor IconColor = bCanAct ? FLinearColor::White : DisactableColor;
		const FLinearColor FinalColorAndOpacity(InWidgetStyle.GetColorAndOpacityTint() * IconColor * IconBrush->GetTint(InWidgetStyle));

		FSlateDrawElement::MakeBox(OutDrawElements, NewLayerId, AllottedGeometry.ToPaintGeometry(), IconBrush, DrawEffects, FinalColorAndOpacity);
	
		if (AbilitySystem.IsValid() && !bCanAct && 0 < RemainCoolTime)
		{
			NewLayerId += 2;
			static const FSlateBrush CoolCoverBrush = FSlateImageBrush(NAME_None, FVector2D(32), (FSlateColor)(FLinearColor(0, 0, 0, 0.5)));
			const FLinearColor CoolCoverColorAndOpacity(InWidgetStyle.GetColorAndOpacityTint() * CoolCoverBrush.GetTint(InWidgetStyle));

			FGeometry CoolCoverGeometry = AllottedGeometry.MakeChild(FSlateRenderTransform(FScale2f(1, RemainCoolTime / CoolTimeDuration)), FVector2D(0.5, 1));

			FSlateDrawElement::MakeBox(OutDrawElements, NewLayerId, CoolCoverGeometry.ToPaintGeometry(), &CoolCoverBrush, DrawEffects, CoolCoverColorAndOpacity);
		}
	}

	return NewLayerId;
}