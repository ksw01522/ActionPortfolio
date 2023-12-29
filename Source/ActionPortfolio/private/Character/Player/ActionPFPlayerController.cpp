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

AActionPFPlayerController::AActionPFPlayerController()
{
	PlayerDialogueMC = CreateDefaultSubobject<UPlayerDialogueMCComponent>("PlayerDialogueMC");

#if WITH_EDITORONLY_DATA
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
	Super::OnUnPossess();
}

void AActionPFPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		
		EnhancedInputComponent->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &AActionPFPlayerController::OpenMenu);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AActionPFPlayerController::InteractFocusedInteraction);
	
		

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ControllerMappingContext, 0);
		}
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
}

void AActionPFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ChangeGameInputMode();
	if (!PlayerMainSlate.IsValid()) { CreatePlayerSlates(); }

	
}







UAbilitySystemComponent* AActionPFPlayerController::GetAbilitySystemComponent() const
{
	AActionPortfolioCharacter* ControlPawn = GetPawn<AActionPortfolioCharacter>();
	if(!IsValid(ControlPawn)) return nullptr;

	return ControlPawn->GetAbilitySystemComponent();
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

	ExitNPCInteractBTN->SetOnClicked(TBaseDelegate<FReply>::CreateUObject(this, &AActionPFPlayerController::OnClickExitInteractNPC));

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

		TArray<UActorComponent*> TempComponents = TracedActor->GetComponentsByClass(UInteractionSystemComponent::StaticClass());
		for (const auto& TempComponent : TempComponents)
		{
			UInteractionSystemComponent* TempInteraction = Cast<UInteractionSystemComponent>(TempComponent);
			if(TempInteraction) CurrentTracedInteractions.AddUnique(TempInteraction);
		}
	}
	
	//기존에 트레이스에 걸린 상호작용이 벗어나면 목록에서 삭제
	int PrevInteractionsNum = PrevTracedInteractions.Num();
	for (int i = 0; i < PrevInteractionsNum; i++)
	{
		if (CurrentTracedInteractions.Contains(PrevTracedInteractions[i])) continue;

		PrevTracedInteractions.RemoveAt(i);
		--i;
		--PrevInteractionsNum;
	}

	//이번에 트레이스에 걸린 상호작용들을 AddUnique로 중복적용을 피하며 등록
	for (auto& NewInteraction : CurrentTracedInteractions)
	{
		PrevTracedInteractions.AddUnique(NewInteraction);
	}

	//새로이 Focus가 될 상호작용이 있는지 체크
	CheckFocusInteraction();
}

void AActionPFPlayerController::CheckValidInteraction()
{
	int Length = PrevTracedInteractions.Num();
	for (int i = 0; i < Length; i++)
	{
		if (PrevTracedInteractions[i].IsValid()) continue;
		
		PrevTracedInteractions.RemoveAt(i);
		--i;
		--Length;
	}
}

void AActionPFPlayerController::CheckFocusInteraction()
{
	UInteractionSystemComponent* NewFocusInteraction = nullptr;
	if(!PrevTracedInteractions.IsEmpty()) NewFocusInteraction = PrevTracedInteractions[0].Get();
	if(FocusInteraction == NewFocusInteraction) return;

	if (FocusInteraction.IsValid())
	{
		FocusInteraction->OnFocusedOff();
	}

	FocusInteraction = NewFocusInteraction;
	if(FocusInteraction.IsValid()) FocusInteraction->OnFocusedOn();
}

void AActionPFPlayerController::InteractFocusedInteraction()
{
#if WITH_EDITOR
	PFLOG(Warning, TEXT("Try Interact FocusedInteraction"));
#endif
	if(!FocusInteraction.IsValid()) return;
#if WITH_EDITOR
	PFLOG(Warning, TEXT("Call Interact FocusedInteraction"));
#endif

	FocusInteraction->Interact(GetPawn());
}

void AActionPFPlayerController::ClearForInteraction()
{
	if (!PrevTracedInteractions.IsEmpty()) PrevTracedInteractions.Empty();
	if (FocusInteraction.IsValid()) {
		FocusInteraction->OnFocusedOff();
		FocusInteraction.Reset();
	}
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

