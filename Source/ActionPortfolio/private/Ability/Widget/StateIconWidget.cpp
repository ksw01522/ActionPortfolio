// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Widget/StateIconWidget.h"
#include "Ability/Widget/SStateEffectIcon.h"
#include "Components/Image.h"

void UStateIconWidget::SetIconImage(UMaterialInterface* NewImage)
{
	IconImage->SetBrushFromMaterial(NewImage);
}

void UStateIconWidget::SetIconBrush(const FSlateBrush& InBrush)
{
	IconImage->SetBrush(InBrush);
}

void UStateIconWidget::SetStateEffectStack(int NewStack)
{
	//나중에 채울지 결정
}

#if WITH_EDITOR
const FText UStateIconWidget::GetPaletteCategory()
{
	return NSLOCTEXT("ActionPF", "ActionPFAbility", "ActionPFAbility");
}
#endif



