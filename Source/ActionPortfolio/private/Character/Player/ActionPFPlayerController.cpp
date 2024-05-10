// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/ActionPFPlayerController.h"
#include "ActionPortfolio.h"
#include "ActionPortfolioInstance.h"

#include "Interaction/InteractionType/InteractionSystemComponent_NPC.h"
#include "Interaction/InteractionType/SNPCInteractWidget.h"

#include "Engine/GameViewportClient.h"
#include "Engine/DataTable.h"
#include "Character/Player/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Player/Widget_PlayerMainUI.h"
#include "Character/Player/PlayerDialogueMCComponent.h"

#include "Blueprint/UserWidget.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Character/Player/InventoryComponent.h"
#include "Items/ItemBase.h"
#include "Items/ItemManagerSubsystem.h"
#include "Character/CharacterStatusComponent.h"

#include "CustomInputHelper.h"
#include "InputMappingContext.h"
#include "CustomInputSettingSubsystem.h"

#include "Ability/Widget/SAbilitySlot.h"

#include "LockOn/LockOnTargetComponent.h"
#include "LockOn/LockOnSubsystem.h"
#include "GameFramework/PlayerState.h"

#include "Items/Widget/SInventorySlate.h"
#include "Items/Slot/ItemSlot.h"

#include "Character/Player/Widget/UserWidget_PlayerInventory.h"
#include "Items/Widget/InventoryWidget.h"

AActionPFPlayerController::AActionPFPlayerController()
{
	PlayerDialogueMC = CreateDefaultSubobject<UPlayerDialogueMCComponent>("PlayerDialogueMC");

	Inventory = CreateDefaultSubobject<UInventoryComponent>("PlayerInventory");

#if WITH_EDITOR
	bDrawTraceInteractionLine = true;
#endif
}

void AActionPFPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Interaction Tick
	TraceInteractions();

	Tick_LockOn(DeltaSeconds);
}

void AActionPFPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(aPawn);
	if (!IsValid(PlayerChar)) return;

	PlayerChar->Tags.AddUnique(FName("PlayerCharacter"));
}

void AActionPFPlayerController::OnUnPossess()
{
	PlayerMainUI = nullptr;

	EmptyInteractions();

	APawn* PossessedPawn = GetPawn();
	if(IsValid(PossessedPawn)){
		PossessedPawn->Tags.Remove(FName("PlayerCharacter"));
	}
	Super::OnUnPossess();
}

void AActionPFPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		
		EnhancedInput->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &AActionPFPlayerController::OpenMenu);
		
		EnhancedInput->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &AActionPFPlayerController::OpenInventory);
		
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AActionPFPlayerController::InteractFocusedInteraction);

		EnhancedInput->BindAction(LockOnAction, ETriggerEvent::Triggered, this, &AActionPFPlayerController::PressLockOnAction);

		UCustomInputSettingSubsystem::GetInstance()->RegisterAffectedController(this);

		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddPlayerMappableConfig(ControllerInput);
	}
}



void AActionPFPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

void AActionPFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetGenericTeamId(1);

	//겹치게 될 Interaction들의 예상 최대개수만큼 미리 메모리 잡기 : 한 Actor에 여러개의 상호작용이 있을 수도 있으니
	PrevTracedInteractionActors.Reserve(4);
	TracedInteractionActors.Reserve(4);
	FocusedInteractions.Reserve(2);

	ChangeGameInputMode();

	MenuWidget = CreateWidget(this, MenuWidgetClass, "PlayerMenu");
	MenuWidget->AddToViewport(10);
	MenuWidget->SetVisibility(ESlateVisibility::Collapsed);

	CreateInventorySlate();

	Inventory->AddItemByCode("Test", 1);
}





void AActionPFPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (TeamID != NewTeamID) {
		TeamID = NewTeamID;
	}
}



void AActionPFPlayerController::AddCustomFocuseWidget(SWidget& InWidget, bool bSaveToStack)
{
	TSharedRef<SWidget> Shared = InWidget.AsShared();

	AddCustomFocuseWidget(Shared, bSaveToStack);
}

void AActionPFPlayerController::AddCustomFocuseWidget(TSharedRef<SWidget> InWidget, bool bSaveToStack)
{
	FInputModeUIOnly UIInputMode;
	UIInputMode.SetWidgetToFocus(InWidget);

	bShowMouseCursor = true;
	SetInputMode(UIInputMode);

	if (bSaveToStack)
	{
		FocusedWidgetStack.Add(MoveTemp(InWidget));
	}
}

void AActionPFPlayerController::RemoveCustomFocuseWidgetStack()
{
	if(FocusedWidgetStack.IsEmpty()) return;

	FocusedWidgetStack.Pop();

	if (FocusedWidgetStack.IsEmpty())
	{
		ChangeGameInputMode();
	}
	else
	{
		FInputModeUIOnly UIInputMode;
		UIInputMode.SetWidgetToFocus(FocusedWidgetStack.Last());

		SetInputMode(UIInputMode);
	}
}

