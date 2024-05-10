// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Widget/SAbilitySlot.h"
#include "SlateOptMacros.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "Widgets/SOverlay.h"
#include "Ability/Widget/SAbilityIcon.h"
#include "Ability/AbilitySlot.h"
#include "ActionPortfolio.h"
#include "Ability/ActionPFAbilityBFL.h"
#include "CustomInputSettingSubsystem.h"

SLATE_IMPLEMENT_WIDGET(SAbilitySlot)
void SAbilitySlot::PrivateRegisterAttributes(FSlateAttributeInitializer& Initailizer)
{

}

SAbilitySlot::SAbilitySlot()
{
	SetCanTick(false);
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAbilitySlot::Construct(const FArguments& InArgs)
{
	SetVisibility(EVisibility::Visible);
	
	BackgroundBrush = InArgs._BackgroundBrush;
	BGColorAndOpacity = InArgs._BackgroundColorAndOpacity;
	SlotSize = InArgs._SlotSize;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SAbilitySlot::SetBackgroundBrush(const FSlateBrush* InBrush)
{
	BackgroundBrush = InBrush;
}



FVector2D SAbilitySlot::ComputeDesiredSize(float) const
{
	return SlotSize;
}

void SAbilitySlot::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	
}

int32 SAbilitySlot::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const bool bEnabled = ShouldBeEnabled(bParentEnabled);
	const ESlateDrawEffect DrawEffects = bEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;
	int NewLayerId = LayerId;

	if ((BackgroundBrush != nullptr) && (BackgroundBrush->DrawAs != ESlateBrushDrawType::NoDrawType))
	{
		const FLinearColor FinalColorAndOpacity(InWidgetStyle.GetColorAndOpacityTint() * BGColorAndOpacity.GetColor(InWidgetStyle) * BackgroundBrush->GetTint(InWidgetStyle));

		FSlateDrawElement::MakeBox(OutDrawElements, NewLayerId, AllottedGeometry.ToPaintGeometry(), BackgroundBrush, DrawEffects, FinalColorAndOpacity);
	
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, NewLayerId, InWidgetStyle, bEnabled);
}

UAbilitySlot* SAbilitySlot::GetOwnerSlot() const
{
	return OwnerSlot.Get();
}

void SAbilitySlot::LinkOwnerSlot(UAbilitySlot* InOwner)
{
	OwnerSlot = InOwner;
}

void SAbilitySlot::UnlinkOwnerSlot(UAbilitySlot* InOwner)
{
	if (OwnerSlot == InOwner)
	{
		OwnerSlot = nullptr;
	}
}

void SAbilitySlot::SetAbilityIcon(const TSharedPtr<SAbilityIcon>& InIcon)
{
	if (InIcon.IsValid())
	{
		ChildSlot
		[
			InIcon.ToSharedRef()
		];
	}
	else
	{
		ChildSlot.DetachWidget();
	}
}



FReply SAbilitySlot::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FDragDropOperation_AbilitySlot> DDO = DragDropEvent.GetOperationAs<FDragDropOperation_AbilitySlot>();

	if (DDO.IsValid())
	{
		UActionPFAbilityBFL::TryChangeAbilitySlot(DDO->GetFromSlot(), OwnerSlot.Get());
	}
	else
	{
	}

	return FReply::Handled();
}

FReply SAbilitySlot::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && OwnerSlot.IsValid() && OwnerSlot->GetAbilityClass().GetDefaultObject() != nullptr)
	{
		TSharedRef<FDragDropOperation_AbilitySlot> DDO = FDragDropOperation_AbilitySlot::AbilitySlotDrag(OwnerSlot.Get());

		return FReply::Handled().BeginDragDrop(DDO);
	}

	return FReply::Handled();
}


void FDragDropOperation_AbilitySlot::Construct()
{
	FDragDropOperation::Construct();

	SetDecoratorVisibility(true);
}

void FDragDropOperation_AbilitySlot::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
	FDragDropOperation::OnDrop(bDropWasHandled, MouseEvent);
}

TSharedPtr<SWidget> FDragDropOperation_AbilitySlot::GetDefaultDecorator() const
{
	return FromSlot->GetAbilityIcon();
}

TSharedRef<FDragDropOperation_AbilitySlot> FDragDropOperation_AbilitySlot::AbilitySlotDrag(UAbilitySlot* InFromSlot)
{
	TSharedRef<FDragDropOperation_AbilitySlot> NewOperation = MakeShared<FDragDropOperation_AbilitySlot>();
	NewOperation->FromSlot = InFromSlot;
	NewOperation->Construct();

	return NewOperation;
}



SLATE_IMPLEMENT_WIDGET(SAbilitySlot_HotKey)
void SAbilitySlot_HotKey::PrivateRegisterAttributes(FSlateAttributeInitializer& Initailizer)
{

}

