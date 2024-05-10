// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SCustomInputSetting.h"
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
#include "PlayerMappableInputConfig.h"
#include "Widgets/Layout/SBackgroundBlur.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widget/SInputKeyLabel.h"

#define LOCTEXT_NAMESPACE "CustomInputSetting"


////////////////////////////////////////////////////// Node ////////////////////////////////////////
SLATE_IMPLEMENT_WIDGET(SCustomInputSettingNode)
void SCustomInputSettingNode::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{

}

SCustomInputSettingNode::SCustomInputSettingNode()
{
	SetCanTick(false);
}



BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCustomInputSettingNode::Construct(const FArguments& InArgs)
{
	ButtonBrush.SetImageSize(InArgs._IconSize);

	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(7)
			.Padding(0,0,10,0)
			[
				SAssignNew(NameBorder, SBorder)
				.BorderImage(InArgs._TextBorderBrush)
				.Padding(InArgs._Padding)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				[
					SAssignNew(NameTextBlock, STextBlock)
					.TextStyle(InArgs._NameBlockStyle)
					.Text(InArgs._NameText)
				]
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(3)
			.Padding(10,0,0,0)
			[
				SAssignNew(IconBorder, SBorder)
				.BorderImage(InArgs._IconBorderBrush)
				.Padding(10)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SAssignNew(ButtonIcon, SImage)
					.Image(&ButtonBrush)
				]
			]
		]

		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(FocusedFrame,SImage)
			.Visibility(EVisibility::Collapsed)
			.Image(InArgs._FocusedFrameBrush)
		]
	];


	TSharedPtr<FNavigationMetaData> NaviMetaData = GetMetaData<FNavigationMetaData>();
	if (!NaviMetaData.IsValid())
	{
		NaviMetaData = MakeShared<FNavigationMetaData>();
		AddMetadata(NaviMetaData.ToSharedRef());
	}

	//임시 Navigation 지정
	for (EUINavigation UINavi : TEnumRange<EUINavigation>())
	{
		NaviMetaData->SetNavigationStop(UINavi);
	}

	SetKeyIcon(InArgs._Key);
	bIsGamepad = InArgs._bIsGamepad;
	Code = InArgs._Code;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SCustomInputSettingNode::SetPadding(FMargin InPadding)
{
	NameBorder->SetPadding(InPadding);
}



void SCustomInputSettingNode::SetNameBlockStyle(const FTextBlockStyle* InStyle)
{
	NameTextBlock->SetTextStyle(InStyle);
}

void SCustomInputSettingNode::SetText(FText InText)
{
	NameTextBlock->SetText(InText);
}


void SCustomInputSettingNode::SetKeyIcon(const FKey& InKey)
{	
	UCustomInputSettingSubsystem* SystemInstance = UCustomInputSettingSubsystem::GetInstance();
#if WITH_EDITOR
	if(SystemInstance == nullptr) return;
#endif

	ButtonBrush.SetResourceObject(SystemInstance->GetKeyIcon(InKey));
}

void SCustomInputSettingNode::SetTextBorderBrush(const FSlateBrush* InBrush)
{
	NameBorder->SetBorderImage(InBrush);
}

void SCustomInputSettingNode::SetIconBorderBrush(const FSlateBrush* InBrush)
{
	IconBorder->SetBorderImage(InBrush);
}

void SCustomInputSettingNode::SetFocusedFrameBrush(const FSlateBrush* InBrush)
{
	FocusedFrame->SetImage(InBrush);
}
void SCustomInputSettingNode::SetInputCode(const FName& InCode)
{
	Code = InCode;
}


TOptional<bool> SCustomInputSettingNode::OnQueryShowFocus(const EFocusCause InFocusCause) const
{
	return false;
}

FReply SCustomInputSettingNode::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	FocusedFrame->SetVisibility(EVisibility::SelfHitTestInvisible);
	return FReply::Handled();
}

void SCustomInputSettingNode::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	FocusedFrame->SetVisibility(EVisibility::Collapsed);
}

