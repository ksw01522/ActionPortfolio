// Fill out your copyright notice in the Description page of Project Settings.


#include "CIDSDetailCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "HAL/PlatformApplicationMisc.h"
#include "IPropertyUtilities.h"
#include "CustomInputDeveloperSettings.h"
#include "CustomInputSettingEditor.h"
#include "Widgets/SCompoundWidget.h"
#include "SlateOptMacros.h"
#include "CustomInputSettingSubsystem.h"
#include "CustomInputSettingRuntime.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Brushes/SlateImageBrush.h"
#include "CustomInputSettingSubsystem.h"
#include "Types/NavigationMetaData.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Layout/SBackgroundBlur.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"

class SCIDSMappingPreview : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SCIDSMappingPreview)
    {}

    SLATE_ARGUMENT(const FSlateBrush*, BorderBrush)
    SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, BoxSizeProperty)
    SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, BoxPaddingProperty)
    SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, NameTextStyleProperty)
    SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, GuideTextStyleProperty)
    SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, DefaultColorProperty)
    SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, ErrorColorProperty)

    SLATE_END_ARGS()

private:
   TSharedPtr<IPropertyHandle> BoxSizeProperty;
   FVector2D CachedBoxSize;

   TSharedPtr<IPropertyHandle> BoxPaddingProperty;
   FMargin CachedBoxPadding;

   TSharedPtr<IPropertyHandle> NameTextStyleProperty;
   FTextBlockStyle CachedNameTextStyle;

   TSharedPtr<IPropertyHandle> GuideTextStyleProperty;
   FTextBlockStyle CachedGuideTextStyle;

   TSharedPtr<IPropertyHandle> DefaultColorProperty;
   FSlateColor CachedDefaultColor;

   TSharedPtr<IPropertyHandle> ErrorColorProperty;
   FSlateColor CachedErrorColor;

   TSharedPtr<SBox> SizeBox;
   TSharedPtr<SBorder> Border;
   TSharedPtr<STextBlock> NameTextBlock;
   TSharedPtr<STextBlock> GuideTextBlock;

public:

    void Construct(const FArguments& InArgs)
    {
        BoxSizeProperty = InArgs._BoxSizeProperty;
        BoxPaddingProperty = InArgs._BoxPaddingProperty;
        NameTextStyleProperty = InArgs._NameTextStyleProperty;
        GuideTextStyleProperty = InArgs._GuideTextStyleProperty;
        DefaultColorProperty = InArgs._DefaultColorProperty;
        ErrorColorProperty = InArgs._ErrorColorProperty;

        FSimpleDelegate OnBoxSizeChangedDelegate = FSimpleDelegate::CreateSP(this, &SCIDSMappingPreview::OnBoxSizeChanged);
        BoxSizeProperty->SetOnChildPropertyValueChanged(OnBoxSizeChangedDelegate);

        FSimpleDelegate OnBoxPaddingChangedDelegate = FSimpleDelegate::CreateSP(this, &SCIDSMappingPreview::OnBoxPaddingChanged);
        BoxPaddingProperty->SetOnChildPropertyValueChanged(OnBoxPaddingChangedDelegate);

        FSimpleDelegate OnNameTextStyleChangedDelegate = FSimpleDelegate::CreateSP(this, &SCIDSMappingPreview::OnNameTextStyleChanged);
        NameTextStyleProperty->SetOnChildPropertyValueChanged(OnNameTextStyleChangedDelegate);

        FSimpleDelegate OnGuideTextStyleChangedDelegate = FSimpleDelegate::CreateSP(this, &SCIDSMappingPreview::OnGuideTextStyleChanged);
        GuideTextStyleProperty->SetOnChildPropertyValueChanged(OnGuideTextStyleChangedDelegate);

        FSimpleDelegate OnDefaultColorChangedDelegate = FSimpleDelegate::CreateSP(this, &SCIDSMappingPreview::OnDefaultColorChanged);
        DefaultColorProperty->SetOnChildPropertyValueChanged(OnDefaultColorChangedDelegate);

        FSimpleDelegate OnErrorColorChangedDelegate = FSimpleDelegate::CreateSP(this, &SCIDSMappingPreview::OnErrorColorChanged);
        ErrorColorProperty->SetOnChildPropertyValueChanged(OnErrorColorChangedDelegate);

        CachePropertyValues();

        SetVisibility(EVisibility::HitTestInvisible);

        ChildSlot
        [
            SNew(SBackgroundBlur)
            .bApplyAlphaToBlur(true)
            .BlurStrength(10)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            [
                SAssignNew(SizeBox, SBox)
                    .HAlign(HAlign_Fill)
                    .VAlign(VAlign_Fill)
                    .MinDesiredWidth(CachedBoxSize.X)
                    .MinDesiredHeight(CachedBoxSize.Y)
                    [
                        SAssignNew(Border, SBorder)
                            .Padding(CachedBoxPadding)
                            .BorderImage(InArgs._BorderBrush)
                            .BorderBackgroundColor(FLinearColor(1, 1, 1, 1))
                            .ColorAndOpacity(FLinearColor(1, 1, 1, 1))
                            .HAlign(HAlign_Fill)
                            .VAlign(VAlign_Fill)
                            [
                                SNew(SVerticalBox)
                                    + SVerticalBox::Slot()
                                    .HAlign(HAlign_Center)
                                    .VAlign(VAlign_Center)
                                    .FillHeight(6)
                                    [
                                        SAssignNew(NameTextBlock, STextBlock)
                                            .Justification(ETextJustify::Center)
                                            .TextStyle(&CachedNameTextStyle)
                                            .ColorAndOpacity(CachedDefaultColor)
                                            .Text(FText::FromString("Test Name"))
                                    ]
                                    + SVerticalBox::Slot()
                                    .HAlign(HAlign_Center)
                                    .VAlign(VAlign_Center)
                                    .FillHeight(4)
                                    [
                                        SAssignNew(GuideTextBlock, STextBlock)
                                            .Justification(ETextJustify::Center)
                                            .TextStyle(&CachedGuideTextStyle)
                                            .ColorAndOpacity(CachedErrorColor)
                                            .Text(FText::FromString("Test Guide Text"))
                                    ]
                            ]
                    ]
            ]
        ];


        
        
    }