SAbilitySlot_HotKey::SAbilitySlot_HotKey()
{
	SetCanTick(false);
	CurrentHotKeyBrush = nullptr;
}

SAbilitySlot_HotKey::~SAbilitySlot_HotKey()
{
	ClearOnChangedDeviceEvent();
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAbilitySlot_HotKey::Construct(const FArguments& InArgs)
{
	SetVisibility(EVisibility::Visible);

	Super::Construct(SAbilitySlot::FArguments()
					.BackgroundBrush(InArgs._BackgroundBrush)
					.BackgroundColorAndOpacity(InArgs._BackgroundColorAndOpacity)
					.SlotSize(InArgs._SlotSize));
	
	CurrentHotKeyBrush = &HotKeyBrush_Keyboard;

	AddOnChangedDeviceEvent();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAbilitySlot_HotKey::AddOnChangedDeviceEvent()
{
	UCustomInputSettingSubsystem* CISS = UCustomInputSettingSubsystem::GetInstance();

#if WITH_EDITOR
	if (CISS == nullptr) return;
#endif

	OnChangedInputDeviceHandle = CISS->AddInputDeviceChangeEvent(TDelegate<void(EInputDeviceType)>::CreateSP(this, &SAbilitySlot_HotKey::OnCurrentDeviceChanged));

	if (CISS->GetCurrentInputDevice() == EInputDeviceType::Keyboard)
	{
		CurrentHotKeyBrush = &HotKeyBrush_Keyboard;
	}
	else
	{
		CurrentHotKeyBrush = &HotKeyBrush_Gamepad;
	}
}

void SAbilitySlot_HotKey::ClearOnChangedDeviceEvent()
{
	UCustomInputSettingSubsystem* CISS = UCustomInputSettingSubsystem::GetInstance();

#if WITH_EDITOR
	if (CISS == nullptr) return;
#endif

	CISS->RemoveInputDeviceChangeEvent(OnChangedInputDeviceHandle);
	OnChangedInputDeviceHandle.Reset();
	CurrentHotKeyBrush = nullptr;
}

void SAbilitySlot_HotKey::OnCurrentDeviceChanged(EInputDeviceType InType)
{
	if (InType == EInputDeviceType::Keyboard)
	{
		CurrentHotKeyBrush = &HotKeyBrush_Keyboard;
	}
	else
	{
		CurrentHotKeyBrush = &HotKeyBrush_Gamepad;
	}
}

int32 SAbilitySlot_HotKey::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int IconLayerId = Super::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	const bool bEnabled = ShouldBeEnabled(bParentEnabled);
	const ESlateDrawEffect DrawEffects = bEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

	if (CurrentHotKeyBrush != nullptr)
	{
		const FLinearColor FinalColorAndOpacity(InWidgetStyle.GetColorAndOpacityTint() * CurrentHotKeyBrush->GetTint(InWidgetStyle));

		FGeometry HotKeyGeometry = AllottedGeometry.MakeChild(FSlateRenderTransform(FScale2f(0.35,0.35)), FVector2D(1));

		FSlateDrawElement::MakeBox(OutDrawElements, LayerId + 2, HotKeyGeometry.ToPaintGeometry(), CurrentHotKeyBrush, DrawEffects, FinalColorAndOpacity);
	}

	return IconLayerId < LayerId + 2 ? LayerId + 2 : IconLayerId;
}

void SAbilitySlot_HotKey::SetHotKeyKeyboard(const FKey& InKey)
{
	UCustomInputSettingSubsystem* SystemInst = UCustomInputSettingSubsystem::GetInstance();

#if WITH_EDITOR
	if (SystemInst == nullptr) return;
#endif
	HotKeyBrush_Keyboard.SetResourceObject(SystemInst->GetKeyIcon(InKey));
}

void SAbilitySlot_HotKey::SetHotKeyGamepad(const FKey& InKey)
{
	UCustomInputSettingSubsystem* SystemInst = UCustomInputSettingSubsystem::GetInstance();

#if WITH_EDITOR
	if (SystemInst == nullptr) return;
#endif
	HotKeyBrush_Gamepad.SetResourceObject(SystemInst->GetKeyIcon(InKey));
}

void SAbilitySlot_HotKey::SetHotKeys(const FKey& KeyboardKey, const FKey& GamepadKey)
{
	UCustomInputSettingSubsystem* SystemInst = UCustomInputSettingSubsystem::GetInstance();

#if WITH_EDITOR
	if (SystemInst == nullptr) return;
#endif

	HotKeyBrush_Keyboard.SetResourceObject(SystemInst->GetKeyIcon(KeyboardKey));
	HotKeyBrush_Gamepad.SetResourceObject(SystemInst->GetKeyIcon(GamepadKey));
}