FReply SCustomInputSettingNode::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (!IsEnabled() || !bCanSupportFocus || !SupportsKeyboardFocus()) return FReply::Unhandled();

	FSlateApplication& SlateApp = FSlateApplication::Get();

	if (SlateApp.GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept)
	{
		RequestCustomKeySettingByNode();
	}
	else
	{
		EUINavigation Direction = FSlateApplicationBase::Get().GetNavigationDirectionFromKey(InKeyEvent);
		if (Direction != EUINavigation::Invalid && Direction != EUINavigation::Previous && Direction != EUINavigation::Next)
		{
			const ENavigationGenesis Genesis = InKeyEvent.GetKey().IsGamepadKey() ? ENavigationGenesis::Controller : ENavigationGenesis::Keyboard;
			return FReply::Handled().SetNavigation(Direction, Genesis);
		}
	}
	return FReply::Unhandled();
}

FReply SCustomInputSettingNode::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if(!IsEnabled() || MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton) return FReply::Unhandled();

	RequestCustomKeySettingByNode();

	return FReply::Handled();
}

void SCustomInputSettingNode::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if(!IsEnabled()) return;

	FSlateApplication& SlateApp = FSlateApplication::Get();

	TOptional<EFocusCause> FocusData = HasUserFocus(MouseEvent.GetUserIndex());

	if (!FocusData.IsSet())
	{
		SlateApp.SetUserFocus(MouseEvent.GetUserIndex(), SharedThis(this));
	}
}

void SCustomInputSettingNode::OnMouseLeave(const FPointerEvent& MouseEvent)
{
}

void SCustomInputSettingNode::RequestCustomKeySettingByNode()
{
	if (Code == NAME_None) return;

	UCustomInputSettingSubsystem* SettingInstance = UCustomInputSettingSubsystem::GetInstance();

	SettingInstance->RequestMappingKey(Code, bIsGamepad, NameTextBlock->GetText());
}



void SCustomInputSettingNode::SetNavigation(EUINavigation Direction, TSharedPtr<SWidget> InWidget)
{
	if(Direction == EUINavigation::Invalid || Direction == EUINavigation::Num ) return;

	TSharedPtr<FNavigationMetaData> NaviMetaData = GetMetaData<FNavigationMetaData>();

	if (InWidget.IsValid())		{ NaviMetaData->SetNavigationExplicit(Direction, InWidget); }
	else						{ NaviMetaData->SetNavigationStop(Direction); }
}

void SCustomInputSettingNode::SetIconSize(FVector2D InSize)
{
	ButtonBrush.SetImageSize(InSize);
}



/////////////////////////////// Window
SLATE_IMPLEMENT_WIDGET(SCustomInputSettingWindow)
void SCustomInputSettingWindow::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
	
}


SCustomInputSettingWindow::SCustomInputSettingWindow()
{
	TargetInputConfig = nullptr;
	SetCanTick(false);


}

