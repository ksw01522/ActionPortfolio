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

#include "UI/SActionPFMainSlate.h"
#include "UI/DialogueSlate.h"

#include "Blueprint/UserWidget.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Character/Player/InventoryComponent.h"
#include "Items/ItemBase.h"
#include "Items/ItemManagerSubsystem.h"
#include "Character/CharacterStatusComponent.h"

#include "CustomInputHelper.h"
#include "InputMappingContext.h"
#include "CustomInputSettingSubsystem.h"
#include "Widget/CustomInputSettingWidget.h"

AActionPFPlayerController::AActionPFPlayerController()
{
	PlayerDialogueMC = CreateDefaultSubobject<UPlayerDialogueMCComponent>("PlayerDialogueMC");

	Inventory = CreateDefaultSubobject<UInventoryComponent>("PlayerInventory");

	bRegisteredInteractMapping = false;
	bCanInteract = true;
	interactMappingPriority = 1;

#if WITH_EDITOR
	bDrawTraceInteractionLine = true;
#endif

	
}

void AActionPFPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Interaction Tick
	TraceInteractions();
}

void AActionPFPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(aPawn);
	if (!IsValid(PlayerChar)) return;

	PlayerChar->Tags.AddUnique(FName("PlayerCharacter"));

	if (IsValid(PlayerMainUI)) {
		PlayerMainUI->LinkASC();
	}
	else
	{
		CreatePlayerMainUI();
	}


}

void AActionPFPlayerController::OnUnPossess()
{
	ClearForInteraction();

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
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AActionPFPlayerController::InteractFocusedInteraction);

		UCustomInputSettingSubsystem::GetInstance()->RegisterAffectedController(this);

		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddPlayerMappableConfig(ControllerInput);
	}
}



void AActionPFPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	SetGenericTeamId(1);

	//NPC와의 상호작용을 위한 예상 버튼개수만큼 미리 메모리 잡기
	NPCInteractBTNs.Reserve(4);

	//겹치게 될 Interaction들의 예상 최대개수만큼 미리 메모리 잡기 : 한 Actor에 여러개의 상호작용이 있을 수도 있으니
	PrevTracedInteractions.Reserve(4);

	Inventory->OnChangedInventory.AddDynamic(this, &AActionPFPlayerController::UpdateInventoryWidget);
}

void AActionPFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ChangeGameInputMode();
	if (!PlayerMainSlate.IsValid()) { CreatePlayerSlates(); }


	SetInputMode(FInputModeGameAndUI());
	SetShowMouseCursor(true);
}





void AActionPFPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (TeamID != NewTeamID) {
		TeamID = NewTeamID;
	}
}





UWidget_PlayerMainUI* AActionPFPlayerController::CreatePlayerMainUI()
{
	if (PlayerMainUIClass.GetDefaultObject() == nullptr)
	{
		PFLOG(Error, TEXT("Not Allocated Main UI Class."));
		return nullptr;
	}

	if (IsValid(PlayerMainUI))
	{
		PlayerMainUI->RemoveFromParent();
	}

	PlayerMainUI = CreateWidget<UWidget_PlayerMainUI>(this, PlayerMainUIClass, "Player Main UI");
	PlayerMainUI->AddToViewport(0);
	MainUIHideCount = 0;

	return PlayerMainUI;
}