void AActionPFPlayerController::HideMainUI()
{
	MainUIHideCount++;
	if (0 < MainUIHideCount)
	{
		PlayerMainUI->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AActionPFPlayerController::DisplayMainUI()
{
	MainUIHideCount--;
	if (MainUIHideCount <= 0)
	{
		PlayerMainUI->SetVisibility(ESlateVisibility::Visible);
	}
}

void AActionPFPlayerController::SetPlayerMainUI(UWidget_PlayerMainUI* InMainUI)
{
	PlayerMainUI = InMainUI;

	if (PlayerMainUI)
	{
		if (0 < MainUIHideCount)
		{
			PlayerMainUI->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			PlayerMainUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}



void AActionPFPlayerController::OpenMenu()
{
	ChangeUIInputMode();
	MenuWidget->SetVisibility(ESlateVisibility::Visible);

	HideMainUI();
}




void AActionPFPlayerController::ChangeUIInputMode()
{
	InputComponent->bBlockInput = true;

	SetShowMouseCursor(true);
	
	SetInputMode( FInputModeGameAndUI());
}

void AActionPFPlayerController::ChangeGameInputMode()
{
	InputComponent->bBlockInput = false;

	SetShowMouseCursor(false);

	SetInputMode(FInputModeGameOnly());
}


void AActionPFPlayerController::OnChangedFocusedInteractionActor()
{
	if (TracedInteractionActors.IsEmpty())
	{
		EmptyInteractions();
		return;
	}

	if(CurrentFocusedActor.Get() == TracedInteractionActors[0]) return;

	EmptyInteractions();

	CurrentFocusedActor = TracedInteractionActors[0];
	TArray<UInteractionSystemComponent*> TempInteractions;

	CurrentFocusedActor->GetComponents<UInteractionSystemComponent>(TempInteractions);
	for (auto& TempInteraction : TempInteractions)
	{
		if (TempInteraction->IsEnableInteract())
		{
			FocusedInteractions.Add(TempInteraction);
		}
	}

	FocusedInteractions[FocusedInteractionIDX]->OnFocusedOnByPlayer();
}




void AActionPFPlayerController::TraceInteractions()
{
	APawn* PlayerPawn = GetPawn();
	if (!IsValid(PlayerPawn)){return;}

	PrevTracedInteractionActors = TracedInteractionActors;
	TracedInteractionActors.Empty();

	//Trace를 위한 매개변수들
	static const float TraceDistance = 300;
	TArray<FHitResult> HitResults;
	FVector TraceStart = PlayerPawn->GetActorLocation();
	FVector TraceEnd = TraceStart + TraceDistance * PlayerPawn->GetActorForwardVector();
	FCollisionQueryParams CQP(FName("Player Trace Interactions"), false, PlayerPawn);

	//Trace
	//ECC_GameTraceChannel1 == InteractorSensor
	GetWorld()->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, ECC_GameTraceChannel1, CQP);

	TArray<AActor*> TracedActors;
	
	//Trace에 걸린 Actor들이 있을 시에 상호작용컴포넌트가 있는지 체크
	for (const auto& HitResult : HitResults)
	{
		AActor* TracedActor = HitResult.GetActor();
		if(!IsValid(TracedActor)) continue;

		TArray<UInteractionSystemComponent*> TempComponents;
		TracedActor->GetComponents<UInteractionSystemComponent>(TempComponents);
		
		bool bCanInteract = false;

		for (auto& InteractComponent : TempComponents)
		{
			bCanInteract = bCanInteract || InteractComponent->IsEnableInteract();
		}
		
		if(bCanInteract) TracedActors.Add(TracedActor);
	}
	
#if WITH_EDITOR
	if (bDrawTraceInteractionLine)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, TracedActors.IsEmpty() ? FColor::Red : FColor::Green);
	}
#endif

	//비어있는지 체크
	if (TracedActors.IsEmpty())
	{
		OnChangedFocusedInteractionActor();
		return;
	}

	//이미 있던 액터들 추가
	for (auto& PrevTracedActor : PrevTracedInteractionActors)
	{
		if (TracedActors.Contains(PrevTracedActor))
		{
			TracedInteractionActors.Add(PrevTracedActor);
		}
	}
	
	//새로운 액터들 추가
	for (auto& TracedActor : TracedActors)
	{
		if (!TracedInteractionActors.Contains(TracedActor))
		{
			TracedInteractionActors.Add(TracedActor);
		}
	}

	OnChangedFocusedInteractionActor();
}

void AActionPFPlayerController::EmptyInteractions()
{
	if(!CurrentFocusedActor.IsValid()) return;

	FocusedInteractions[FocusedInteractionIDX]->OnFocusedOffByPlayer();
	FocusedInteractions.Empty();
	CurrentFocusedActor.Reset();
	FocusedInteractionIDX = 0;
}

void AActionPFPlayerController::FocusNextInteraction()
{
	if(!CurrentFocusedActor.IsValid() || (FocusedInteractions.Num() == 1 && TracedInteractionActors.Num() == 1)) return;

	FocusedInteractions[FocusedInteractionIDX]->OnFocusedOffByPlayer();
	if (FocusedInteractions.IsValidIndex(++FocusedInteractionIDX))
	{
		FocusedInteractions[FocusedInteractionIDX]->OnFocusedOnByPlayer();
	}
	else
	{
		TracedInteractionActors.Add(CurrentFocusedActor.Get());
		TracedInteractionActors.RemoveAt(0);
		OnChangedFocusedInteractionActor();
	}
}



void AActionPFPlayerController::InteractFocusedInteraction()
{
#if WITH_EDITOR
	PFLOG(Warning, TEXT("Try Interact FocusedInteraction"));
#endif
	if(!FocusedInteractions.IsValidIndex(FocusedInteractionIDX)) return;
#if WITH_EDITOR
	PFLOG(Warning, TEXT("Call Interact FocusedInteraction"));
#endif

	FocusedInteractions[FocusedInteractionIDX]->Interact(GetPawn());
}



void AActionPFPlayerController::CreateInventorySlate()
{
	InventoryWidget = CreateWidget<UUserWidget_PlayerInventory>(this, InventoryWidgetClass, "PlayerInventory");

}

void AActionPFPlayerController::OpenInventory()
{
	InventoryWidget->AddToViewport(10);
	InventoryWidget->GetInventoryWidget()->SetUserFocus(this);
	
	ChangeUIInputMode();
}

void AActionPFPlayerController::CloseInventory()
{
	InventoryWidget->RemoveFromParent();

	ChangeGameInputMode();
}

void AActionPFPlayerController::PickUpItem(ADroppedItem* Target)
{
	if (!IsValid(Target))
	{
		PFLOG(Error, TEXT("Try Pick Up Invalidate DroppedItem."));
		return;
	}

	Inventory->AddItemByDropItem(*Target);
}

void AActionPFPlayerController::Tick_LockOn(float DeltaTime)
{
	if (!LockOnTarget.IsValid()) return;

	FVector DeltaVector = LockOnTarget->GetComponentLocation() - PlayerCameraManager->GetCameraLocation();
	FRotator NewRot = DeltaVector.Rotation();
	ClientSetRotation(NewRot, false);
}

void AActionPFPlayerController::AddPitchInput(float Val)
{
	if(LockOnTarget.IsValid()) return;

	Super::AddPitchInput(Val);
}

void AActionPFPlayerController::AddYawInput(float Val)
{
	if (LockOnTarget.IsValid()) return;

	Super::AddYawInput(Val);
}

bool AActionPFPlayerController::CanLockOnTarget(ULockOnTargetComponent* InTarget) const
{
	if(InTarget == nullptr) return false;

	AActor* TargetActor = InTarget->GetOwner();

	if(GetTeamAttitudeTowards(*TargetActor) == ETeamAttitude::Friendly) return false;

	return true;
}

void AActionPFPlayerController::SearchLockOnTarget()
{
	ULockOnSubsystem* LockOnSS = GetWorld()->GetSubsystem<ULockOnSubsystem>();
	check(LockOnSS);

	TArray<ULockOnTargetComponent*> AbleTargets;
	float Dist = LockOnSearchDistance * LockOnSearchDistance;

#if WITH_EDITOR
	FVector ActorCenterPos = GetPawn()->GetActorLocation();
	DrawDebugSphere(GetWorld(), ActorCenterPos, LockOnSearchDistance, 32, FColor::Green, false, 2);
#endif

	if (LockOnSS->GetLockOnAbleTargets(AbleTargets, TDelegate<bool(ULockOnTargetComponent*)>::CreateUObject(this, &AActionPFPlayerController::CanLockOnTarget)))
	{
		FVector CenterPos = GetPawn()->GetActorLocation();

		FVector CameraForward = PlayerCameraManager->GetCameraRotation().Vector();
		
		for (int i = 0; i < AbleTargets.Num(); i++)
		{
			if(Dist < FVector::DistSquared(CenterPos, AbleTargets[i]->GetComponentLocation())) continue;

			FVector DistVector = AbleTargets[i]->GetComponentLocation() - CenterPos;

			float Cos = FVector::DotProduct(CameraForward, DistVector);
			
			#if WITH_EDITOR
			float Test = 0.8125 * DistVector.Length();
			PFLOG(Warning, TEXT("%.1f, Cos = %.1f"), Test, Cos);
			#endif 

			if (0.8125 * DistVector.Length() <= Cos)
			{
				LockOnTarget = AbleTargets[i];
			}
		}
	}
}

void AActionPFPlayerController::ClearLockOnTarget()
{
	LockOnTarget.Reset();
}

UAbilitySystemComponent* AActionPFPlayerController::GetAbilitySystemComponent() const
{
	if(IAbilitySystemInterface* PawnASI = Cast<IAbilitySystemInterface>(GetPawn()))
	{
		return PawnASI->GetAbilitySystemComponent();
	}

	return nullptr;
}

UCharacterStatusComponent* AActionPFPlayerController::GetStatusComponent() const
{
	if (APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		return PlayerCharacter->GetCharacterStatusComponent();
	}

	return nullptr;
}

void AActionPFPlayerController::PressLockOnAction()
{
	if (LockOnTarget.IsValid())
	{
		ClearLockOnTarget();
	}
	else
	{
		SearchLockOnTarget();
	}
}