SCustomInputSettingWindow::~SCustomInputSettingWindow()
{
	if(UCustomInputSettingSubsystem* SettingInstance = UCustomInputSettingSubsystem::GetInstance())
	{
		SettingInstance->RemoveOnChangedMappingEvent(OnChangedMappingHandle);
		SettingInstance->RemoveResetMappingEvent(OnResetMappingHandle);
	}
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCustomInputSettingWindow::Construct(const FArguments& InArgs)
{	
	TWeakPtr<SWidget> WeakWindow(AsShared());
	
	UCustomInputSettingSubsystem* SettingInstance = UCustomInputSettingSubsystem::GetInstance();
	
#if WITH_EDITOR
	if (SettingInstance != nullptr)
	{
		OnChangedMappingHandle = SettingInstance->AddOnChangedMappingEvent(TDelegate<void (const FName&, const FKey&)>::CreateSP(this, &SCustomInputSettingWindow::OnChangedMapping));
		OnResetMappingHandle = SettingInstance->AddResetMappingEvent(TDelegate<void (UPlayerMappableInputConfig*)>::CreateSP(this, &SCustomInputSettingWindow::OnResetMapping));
	}
#else
	OnChangedMappingHandle = SettingInstance->AddOnChangedMappingEvent(TDelegate<void(const FName&, const FKey&)>::CreateSP(this, &SCustomInputSettingWindow::OnChangedMapping));
	OnResetMappingHandle = SettingInstance->AddResetMappingEvent(TDelegate<void(UPlayerMappableInputConfig*)>::CreateSP(this, &SCustomInputSettingWindow::OnResetMapping));
#endif

	NodePadding = InArgs._NodePadding;
	DistanceBetweenNode = InArgs._DistanceBetweenNode;

	NodeTextBorderBrush = InArgs._TextBorderBrush;
	NodeIconBorderBrush = InArgs._IconBorderBrush;
	NodeFocusedFrameBrush = InArgs._FocusedFrameBrush;

	SelectedTabBrush = InArgs._SelectedTabBrush;
	UnselectedTabBrush = InArgs._UnselectedTabBrush;

	NodeNameBlockStyle = InArgs._NodeNameBlockStyle;

	
	ChildSlot
	[
		SNew(SVerticalBox)
		.Visibility(EVisibility::Visible)
		+SVerticalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Fill)
		.AutoHeight()
		.Padding(0,0,0,10)
		[
			SAssignNew(TabSizeBox, SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.WidthOverride(InArgs._TabSize.X < 0 ? 0 : 2 * InArgs._TabSize.X)
			.HeightOverride(InArgs._TabSize.Y < 0 ? 0 : InArgs._TabSize.Y)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				.FillWidth(1)
				[
					SAssignNew(KeyboardTab, SBorder)
					.BorderImage(SelectedTabBrush)
					.Padding(0)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
						.OnMouseButtonDown_Lambda([WeakWindow](const FGeometry&, const FPointerEvent&) -> FReply{
									if(!WeakWindow.IsValid() || !WeakWindow.Pin()->IsEnabled()) return FReply::Unhandled();
									SCustomInputSettingWindow* Window = static_cast<SCustomInputSettingWindow*>(WeakWindow.Pin().Get());
									Window->ShowCustomInputDevice(false);

									return FReply::Handled();
								})
					[
						SNew(STextBlock)
						.Visibility(EVisibility::SelfHitTestInvisible)
						.Text(LOCTEXT("Keyboard Name", "키보드/마우스"))
						.TextStyle(InArgs._TabTextStyle)
						.AutoWrapText(false)
					]
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				.Padding(10, 0, 0, 0)
				.FillWidth(1)
				[
					SAssignNew(GamepadTab, SBorder)
					.BorderImage(UnselectedTabBrush)
					.Padding(0)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.OnMouseButtonDown_Lambda([WeakWindow](const FGeometry&, const FPointerEvent&) -> FReply {
							if (!WeakWindow.IsValid() || !WeakWindow.Pin()->IsEnabled()) return FReply::Unhandled();
							SCustomInputSettingWindow* Window = static_cast<SCustomInputSettingWindow*>(WeakWindow.Pin().Get());
							Window->ShowCustomInputDevice(true);

							return FReply::Handled();
						})
					[
						SNew(STextBlock)
						.Visibility(EVisibility::SelfHitTestInvisible)
						.Text(LOCTEXT("Gamepad Name", "게임패드"))
						.TextStyle(InArgs._TabTextStyle)
						.AutoWrapText(false)
					]
				]
			]

			
		]
		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1)
		[
			SAssignNew(InputDeviceSwitcher, SWidgetSwitcher)
			.Visibility(EVisibility::SelfHitTestInvisible)
			+ SWidgetSwitcher::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(KeyboardScrollBox, SScrollBox)
				.NavigationDestination(EDescendantScrollDestination::Center)
			]
			+ SWidgetSwitcher::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(GamepadScrollBox, SScrollBox)
				.NavigationDestination(EDescendantScrollDestination::Center)
			]
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Fill)
		.AutoHeight()
		.Padding(5,0,0,0)
		[
			SAssignNew(DescriptionBox, SHorizontalBox)
			.Visibility(EVisibility::HitTestInvisible)
		]
	];

	KeyboardScrollPanel = (static_cast<SScrollPanel*>(&KeyboardScrollBox->GetChildren()->GetChildAt(0)->GetChildren()->GetChildAt(0)->GetChildren()->GetChildAt(0).Get()));
	GamepadScrollPanel = (static_cast<SScrollPanel*>(&GamepadScrollBox->GetChildren()->GetChildAt(0)->GetChildren()->GetChildAt(0)->GetChildren()->GetChildAt(0).Get()));

	ShowCustomInputDevice(false);

	CreateDescriptionBox();
	SetDescriptionTextStyle(InArgs._DescriptionTextStyle);
	SetDescriptionLabelSize(InArgs._DescriptionLabelSize);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION



void SCustomInputSettingWindow::OnChangedMapping(const FName& InCode, const FKey& NewKey)
{
	KeyboardScrollPanel->GetChildren()->ForEachWidget([InCode, NewKey](SWidget& ChildWidget)
		{
			SCustomInputSettingNode& Node = static_cast<SCustomInputSettingNode&>(ChildWidget);
			if(Node.GetCode() == InCode) Node.SetKeyIcon(NewKey);
		});

	GamepadScrollPanel->GetChildren()->ForEachWidget([InCode, NewKey](SWidget& ChildWidget)
		{
			SCustomInputSettingNode& Node = static_cast<SCustomInputSettingNode&>(ChildWidget);
			if (Node.GetCode() == InCode) Node.SetKeyIcon(NewKey);
		});
}

void SCustomInputSettingWindow::SetMappableKeys(class UPlayerMappableInputConfig* Target)
{
	if(TargetInputConfig == Target) return;

	TargetInputConfig = Target;

	UCustomInputSettingSubsystem* SystemInst = UCustomInputSettingSubsystem::GetInstance();

	//Reset Nodes
	KeyboardScrollBox->ClearChildren();
	GamepadScrollBox->ClearChildren();

	if(TargetInputConfig == nullptr) return;

	TArray<FEnhancedActionKeyMapping> TargetKeyMappings = TargetInputConfig->GetPlayerMappableKeys();
#if WITH_EDITOR
	TMap<FName, FKey> AlreadyCustomMappingKeys;
	if(SystemInst != nullptr) AlreadyCustomMappingKeys = SystemInst->GetCustomMappingKeys();
#else
	TMap<FName, FKey> AlreadyCustomMappingKeys = SystemInst->GetCustomMappingKeys();
#endif
	

	for (int i = 0; i < TargetKeyMappings.Num(); i++)
	{
		bool bIsGamepad = TargetKeyMappings[i].Key.IsGamepadKey();
		FName TempCode = TargetKeyMappings[i].PlayerMappableOptions.Name;
		FKey TempKey = AlreadyCustomMappingKeys.Find(TempCode) != nullptr ? AlreadyCustomMappingKeys[TempCode] : TargetKeyMappings[i].Key;

		TSharedPtr<SCustomInputSettingNode> TempNode;

		SAssignNew(TempNode, SCustomInputSettingNode)
			.Padding(NodePadding)
			.TextBorderBrush(NodeTextBorderBrush)
			.IconBorderBrush(NodeIconBorderBrush)
			.FocusedFrameBrush(NodeFocusedFrameBrush)
			.NameBlockStyle(NodeNameBlockStyle)
			.NameText(TargetKeyMappings[i].PlayerMappableOptions.DisplayName)
			.Code(TempCode)
			.bIsGamepad(false)
			.Key(TempKey)
			.IconSize(IconSize);

		if(bIsGamepad)	
		{ 
			GamepadScrollBox->AddSlot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(0, 0, 0, DistanceBetweenNode)
				[
					TempNode.ToSharedRef()
				];
		}
		else 
		{ 
			KeyboardScrollBox->AddSlot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(0, 0, 0, DistanceBetweenNode)
				[
					TempNode.ToSharedRef()
				];
		}
	}

	RebuildNavigation();
}



void SCustomInputSettingWindow::SetMappableKeys(TSoftObjectPtr<class UPlayerMappableInputConfig> Target)
{
	SetMappableKeys(Target.LoadSynchronous());
}