void AActionPFPlayerController::CreatePlayerSlates()
{
	ForceHiddenSlateCount = 0;

	SAssignNew(PlayerMainSlate, SActionPFMainSlate)
		.Visibility(EVisibility::Visible);
		
	//PlayerDialogueMC Slate 묶기
	PlayerDialogueMC->BindDialogueBox(PlayerMainSlate->MainDialogueBox);
	PlayerDialogueMC->BindAnswerBox(PlayerMainSlate->AnswersVerticalBox);

	//컨트롤러 안에 있는 Slate Weak ptr이랑 묶기
	NPCInteractBTNBox = PlayerMainSlate->NPCInteractBTNBox;

	SButton* ExitNPCInteractBTN = static_cast<SButton*>(&NPCInteractBTNBox.Pin()->GetSlot(0).GetWidget().Get());

	ExitNPCInteractBTN->SetOnClicked(TDelegate<FReply()>::CreateUObject(this, &AActionPFPlayerController::OnClickExitInteractNPC));

	GEngine->GameViewport->AddViewportWidgetContent(PlayerMainSlate.ToSharedRef());
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



void AActionPFPlayerController::OpenMenu()
{
	UUserWidget* CurrentMenuWidget = GetMenuWidget();
	if(CurrentMenuWidget == nullptr || CurrentMenuWidget->GetVisibility() == ESlateVisibility::Visible) return;

	ChangeUIInputMode();
	CurrentMenuWidget->SetVisibility(ESlateVisibility::Visible);
	HideMainUI();
}

void AActionPFPlayerController::CloseMenu()
{
	UUserWidget* CurrentMenuWidget = GetMenuWidget();
	if (CurrentMenuWidget == nullptr || CurrentMenuWidget->GetVisibility() == ESlateVisibility::Collapsed) return;

	ChangeGameInputMode();
	CurrentMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	DisplayMainUI();
}

UUserWidget* AActionPFPlayerController::GetMenuWidget()
{
	if (!IsValid(MenuWidget)) {
		if (MenuWidgetClass.GetDefaultObject() == nullptr) return nullptr;

		MenuWidget = CreateWidget(this, MenuWidgetClass, "PlayerMenu");
		MenuWidget->AddToViewport(100);
		MenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (!IsValid(MenuWidget)) return nullptr;

	

	return MenuWidget;
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



/////////////////////// Slate /////////////////////



void AActionPFPlayerController::HideMainSlate()
{
	++ForceHiddenSlateCount;

	PlayerMainSlate->SetVisibility(EVisibility::Collapsed);
}

void AActionPFPlayerController::DisplayMainSlate()
{
	--ForceHiddenSlateCount;

	ensure(0 <= ForceHiddenSlateCount);

	if (ForceHiddenSlateCount == 0)
	{
		PlayerMainSlate->SetVisibility(EVisibility::SelfHitTestInvisible);
	}
}

void AActionPFPlayerController::SetCanInteract(bool NewState)
{
	if(bCanInteract == NewState) return;

	bCanInteract = NewState;

	if (bCanInteract)
	{
		
	}
	else
	{
		UnregisterInteractMapping();
	}
}

void AActionPFPlayerController::TraceInteractions()
{
	APawn* PlayerPawn = GetPawn();
	if (!IsValid(PlayerPawn)){return;}

	TArray<UInteractionSystemComponent*> CurrentTracedInteractions;

	//Trace를 위한 매개변수들
	static const float TraceDistance = 200;
	TArray<FHitResult> HitResults;
	FVector TraceStart = PlayerPawn->GetActorLocation();
	FVector TraceEnd = TraceStart + TraceDistance * PlayerPawn->GetActorForwardVector();
	FCollisionQueryParams CQP(FName("Player Trace Interactions"), false, PlayerPawn);

	//Trace
	//ECC_GameTraceChannel1 == InteractorSensor
	GetWorld()->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, ECC_GameTraceChannel1, CQP);


#if WITH_EDITOR
	if (bDrawTraceInteractionLine)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, HitResults.IsEmpty() ? FColor::Red : FColor::Green);
	}

#endif

	//Trace에 걸린 Actor들이 있을 시에 UInteractionSystemComponent를 TracedInteractions에 추가
	for (const auto& HitResult : HitResults)
	{
		AActor* TracedActor = HitResult.GetActor();
		if(!IsValid(TracedActor)) continue;

		TArray<UInteractionSystemComponent*> TempComponents;
		TracedActor->GetComponents<UInteractionSystemComponent>(TempComponents);
		for (const auto& TempComponent : TempComponents)
		{
			CurrentTracedInteractions.AddUnique(TempComponent);
		}
	}
	
	//기존에 이미 주목하고있는 상호작용 체크
	if(CurrentTracedInteractions.Contains(FocusedInteraction.Get())) { CurrentTracedInteractions.Remove(FocusedInteraction.Get()); }
	else if (FocusedInteraction.IsValid()) 
	{ 
		FocusedInteraction->OnFocusedOff();
		FocusedInteraction.Reset(); 		
	}

	
	int PrevInteractionsNum = PrevTracedInteractions.Num();
	for (int i = 0; i < PrevInteractionsNum; i++)
	{
		if (CurrentTracedInteractions.Contains(PrevTracedInteractions[i].Get()))
		{
			CurrentTracedInteractions.Remove(PrevTracedInteractions[i].Get());
		}
		else
		{
			PrevTracedInteractions.RemoveAt(i);
			--i;
			--PrevInteractionsNum;
		}
	}

	PrevTracedInteractions.Append(CurrentTracedInteractions);

	if (!CheckFocusedInteraction())
	{
		FocusNextInteraction();
	}

	if (!FocusedInteraction.IsValid() && PrevTracedInteractions.IsEmpty())
	{
		UnregisterInteractMapping();
	}
}

void AActionPFPlayerController::AddInteraction(UInteractionSystemComponent* NewInteraction)
{
	//상호작용이 유효한가 체크
	if(NewInteraction == nullptr || !IsValid(NewInteraction)) return;
	
	//상호작용이 이미 등록되어 있는가 체크
	if (NewInteraction == FocusedInteraction.Get() || PrevTracedInteractions.Find(NewInteraction) != INDEX_NONE) return;

	//주목하고 있는 상호작용이 없다면 바로 주목하기
	if (!FocusedInteraction.IsValid())
	{
		FocusInteraction(NewInteraction);
	}
	else //없다면 그냥 배열에 넣기
	{
		PrevTracedInteractions.Add(NewInteraction);
	}
}

