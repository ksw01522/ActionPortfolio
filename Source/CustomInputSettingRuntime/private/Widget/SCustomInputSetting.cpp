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

	//임시
	NaviMetaData->SetNavigationStop(EUINavigation::Previous);
	NaviMetaData->SetNavigationStop(EUINavigation::Next);

	NaviMetaData->SetNavigationStop(EUINavigation::Left);
	NaviMetaData->SetNavigationStop(EUINavigation::Right);
	NaviMetaData->SetNavigationStop(EUINavigation::Up);
	NaviMetaData->SetNavigationStop(EUINavigation::Down);

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
	if (bCanSupportFocus && SupportsKeyboardFocus())
	{
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

	NodeNameBlockStyle = InArgs._NodeNameBlockStyle;

	ChildSlot
	[
		SAssignNew(InputDeviceSwitcher, SWidgetSwitcher)
		.Visibility(EVisibility::SelfHitTestInvisible)
		+SWidgetSwitcher::Slot()
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
	];

	KeyboardScrollPanel = (static_cast<SScrollPanel*>(&KeyboardScrollBox->GetChildren()->GetChildAt(0)->GetChildren()->GetChildAt(0)->GetChildren()->GetChildAt(0).Get()));
	GamepadScrollPanel = (static_cast<SScrollPanel*>(&GamepadScrollBox->GetChildren()->GetChildAt(0)->GetChildren()->GetChildAt(0)->GetChildren()->GetChildAt(0).Get()));

	ShowCustomInputDevice(false);
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
			.Key(TempKey);

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


void SCustomInputSettingWindow::SetNodeNameBlockStyle(const FTextBlockStyle* InStyle)
{
	if(NodeNameBlockStyle == InStyle)

	NodeNameBlockStyle = InStyle;

	KeyboardScrollPanel->GetChildren()->ForEachWidget([InStyle](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetNameBlockStyle(InStyle);
		});

	GamepadScrollPanel->GetChildren()->ForEachWidget([InStyle](SWidget& ChildWidget)
		{
			static_cast<SCustomInputSettingNode&>(ChildWidget).SetNameBlockStyle(InStyle);
		});
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

	return FReply::Handled();
}

void SCustomInputSettingWindow::ShowCustomInputDevice(bool bIsGamepad)
{
	if (bIsGamepad)
	{
		InputDeviceSwitcher->SetActiveWidget(GamepadScrollBox.ToSharedRef());
	}
	else
	{
		InputDeviceSwitcher->SetActiveWidget(KeyboardScrollBox.ToSharedRef());
	}
}


void SCustomInputSettingWindow::EnterCustomInputSetting(uint32 UserIndex)
{
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