void SCustomInputSettingWindow::RebuildNavigation()
{
	FChildren* KeyboardNodes = KeyboardScrollPanel->GetChildren();
	int KeyboardNodesNum = KeyboardNodes->Num();
	if(KeyboardNodesNum <= 1) return;

	int LoopMax = KeyboardNodesNum - 1;
	//첫번째 노드 내비
	{
		SCustomInputSettingNode& TempNode = static_cast<SCustomInputSettingNode&>(KeyboardNodes->GetChildAt(0).Get());
		SCustomInputSettingNode& NaviDownNode = static_cast<SCustomInputSettingNode&>(KeyboardNodes->GetChildAt(1).Get());

		TempNode.SetNavigation(EUINavigation::Down, NaviDownNode.AsShared());
	}

	for (int i = 1; i < LoopMax; i++)
	{
		SCustomInputSettingNode& TempNode = static_cast<SCustomInputSettingNode&>( KeyboardNodes->GetChildAt(i).Get());
		SCustomInputSettingNode& NaviUpNode = static_cast<SCustomInputSettingNode&>(KeyboardNodes->GetChildAt(i-1).Get());
		SCustomInputSettingNode& NaviDownNode = static_cast<SCustomInputSettingNode&>(KeyboardNodes->GetChildAt(i+1).Get());

		TempNode.SetNavigation(EUINavigation::Up, NaviUpNode.AsShared());
		TempNode.SetNavigation(EUINavigation::Down, NaviDownNode.AsShared());
	}

	//마지막 노드 내비
	{
		SCustomInputSettingNode& TempNode = static_cast<SCustomInputSettingNode&>(KeyboardNodes->GetChildAt(LoopMax).Get());
		SCustomInputSettingNode& NaviUpNode = static_cast<SCustomInputSettingNode&>(KeyboardNodes->GetChildAt(LoopMax - 1).Get());

		TempNode.SetNavigation(EUINavigation::Up, NaviUpNode.AsShared());
	}
}

void SCustomInputSettingWindow::OnResetMapping(UPlayerMappableInputConfig* InConfig)
{
	UpdateNodes();
}

void SCustomInputSettingWindow::UpdateNodes()
{
	if (TargetInputConfig == nullptr) return;

	UCustomInputSettingSubsystem* SystemInst = UCustomInputSettingSubsystem::GetInstance();

	TArray<FEnhancedActionKeyMapping> TargetKeyMappings = TargetInputConfig->GetPlayerMappableKeys();
	TMap<FName, FKey> TempKeyboard;
	TMap<FName, FKey> TempGamepad;
	
	for (const auto& TempMapping : TargetKeyMappings)
	{
		FName TempCode = TempMapping.PlayerMappableOptions.Name;

		if (TempMapping.Key.IsGamepadKey())
		{
			TempGamepad.Add(TempCode, TempMapping.Key);
		}
		else
		{
			TempKeyboard.Add(TempCode, TempMapping.Key);
		}
	}

#if WITH_EDITOR
	TMap<FName, FKey> AlreadyCustomMappingKeys;
	if (SystemInst != nullptr) AlreadyCustomMappingKeys = SystemInst->GetCustomMappingKeys();
#else
	TMap<FName, FKey> AlreadyCustomMappingKeys = SystemInst->GetCustomMappingKeys();
#endif

	for (const auto& MappedCode : AlreadyCustomMappingKeys)
	{
		if (MappedCode.Value.IsGamepadKey())
		{
			FKey* Temp = TempGamepad.Find(MappedCode.Key);
			if(Temp != nullptr) { *Temp = MappedCode.Value; }
		}
		else
		{
			FKey* Temp = TempKeyboard.Find(MappedCode.Key);
			if (Temp != nullptr) { *Temp = MappedCode.Value; }
		}
	}
	
	KeyboardScrollPanel->GetChildren()->ForEachWidget([&TempKeyboard](SWidget& ChildWidget)
		{
			SCustomInputSettingNode& Node = static_cast<SCustomInputSettingNode&>(ChildWidget);
			Node.SetKeyIcon(TempKeyboard[Node.GetCode()]);
		});

	GamepadScrollPanel->GetChildren()->ForEachWidget([&TempGamepad](SWidget& ChildWidget)
		{
			SCustomInputSettingNode& Node = static_cast<SCustomInputSettingNode&>(ChildWidget);
			Node.SetKeyIcon(TempGamepad[Node.GetCode()]);
		});
}

void SCustomInputSettingWindow::SetNodePadding(const FMargin& InNodePadding)
{
	if(NodePadding == InNodePadding) return;

	NodePadding = InNodePadding;

	KeyboardScrollPanel->GetChildren()->ForEachWidget([InNodePadding](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetPadding(InNodePadding);
		});

	GamepadScrollPanel->GetChildren()->ForEachWidget([InNodePadding](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetPadding(InNodePadding);
		});
}