void AActionPFPlayerController::RemoveInteraction(UInteractionSystemComponent* NewInteraction)
{
	//상호작용이 유효한가 체크
	if (NewInteraction == nullptr || !IsValid(NewInteraction)) return;

	if (NewInteraction == FocusedInteraction)
	{
		FocusedInteraction.Reset();
		return;
	}

	PrevTracedInteractions.Remove(NewInteraction);
}

void AActionPFPlayerController::FocusInteraction(UInteractionSystemComponent* NewInteraction)
{
	//상호작용이 유효한가 체크
	if (NewInteraction == nullptr || !IsValid(NewInteraction)) return;

	if (FocusedInteraction.IsValid())
	{
		FocusedInteraction->OnFocusedOff();
		PrevTracedInteractions.Add(FocusedInteraction);
	}
	else
	{
		RegisterInteractMapping();
	}

	FocusedInteraction = NewInteraction;
	FocusedInteraction->OnFocusedOn();
}

void AActionPFPlayerController::FocusNextInteraction()
{
	if(PrevTracedInteractions.IsEmpty()) return;

	int PrevTracedNum = PrevTracedInteractions.Num();
	for (int i = 0; i < PrevTracedNum; i++)
	{
		if (!PrevTracedInteractions[i].IsValid())
		{
			PrevTracedInteractions.RemoveAt(i);
			--i;
			--PrevTracedNum;
			continue;
		}

		if (PrevTracedInteractions[i]->IsEnableInteract())
		{
			FocusInteraction(PrevTracedInteractions[i].Get());
			PrevTracedInteractions.RemoveAt(i);
			break;
		}
	}
}

bool AActionPFPlayerController::CheckFocusedInteraction()
{
	return FocusedInteraction.IsValid() && FocusedInteraction->IsEnableInteract();
}



void AActionPFPlayerController::InteractFocusedInteraction()
{
#if WITH_EDITOR
	PFLOG(Warning, TEXT("Try Interact FocusedInteraction"));
#endif
	if(!FocusedInteraction.IsValid()) return;
#if WITH_EDITOR
	PFLOG(Warning, TEXT("Call Interact FocusedInteraction"));
#endif

	FocusedInteraction->Interact(GetPawn());
}

void AActionPFPlayerController::ClearForInteraction()
{
	if (!PrevTracedInteractions.IsEmpty()) PrevTracedInteractions.Empty();
	if (FocusedInteraction.IsValid()) {
		FocusedInteraction->OnFocusedOff();
		FocusedInteraction.Reset();
	}
}

void AActionPFPlayerController::RegisterInteractMapping()
{
	if(bRegisteredInteractMapping) return;
	UCustomInputHelper::AddInputMapping(this, InteractMapping, interactMappingPriority);
	bRegisteredInteractMapping = true;
}

void AActionPFPlayerController::UnregisterInteractMapping()
{
	if(!bRegisteredInteractMapping) return;
	UCustomInputHelper::RemoveInputMapping(this, InteractMapping);
}


/////////////////////// Interact /////////////////////

void AActionPFPlayerController::InteractWithNPC(UInteractionSystemComponent_NPC* NPCInteractionSystem)
{
	if (!IsValid(NPCInteractionSystem)){ return; }
	////////////////////////
	ChangeUIInputMode();

	HideMainUI();
	AssginNPCInteractButton(NPCInteractionSystem);

	const UDialogueSession* GreetingDialogue = NPCInteractionSystem->GetGreetingDialogue(this);

	if (GreetingDialogue)
	{
		EnterDialogueInNPCInteract(GreetingDialogue);
	}
	else
	{
		ShowNPCInteractBTNs();
	}
}

void AActionPFPlayerController::AssginNPCInteractButton(UInteractionSystemComponent_NPC* NPCInteracts)
{
	TArray<UNPCInteract*> AbleNPCInteracts =  NPCInteracts->GetAbleNPCInteractions(this);

	const int AbleInteractCount = AbleNPCInteracts.Num();
	const int NPCInteractBTNCount = NPCInteractBTNs.Num();

	CheckAndCreateNPCinteractBTNs(AbleInteractCount);

	for (int i = 0; i < AbleInteractCount; ++i)
	{
		NPCInteractBTNs[i].Pin()->BindNPCInteract(this, AbleNPCInteracts[i]);
	}

	for (int i = AbleInteractCount; i < NPCInteractBTNCount; ++i)
	{
		NPCInteractBTNs[i].Pin()->SetVisibility(EVisibility::Collapsed);
	}
}