private:
    void CachePropertyValues()
    {
        BoxSizeProperty->GetValue(CachedBoxSize);

        TArray<void*> RawData;
        BoxPaddingProperty->AccessRawData(RawData);
        CachedBoxPadding = *static_cast<FMargin*>(RawData[0]);

        NameTextStyleProperty->AccessRawData(RawData);
        CachedNameTextStyle = *static_cast<FTextBlockStyle*>(RawData[0]);

        GuideTextStyleProperty->AccessRawData(RawData);
        CachedGuideTextStyle = *static_cast<FTextBlockStyle*>(RawData[0]);
        
        DefaultColorProperty->AccessRawData(RawData);
        CachedDefaultColor = *static_cast<FSlateColor*>(RawData[0]);

        ErrorColorProperty->AccessRawData(RawData);
        CachedErrorColor = *static_cast<FSlateColor*>(RawData[0]);
    }

    void OnBoxSizeChanged()
    {
        CachePropertyValues();
        
        SizeBox->SetMinDesiredWidth(CachedBoxSize.X);
        SizeBox->SetMinDesiredHeight(CachedBoxSize.Y);
    }

    void OnBoxPaddingChanged()
    {
        CachePropertyValues();

        Border->SetPadding(CachedBoxPadding);
    }

    void OnNameTextStyleChanged() 
    {
        CachePropertyValues();

        NameTextBlock->SetTextStyle(&CachedNameTextStyle);
    }

    void OnGuideTextStyleChanged()
    {
        CachePropertyValues();

        GuideTextBlock->SetTextStyle(&CachedGuideTextStyle);
    }

    void OnDefaultColorChanged()
    {
        CachePropertyValues();

        NameTextBlock->SetColorAndOpacity(CachedDefaultColor);
    }

    void OnErrorColorChanged()
    {
        CachePropertyValues();

        GuideTextBlock->SetColorAndOpacity(CachedErrorColor);
    }
};

void FCIDSDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    TSharedRef<IPropertyHandle> BoxSizeHandle = DetailBuilder.GetProperty("GuideBoxSize");
    TSharedRef<IPropertyHandle> BoxPaddingHandle = DetailBuilder.GetProperty("GuideBoxPadding");
    TSharedRef<IPropertyHandle> BorderBrushHandle = DetailBuilder.GetProperty("GuideBorderBrush");
    TArray<void*> TempBorderBrush;
    BorderBrushHandle->AccessRawData(TempBorderBrush);
    const FSlateBrush* BorderBrush = static_cast<FSlateBrush*>(TempBorderBrush[0]);
    TSharedRef<IPropertyHandle> NameTextStyleHandle = DetailBuilder.GetProperty("NameTextStyle");
    TSharedRef<IPropertyHandle> GuideTextStyleHandle = DetailBuilder.GetProperty("GuideTextStyle");
    TSharedRef<IPropertyHandle> SuppoertTextStyleHandle = DetailBuilder.GetProperty("SupportTextStyle");
    TSharedRef<IPropertyHandle> DefaultColorHandle = DetailBuilder.GetProperty("DefaultTextColor");
    TSharedRef<IPropertyHandle> ErrorColorHandle = DetailBuilder.GetProperty("ErrorColor");

    
    IDetailCategoryBuilder& SlateCategory = DetailBuilder.EditCategory("Slate", FText::GetEmpty(), ECategoryPriority::Uncommon);

    SlateCategory.AddCustomRow(FText::FromString("Slate"))
        .WholeRowContent()
        [
            SNew(SCIDSMappingPreview)
            .BorderBrush(BorderBrush)
            .BoxSizeProperty(BoxSizeHandle)
            .BoxPaddingProperty(BoxPaddingHandle)
            .NameTextStyleProperty(NameTextStyleHandle)
            .GuideTextStyleProperty(GuideTextStyleHandle)
            .DefaultColorProperty(DefaultColorHandle)
            .ErrorColorProperty(ErrorColorHandle)
        ];
}


TSharedRef<IDetailCustomization> FCIDSDetailCustomization::MakeInstance()
{
	return MakeShareable(new FCIDSDetailCustomization);
}