void SCustomInputSettingWindow::SetNodeTextBorderBrush(const FSlateBrush* InBrush)
{
	if(NodeTextBorderBrush == InBrush) return;

	NodeTextBorderBrush = InBrush;

	KeyboardScrollPanel->GetChildren()->ForEachWidget([InBrush](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetTextBorderBrush(InBrush);
		});

	GamepadScrollPanel->GetChildren()->ForEachWidget([InBrush](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetTextBorderBrush(InBrush);
		});
}

void SCustomInputSettingWindow::SetNodeIconBorderBrush(const FSlateBrush* InBrush)
{
	if (NodeIconBorderBrush == InBrush) return;

	NodeIconBorderBrush = InBrush;

	KeyboardScrollPanel->GetChildren()->ForEachWidget([InBrush](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetIconBorderBrush(InBrush);
		});

	GamepadScrollPanel->GetChildren()->ForEachWidget([InBrush](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetIconBorderBrush(InBrush);
		});
}

void SCustomInputSettingWindow::SetNodeFocusedFrameBrush(const FSlateBrush* InBrush)
{
	if (NodeFocusedFrameBrush == InBrush) return;

	NodeFocusedFrameBrush = InBrush;

	KeyboardScrollPanel->GetChildren()->ForEachWidget([InBrush](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetFocusedFrameBrush(InBrush);
		});

	GamepadScrollPanel->GetChildren()->ForEachWidget([InBrush](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetFocusedFrameBrush(InBrush);
		});
}

void SCustomInputSettingWindow::SetTabSize(FVector2D InSize)
{
	TabSizeBox->SetWidthOverride(InSize.X < 0 ? 0 : 2 * InSize.X);
	TabSizeBox->SetHeightOverride(InSize.Y < 0 ? 0 : InSize.Y);
}

void SCustomInputSettingWindow::SetTabTextStyle(const FTextBlockStyle* InStyle)
{
	static_cast<STextBlock&>(KeyboardTab->GetContent().Get()).SetTextStyle(InStyle);
	static_cast<STextBlock&>(GamepadTab->GetContent().Get()).SetTextStyle(InStyle);
}

void SCustomInputSettingWindow::SetSelectedTabBrush(const FSlateBrush* InBrush)
{
	if(SelectedTabBrush == InBrush) return;

	SelectedTabBrush = InBrush;

	if (InputDeviceSwitcher->GetActiveWidget().Get() == KeyboardScrollBox.Get()) 
	{
		KeyboardTab->SetBorderImage(SelectedTabBrush);
	}
	else
	{
		GamepadTab->SetBorderImage(SelectedTabBrush);
	}
}

void SCustomInputSettingWindow::SetUnselectedTabBrush(const FSlateBrush* InBrush)
{
	if (UnselectedTabBrush == InBrush) return;

	UnselectedTabBrush = InBrush;

	if (InputDeviceSwitcher->GetActiveWidget().Get() == KeyboardScrollBox.Get())
	{
		GamepadTab->SetBorderImage(UnselectedTabBrush);
	}
	else
	{
		KeyboardTab->SetBorderImage(UnselectedTabBrush);
	}
}


void SCustomInputSettingWindow::SetNodeNameBlockStyle(const FTextBlockStyle* InStyle)
{
	NodeNameBlockStyle = InStyle;

	KeyboardScrollPanel->GetChildren()->ForEachWidget([InStyle](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetNameBlockStyle(InStyle);
		});

	GamepadScrollPanel->GetChildren()->ForEachWidget([InStyle](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetNameBlockStyle(InStyle);
		});

	static_cast<STextBlock&>(KeyboardTab->GetContent().Get()).SetTextStyle(InStyle);
	static_cast<STextBlock&>(GamepadTab->GetContent().Get()).SetTextStyle(InStyle);
}

void SCustomInputSettingWindow::SetIconSize(FVector2D InSize)
{
	if(IconSize == InSize) return;

	IconSize = InSize;

	KeyboardScrollPanel->GetChildren()->ForEachWidget([InSize](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetIconSize(InSize);
		});

	GamepadScrollPanel->GetChildren()->ForEachWidget([InSize](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetIconSize(InSize);
		});
}