void AActionPFPlayerController::CheckAndCreateNPCinteractBTNs(int TargetCount)
{
	ensure(NPCInteractBTNBox.Pin().IsValid());
	
	//충분하면 바로 return
	if (TargetCount <= NPCInteractBTNs.Num()) { return; }

	const float BTNBotPadding = 20;

	for (int i = NPCInteractBTNs.Num(); i < TargetCount; ++i)
	{
		TSharedPtr<SNPCInteractButton> TempButton;
		SAssignNew(TempButton, SNPCInteractButton)
			.Visibility(EVisibility::Visible);

		NPCInteractBTNBox.Pin()->
		InsertSlot(NPCInteractBTNs.Num())
		.HAlign(HAlign_Fill)
		.AutoHeight()
		.Padding(0, 0, 0, BTNBotPadding)
			[
				TempButton.ToSharedRef()
			];

		NPCInteractBTNs.Add(TempButton);
	}
}

void AActionPFPlayerController::ShowNPCInteractBTNs()
{
	#if WITH_EDITOR
		PFLOG(Warning, TEXT("Show NPCInteract Buttons"));
	#endif

	NPCInteractBTNBox.Pin()->SetVisibility(EVisibility::SelfHitTestInvisible);
}

void AActionPFPlayerController::HideNPCInteractBTNs()
{
	NPCInteractBTNBox.Pin()->SetVisibility(EVisibility::Collapsed);
}

void AActionPFPlayerController::ExitInteractNPC()
{
#if WITH_EDITOR
	PFLOG(Warning,TEXT("Call ExitInteract NPC"));
#endif
	ChangeGameInputMode();
	DisplayMainUI();
	HideNPCInteractBTNs();

}

FReply AActionPFPlayerController::OnClickExitInteractNPC()
{
	ExitInteractNPC();

	return FReply::Unhandled();
}

void AActionPFPlayerController::EnterDialogueInNPCInteract(const UDialogueSession* NewSession)
{
	if (NewSession == nullptr)
	{
		PFLOG(Error, TEXT("Call By null Dialogue Session."));
		return;
	}

	PlayerDialogueMC->EnterDialogue(NewSession);
	PlayerDialogueMC->TryEnterNextNode();
	PlayerDialogueMC->GetExitDialogueOnceDelegate().AddLambda([&](bool bIsCancelled) {ShowNPCInteractBTNs(); });
	HideNPCInteractBTNs();
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


bool AActionPFPlayerController::TryEquipItemInInventory(APlayerCharacter* Target, int idx)
{
	if(!IsValid(Target)) return false;

	FInventorySlot* InventorySlot = Inventory->GetInventorySlot(EItemType::Equipment, idx);

	if (InventorySlot == nullptr || InventorySlot->IsEmpty())
	{
		return false;
	}

	UItemBase_Equipment* TargetEquipment = Cast<UItemBase_Equipment>(InventorySlot->GetItemInSlot());
	UItemBase_Equipment* AlreadyEquipment = Target->GetEquipment(TargetEquipment->GetEquipmentPart());

	bool bResult = Target->EquipItem(TargetEquipment);

	if(bResult)
	{
		InventorySlot->SetSlot(AlreadyEquipment, 1);
		UpdateInventoryWidget(EItemType::Equipment, idx);
	}

	return bResult;
}

bool AActionPFPlayerController::TryUnequipItem(APlayerCharacter* Target, EEquipmentPart Part)
{
	FInventorySlot* EmptySlot = Inventory->GetEmptySlot(EItemType::Equipment);
	if(EmptySlot == nullptr) return false;

	return Target->UnequipItem(Target->GetEquipment(Part));
}


void AActionPFPlayerController::UpdateInventoryWidget(EItemType InventoryType, int Idx)
{
	FInventorySlot* InventorySlot = Inventory->GetInventorySlot(InventoryType, Idx);
	if (InventorySlot == nullptr)
	{
		ensureMsgf(false, TEXT("Can't find Inventory Slot."));
		return;
	}

	TSoftObjectPtr<UMaterialInterface> UpdateIcon;
	int UpdateCount = 0;
	EItemGrade UpdateItemGrade = EItemGrade::None;


	if (!InventorySlot->IsEmpty())
	{
		UItemManagerSubsystem* ItemManager = UItemManagerSubsystem::GetManagerInstance();
		const UItemBase* TempItem = InventorySlot->GetItemInSlot();

		UpdateIcon = TempItem->GetIconMaterial();
		UpdateCount = InventorySlot->GetCount();
		UpdateItemGrade = TempItem->GetItemGrade();
	}

	PlayerMainUI->UpdateInventorySlot(InventoryType, Idx, UpdateIcon, UpdateItemGrade, UpdateCount);
}
