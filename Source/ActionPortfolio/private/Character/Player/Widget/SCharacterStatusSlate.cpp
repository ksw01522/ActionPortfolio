// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Widget/SCharacterStatusSlate.h"
#include "SlateOptMacros.h"
#include "Items/Equipment/Item_Equipment.h"
#include "Items/Widget/SItemSlot.h"
#include "Widgets/Text/STextBlock.h"
#include "AbilitySystemComponent.h"
#include "Ability/ActionPFAttributeSet.h"
#include "Character/CharacterStatusComponent.h"
#include "ActionPortfolio.h"

SLATE_IMPLEMENT_WIDGET(SCharacterStatusSlate)
void SCharacterStatusSlate::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCharacterStatusSlate::Construct(const FArguments& InArgs)
{
	SAssignNew(SlotBox, SHorizontalBox);
	for (EEquipmentPart Part : TEnumRange<EEquipmentPart>())
	{
		TSharedPtr<SItemSlot> TempSlot;

		SAssignNew(TempSlot, SItemSlot)
		.BackgroundBrush(InArgs._SlotBackgroundBrush);

		TempSlot->GetOnFocusedItemSlot().AddSP(this, &SCharacterStatusSlate::NativeOnFocusedEquipmentSlot);

		SlotBox->AddSlot()
		.Padding(InArgs._SlotPadding)
		.FillWidth(1)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			TempSlot.ToSharedRef()
		];

		EquipmentSlots.Add(Part, MoveTemp(TempSlot));
	}

	ETextJustify::Type TextJustify = ETextJustify::Left;

	ChildSlot
	[
		
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SlotBox.ToSharedRef()
		]
		
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(InArgs._TextBlockPadding)
		.FillHeight(1)
		[
			SAssignNew(HP, STextBlock)
			.TextStyle(InArgs._TextStyle)
			.Justification(TextJustify)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(InArgs._TextBlockPadding)
		.FillHeight(1)
		[
			SAssignNew(Stamina, STextBlock)
			.TextStyle(InArgs._TextStyle)
			.Justification(TextJustify)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(InArgs._TextBlockPadding)
		.FillHeight(1)
		[
			SAssignNew(Attack, STextBlock)
			.TextStyle(InArgs._TextStyle)
			.Justification(TextJustify)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(InArgs._TextBlockPadding)
		.FillHeight(1)
		[
			SAssignNew(Defense, STextBlock)
			.TextStyle(InArgs._TextStyle)
			.Justification(TextJustify)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(InArgs._TextBlockPadding)
		.FillHeight(1)
		[
			SAssignNew(FireResistance, STextBlock)
			.TextStyle(InArgs._TextStyle)
			.Justification(TextJustify)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(InArgs._TextBlockPadding)
		.FillHeight(1)
		[
			SAssignNew(IceResistance, STextBlock)
			.TextStyle(InArgs._TextStyle)
			.Justification(TextJustify)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(InArgs._TextBlockPadding)
		.FillHeight(1)
		[
			SAssignNew(ElectricResistance, STextBlock)
			.TextStyle(InArgs._TextStyle)
			.Justification(TextJustify)
		]
	];

	ResetTexts();

	BindStatusComponent(InArgs._BindStatus);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

SItemSlot* SCharacterStatusSlate::GetEquipmentSlot(EEquipmentPart Part) const
{
	if(!EquipmentSlots.Contains(Part)) return nullptr;
	
	return EquipmentSlots.Find(Part)->Get();
}

void SCharacterStatusSlate::SetSlotBackgroundBrush(const FSlateBrush* InBrush)
{
	for (auto& Slot : EquipmentSlots)
	{
		Slot.Value->SetBackgroundBrush(InBrush);
	}
}

void SCharacterStatusSlate::SetSlotPadding(const FMargin& NewPadding)
{
	FChildren* Children = SlotBox->GetChildren();
	for (int i = 0; i < Children->Num(); i++)
	{
		StaticCast<SHorizontalBox::FSlot&>(const_cast<FSlotBase&>( Children->GetSlotAt(i))).SetPadding(NewPadding);
	}
}

void SCharacterStatusSlate::SetTextStyle(const FTextBlockStyle* InStyle)
{
	HP->SetTextStyle(InStyle);
	Stamina->SetTextStyle(InStyle);
	Attack->SetTextStyle(InStyle);
	Defense->SetTextStyle(InStyle);
	FireResistance->SetTextStyle(InStyle);
	IceResistance->SetTextStyle(InStyle);
	ElectricResistance->SetTextStyle(InStyle);
}

void SCharacterStatusSlate::SetTextBlockPadding(const FMargin& NewPadding)
{
	FChildren* Children = ChildSlot.GetChildAt(0)->GetChildren();
	for (int i = 1; i < Children->Num(); i++)
	{
		StaticCast<SVerticalBox::FSlot&>(const_cast<FSlotBase&>(Children->GetSlotAt(i))).SetPadding(NewPadding);
	}
}

void SCharacterStatusSlate::ClearBindComponent()
{
	if (BindedStatus.IsValid())
	{
		if (UAbilitySystemComponent* PrevASC = BindedStatus->GetAbilitySystemComponent())
		{
			for (auto& Handle : Handles)
			{
				PrevASC->GetGameplayAttributeValueChangeDelegate(Handle.Attribute).Remove(Handle.DelegateHandle);
			}
		}

		Handles.Empty();
	}

	ResetTexts();

	for (auto& Slot : EquipmentSlots)
	{
		Slot.Value->BindItemSlot(nullptr);
	}

	BindedStatus.Reset();
}

void SCharacterStatusSlate::ResetTexts()
{
	FText HPText = FText::FromString(FString::Printf(TEXT("체력 :  / ")));
	HP->SetText(HPText);

	FText StaminaText = FText::FromString(FString::Printf(TEXT("기력 :  / ")));
	Stamina->SetText(StaminaText);

	FText AttackText = FText::FromString(FString::Printf(TEXT("공격력 : ")));
	Attack->SetText(AttackText);

	FText DefenseText = FText::FromString(FString::Printf(TEXT("방어력 : ")));
	Defense->SetText(DefenseText);

	FText FireText = FText::FromString(FString::Printf(TEXT("화염 저항 : ")));
	FireResistance->SetText(FireText);

	FText IceText = FText::FromString(FString::Printf(TEXT("얼음 저항 : ")));
	IceResistance->SetText(IceText);

	FText ElecText = FText::FromString(FString::Printf(TEXT("전기 저항 : ")));
	ElectricResistance->SetText(ElecText);
}

void SCharacterStatusSlate::BindStatusComponent(UCharacterStatusComponent* Target)
{
	if(BindedStatus == Target) return;

	ClearBindComponent();
	
	BindedStatus = Target;

	if (BindedStatus.IsValid())
	{
		UAbilitySystemComponent* ASC = BindedStatus->GetAbilitySystemComponent();
		check(ASC);

		FGameplayAttribute HPAT = UCharacterAttributeSet::GetHealthAttribute();
		FDelegateHandle HPHandle = ASC->GetGameplayAttributeValueChangeDelegate(HPAT).AddSP(this, &SCharacterStatusSlate::OnHPValueChanged);
		Handles.Emplace(HPAT, MoveTemp(HPHandle));
		bool bFound = false;
		CurrentHP = ASC->GetGameplayAttributeValue(HPAT, bFound);

		FGameplayAttribute MaxHPAT = UCharacterAttributeSet::GetMaxHealthAttribute();
		FDelegateHandle MaxHPHandle = ASC->GetGameplayAttributeValueChangeDelegate(MaxHPAT).AddSP(this, &SCharacterStatusSlate::OnMaxHPValueChanged);
		MaxHP = ASC->GetGameplayAttributeValue(MaxHPAT, bFound);
		Handles.Emplace(MaxHPAT, MoveTemp(MaxHPHandle));
		UpdateHPText();

		FGameplayAttribute StaminaAT = UCharacterAttributeSet::GetStaminaAttribute();
		FDelegateHandle StaminaHandle = ASC->GetGameplayAttributeValueChangeDelegate(StaminaAT).AddSP(this, &SCharacterStatusSlate::OnStaminaValueChanged);
		Handles.Emplace(StaminaAT, MoveTemp(StaminaHandle));
		CurrentStamina = ASC->GetGameplayAttributeValue(StaminaAT, bFound);

		FGameplayAttribute MaxStaminaAT = UCharacterAttributeSet::GetMaxStaminaAttribute();
		FDelegateHandle MaxStaminaHandle = ASC->GetGameplayAttributeValueChangeDelegate(MaxStaminaAT).AddSP(this, &SCharacterStatusSlate::OnMaxStaminaValueChanged);
		Handles.Emplace(MaxStaminaAT, MoveTemp(MaxStaminaHandle));
		MaxStamina = ASC->GetGameplayAttributeValue(MaxStaminaAT, bFound);
		UpdateStaminaText();

		FGameplayAttribute AttackAT = UCharacterAttributeSet::GetAttackPAttribute();
		FDelegateHandle AttackHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttackAT).AddSP(this, &SCharacterStatusSlate::OnAttackValueChanged);
		Handles.Emplace(AttackAT, MoveTemp(AttackHandle));
		float AttackP = ASC->GetGameplayAttributeValue(AttackAT, bFound);
		SetAttackText(AttackP);

		FGameplayAttribute DefenseAT = UCharacterAttributeSet::GetDefensePAttribute();
		FDelegateHandle DefenseHandle = ASC->GetGameplayAttributeValueChangeDelegate(DefenseAT).AddSP(this, &SCharacterStatusSlate::OnDefenseValueChanged);
		Handles.Emplace(DefenseAT, MoveTemp(DefenseHandle));
		float DefenseP = ASC->GetGameplayAttributeValue(DefenseAT, bFound);
		SetDefenseText(DefenseP);

		FGameplayAttribute FireResistanceAT = UCharacterAttributeSet::GetFireResistanceAttribute();
		FDelegateHandle FireResistanceHandle = ASC->GetGameplayAttributeValueChangeDelegate(FireResistanceAT).AddSP(this, &SCharacterStatusSlate::OnFireResistanceValueChanged);
		Handles.Emplace(FireResistanceAT, MoveTemp(FireResistanceHandle));
		float FireR = ASC->GetGameplayAttributeValue(FireResistanceAT, bFound);
		SetFireResistanceText(FireR);

		FGameplayAttribute IceResistanceAT = UCharacterAttributeSet::GetIceResistanceAttribute();
		FDelegateHandle IceResistanceHandle = ASC->GetGameplayAttributeValueChangeDelegate(IceResistanceAT).AddSP(this, &SCharacterStatusSlate::OnIceResistanceValueChanged);
		Handles.Emplace(IceResistanceAT, MoveTemp(IceResistanceHandle));
		float IceR = ASC->GetGameplayAttributeValue(IceResistanceAT, bFound);
		SetIceResistanceText(IceR);

		FGameplayAttribute ElectricResistanceAT = UCharacterAttributeSet::GetElectricResistanceAttribute();
		FDelegateHandle ElectricResistanceHandle = ASC->GetGameplayAttributeValueChangeDelegate(ElectricResistanceAT).AddSP(this, &SCharacterStatusSlate::OnElectricResistanceValueChanged);
		Handles.Emplace(ElectricResistanceAT, MoveTemp(ElectricResistanceHandle));
		float ElectricR = ASC->GetGameplayAttributeValue(ElectricResistanceAT, bFound);
		SetElectricResistanceText(ElectricR);

		TArray<UEquipmentSlot*> TargetSlots;
		BindedStatus->GetEquipmentSlots(TargetSlots);

		for (auto& TargetSlot : TargetSlots)
		{
			EquipmentSlots[TargetSlot->GetEquipmentPart()]->BindItemSlot(TargetSlot);
		}
	}
}



SCharacterStatusSlate::~SCharacterStatusSlate()
{
	ClearBindComponent();
}

void SCharacterStatusSlate::OnHPValueChanged(const FOnAttributeChangeData& Data)
{
	CurrentHP = Data.NewValue;

	UpdateHPText();
}

void SCharacterStatusSlate::OnMaxHPValueChanged(const FOnAttributeChangeData& Data)
{
	MaxHP = Data.NewValue;

	UpdateHPText();
}

void SCharacterStatusSlate::UpdateHPText()
{
	FText NewText = FText::FromString(FString::Printf(TEXT("체력 : %.0f / %.0f"), CurrentHP, MaxHP));

	HP->SetText(NewText);
}

void SCharacterStatusSlate::OnStaminaValueChanged(const FOnAttributeChangeData& Data)
{
	CurrentStamina = Data.NewValue;
}

void SCharacterStatusSlate::OnMaxStaminaValueChanged(const FOnAttributeChangeData& Data)
{
	MaxStamina = Data.NewValue;
}

void SCharacterStatusSlate::UpdateStaminaText()
{
	FText NewText = FText::FromString(FString::Printf(TEXT("기력 : %.0f / %.0f"), CurrentStamina, MaxStamina));

	Stamina->SetText(NewText);
}

void SCharacterStatusSlate::OnAttackValueChanged(const FOnAttributeChangeData& Data)
{
	SetAttackText(Data.NewValue);
}

void SCharacterStatusSlate::SetAttackText(float InPoint)
{
	FText NewText = FText::FromString(FString::Printf(TEXT("공격력 : %.0f"), InPoint));

	Attack->SetText(NewText);
}

void SCharacterStatusSlate::OnDefenseValueChanged(const FOnAttributeChangeData& Data)
{
	SetDefenseText(Data.NewValue);
}

void SCharacterStatusSlate::SetDefenseText(float InPoint)
{
	FText NewText = FText::FromString(FString::Printf(TEXT("방어력 : %.0f"), InPoint));
	Defense->SetText(NewText);
}

void SCharacterStatusSlate::OnFireResistanceValueChanged(const FOnAttributeChangeData& Data)
{
	SetFireResistanceText(Data.NewValue);
}

void SCharacterStatusSlate::SetFireResistanceText(float InPoint)
{
	FText NewText = FText::FromString(FString::Printf(TEXT("화염 저항 : %.0f"), InPoint));

	FireResistance->SetText(NewText);
}

void SCharacterStatusSlate::OnIceResistanceValueChanged(const FOnAttributeChangeData& Data)
{
	SetIceResistanceText(Data.NewValue);
}

void SCharacterStatusSlate::SetIceResistanceText(float InPoint)
{
	FText NewText = FText::FromString(FString::Printf(TEXT("얼음 저항 : %.0f"), InPoint));

	IceResistance->SetText(NewText);
}

void SCharacterStatusSlate::OnElectricResistanceValueChanged(const FOnAttributeChangeData& Data)
{
	SetElectricResistanceText(Data.NewValue);
}

void SCharacterStatusSlate::SetElectricResistanceText(float InPoint)
{
	FText NewText = FText::FromString(FString::Printf(TEXT("전기 저항 : %.0f"), InPoint));

	ElectricResistance->SetText(NewText);

}

void SCharacterStatusSlate::NativeOnFocusedEquipmentSlot(UItemSlot* FocusedSlot)
{
	if(OnFocsedEqipmentSlot.IsBound()) OnFocsedEqipmentSlot.Broadcast(FocusedSlot);
}

FOnAttributeChangedDelegateStruct::FOnAttributeChangedDelegateStruct(const FGameplayAttribute& InAttribute, const FDelegateHandle& InHandle) : Attribute(InAttribute), DelegateHandle(InHandle)
{
}