void SCustomInputSettingWindow::CreateDescriptionBox()
{
	UCustomInputSettingSubsystem* CSS = UCustomInputSettingSubsystem::GetInstance();

	TSharedPtr<STextBlock> TempTextBlock;
	TSharedPtr<SInputKeyLabel> TempLabel;


	DescriptionBox->AddSlot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Center)
	[
		SAssignNew(TempTextBlock, STextBlock)
		.Text(LOCTEXT("Up/Down", "위/아래 : "))
	];

	DescriptionTexts.Add(TempTextBlock);

	if (CSS != nullptr)
	{
		{
			FKey KeyboardUpKey;
			FKey GamepadUpKey;
			CSS->GetNavigationDirectionKey(EUINavigation::Up, KeyboardUpKey, GamepadUpKey);

			DescriptionBox->AddSlot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[
				SAssignNew(TempLabel, SInputKeyLabel)
				.KeyboardKey(KeyboardUpKey)
				.GamepadKey(GamepadUpKey)
			];

			DescriptionLabels.Add(TempLabel);
		}
		
		{
			FKey KeyboardDownKey;
			FKey GamepadDownKey;
			CSS->GetNavigationDirectionKey(EUINavigation::Down, KeyboardDownKey, GamepadDownKey);

			DescriptionBox->AddSlot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[
				SAssignNew(TempLabel, SInputKeyLabel)
				.KeyboardKey(KeyboardDownKey)
				.GamepadKey(GamepadDownKey)
			];
			DescriptionLabels.Add(TempLabel);
		}
	}

	DescriptionBox->AddSlot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Center)
	[
		SAssignNew(TempTextBlock, STextBlock)
		.Text(LOCTEXT("Input Device Switch", ", 장치전환 : "))
	];

	DescriptionTexts.Add(TempTextBlock);

	if (CSS != nullptr)
	{
		{
			FKey KeyboardPrevKey;
			FKey GamepadPrevKey;
			CSS->GetNavigationDirectionKey(EUINavigation::Previous, KeyboardPrevKey, GamepadPrevKey);

			DescriptionBox->AddSlot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					SAssignNew(TempLabel, SInputKeyLabel)
						.KeyboardKey(KeyboardPrevKey)
						.GamepadKey(GamepadPrevKey)
				];
			DescriptionLabels.Add(TempLabel);
		}

		{
			FKey KeyboardNextKey;
			FKey GamepadNextKey;
			CSS->GetNavigationDirectionKey(EUINavigation::Next, KeyboardNextKey, GamepadNextKey);

			DescriptionBox->AddSlot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					SAssignNew(TempLabel, SInputKeyLabel)
						.KeyboardKey(KeyboardNextKey)
						.GamepadKey(GamepadNextKey)
				];
			DescriptionLabels.Add(TempLabel);
		}
	}

	DescriptionBox->AddSlot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Center)
	[
		SAssignNew(TempTextBlock, STextBlock)
		.Text(LOCTEXT("Accept", ", 설정 : "))
	];

	DescriptionTexts.Add(TempTextBlock);

	if (CSS != nullptr)
	{
		FKey KeyboardAcceptKey;
		FKey GamepadAcceptKey;
		CSS->GetNavigationActionKey(EUINavigationAction::Accept, KeyboardAcceptKey, GamepadAcceptKey);

		DescriptionBox->AddSlot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SAssignNew(TempLabel, SInputKeyLabel)
			.KeyboardKey(KeyboardAcceptKey)
			.GamepadKey(GamepadAcceptKey)
		];
		DescriptionLabels.Add(TempLabel);
	}
}

void SCustomInputSettingWindow::SetDescriptionTextStyle(const FTextBlockStyle* InStyle)
{
	for (auto& DescriptionText : DescriptionTexts)
	{
		DescriptionText->SetTextStyle(InStyle);
	}
}

void SCustomInputSettingWindow::SetDescriptionLabelSize(FVector2D InSize)
{
	for (auto& Label : DescriptionLabels)
	{
		Label->SetIconSize(InSize);
	}
}

