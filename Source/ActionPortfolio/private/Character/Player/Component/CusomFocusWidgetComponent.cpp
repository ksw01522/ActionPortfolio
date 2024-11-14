// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Component/CusomFocusWidgetComponent.h"
#include "Components/Widget.h"
#include "Character/Player/ActionPFPlayerController.h"

// Sets default values for this component's properties
UCusomFocusWidgetComponent::UCusomFocusWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCusomFocusWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	//
	if (AActionPFPlayerController* OwnerController = GetOwner<AActionPFPlayerController>())
	{
		ULocalPlayer* LocalPlayer = OwnerController->GetLocalPlayer();
		if (LocalPlayer == nullptr)
		{
			DestroyComponent();
		}
	}
}


// Called every frame
void UCusomFocusWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UCusomFocusWidgetComponent::AddCustomFocuseWidget(UWidget* InWidget, bool bSaveToStack)
{
	if (!IsValid(InWidget)) return;

	AActionPFPlayerController* PlayerController = GetOwner<AActionPFPlayerController>();

	PlayerController->ChangeUIInputMode();

	if (bSaveToStack)
	{
		FocusedWidgetStack.Add(InWidget);
	}

	InWidget->SetUserFocus(PlayerController);
}



void UCusomFocusWidgetComponent::RemoveCustomFocuseWidgetStack(UWidget* TargetWidget)
{
	if (FocusedWidgetStack.IsEmpty()) return;

	for (auto& CheckWidget : FocusedWidgetStack)
	{
		if (!CheckWidget.IsValid())
		{
			FocusedWidgetStack.Remove(CheckWidget);
		}
	}

	AActionPFPlayerController* PlayerController = GetOwner<AActionPFPlayerController>();

	if (FocusedWidgetStack.IsEmpty())
	{
		PlayerController->ChangeGameInputMode();
		return;
	}

	TWeakObjectPtr<UWidget> TempWidget = FocusedWidgetStack.Last();

	if (TempWidget == TargetWidget)
	{
		FocusedWidgetStack.Pop();

		if (FocusedWidgetStack.IsEmpty())
		{
			PlayerController->ChangeGameInputMode();
		}
		else
		{
			FocusedWidgetStack.Last()->SetUserFocus(PlayerController);
		}

		return;
	}

	FocusedWidgetStack.Remove(TargetWidget);
}