void SCustomInputSettingWindow::SetDistanceBetweenNode(float InDistance)
{
	if(DistanceBetweenNode == InDistance) return;
	
	DistanceBetweenNode = InDistance;

	int KeyboardNum = KeyboardScrollPanel->GetChildren()->Num();
	for (int i = 0; i < KeyboardNum; i++)
	{
		static_cast<SScrollBox::FSlot&>(
			const_cast<FSlotBase&>(
				KeyboardScrollPanel->GetChildren()->GetSlotAt(i)))
					.SetPadding(FMargin(0,0,0, DistanceBetweenNode));
	}
	int GamepadNum = GamepadScrollPanel->GetChildren()->Num();
	for (int i = 0; i < GamepadNum; i++)
	{
		static_cast<SScrollBox::FSlot&>(
			const_cast<FSlotBase&>(
				GamepadScrollPanel->GetChildren()->GetSlotAt(i)))
					.SetPadding(FMargin(0, 0, 0, DistanceBetweenNode));
	}
}


FReply SCustomInputSettingWindow::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FSlateApplication& SlateApp = FSlateApplication::Get();

	if (SlateApp.GetNavigationDirectionFromKey(InKeyEvent) == EUINavigation::Previous || SlateApp.GetNavigationDirectionFromKey(InKeyEvent) == EUINavigation::Next)
	{
		ShowCustomInputDevice(InputDeviceSwitcher->GetActiveWidget() == KeyboardScrollBox.ToSharedRef());
		EnterCustomInputSetting(InKeyEvent.GetUserIndex());

		return FReply::Handled();
	}

	FKey ResetKeyboard;
	FKey ResetGamepad;

	UCustomInputSettingSubsystem* CustomInstance = UCustomInputSettingSubsystem::GetInstance();
	CustomInstance->GetNavigationOptionalActionKey(FName("Reset"), ResetKeyboard, ResetGamepad);

	if (InKeyEvent.GetKey() == ResetKeyboard || InKeyEvent.GetKey() == ResetGamepad)
	{
		CustomInstance->RequestResetMapping(TargetInputConfig);

		return FReply::Handled();
	}
	

	return FReply::Unhandled();
}

FReply SCustomInputSettingWindow::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	EnterCustomInputSetting(InFocusEvent.GetUser());

	KeyboardScrollPanel->GetChildren()->ForEachWidget([](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetEnabled(true);
		});

	GamepadScrollPanel->GetChildren()->ForEachWidget([](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetEnabled(true);
		});

	return FReply::Handled();
}

void SCustomInputSettingWindow::ShowCustomInputDevice(bool bIsGamepad)
{
	if (bIsGamepad)
	{
		InputDeviceSwitcher->SetActiveWidget(GamepadScrollBox.ToSharedRef());
		GamepadTab->SetBorderImage(SelectedTabBrush);
		KeyboardTab->SetBorderImage(UnselectedTabBrush);
	}
	else
	{
		InputDeviceSwitcher->SetActiveWidget(KeyboardScrollBox.ToSharedRef());
		GamepadTab->SetBorderImage(UnselectedTabBrush);
		KeyboardTab->SetBorderImage(SelectedTabBrush);
	}
}

void SCustomInputSettingWindow::ExitCustomInputSetting()
{
	KeyboardTab->SetEnabled(false);
	GamepadTab->SetEnabled(false);

	KeyboardScrollPanel->GetChildren()->ForEachWidget([](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetEnabled(false);
		});

	GamepadScrollPanel->GetChildren()->ForEachWidget([](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetEnabled(false);
		});
}


void SCustomInputSettingWindow::EnterCustomInputSetting(uint32 UserIndex)
{
	KeyboardTab->SetEnabled(true);
	GamepadTab->SetEnabled(true);

	FSlateApplication& SlateApp = FSlateApplication::Get();

	if (InputDeviceSwitcher->GetActiveWidget() == KeyboardScrollBox)
	{
		if(KeyboardScrollPanel->GetChildren()->Num() == 0) return;
		SlateApp.SetUserFocus(UserIndex, KeyboardScrollPanel->GetChildren()->GetChildAt(0));
	}
	else
	{
		if (GamepadScrollPanel->GetChildren()->Num() == 0) return;
		SlateApp.SetUserFocus(UserIndex, GamepadScrollPanel->GetChildren()->GetChildAt(0));
	}
}




#undef LOCTEXT_NAMESPACE

