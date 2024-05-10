// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractionType/InteractionSystemComponent_NPC.h"
#include "Character/Player/ActionPFPlayerController.h"
#include "ActionPortfolio.h"
#include "DialogueSession.h"
#include "Kismet/KismetMathLibrary.h"
#include "DialoguerComponent.h"
#include "DialogueBFL.h"
#include "InterAction/Slate/SNPCInteractionSlate.h"
#include "WidgetStyle/ActionPortfolioWidgetStyle.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Character/Player/PlayerDialogueMCComponent.h"


#define LOCTEXT_NAMESPACE "NPCIneract"

////////////////////////////// NPCInteract ////////////////////////////
UNPCInteract::UNPCInteract()
{

}

TSharedRef<class SButton> UNPCInteract::CreateInteractionButton(AActionPFPlayerController* InteractPlayer)
{
	FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();
	const FButtonStyle& BTNStyle = StyleSet->GetWidgetStyle<FButtonStyle>(InteractionStyle::ButtonStyle::Default);
	FVector2D ButtonSize = StyleSet->GetVector(InteractionStyle::ButtonStyle::DefaultSize);
	const FTextBlockStyle& TextStyle = StyleSet->GetWidgetStyle<FTextBlockStyle>(InteractionStyle::TextStyle::Default);

	return SNew(SButton)
			.ButtonStyle(&BTNStyle)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.OnClicked_UObject(this, &UNPCInteract::NPCInteract, InteractPlayer)
			[
				SNew(SBox)
				.Visibility(EVisibility::HitTestInvisible)
				.WidthOverride(ButtonSize.X)
				.HeightOverride(ButtonSize.Y)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(NPCInteractionName)
					.TextStyle(&TextStyle)
				]
			];

}

// Dialogue
UNPCInteract_Dialogue::UNPCInteract_Dialogue()
{
	NPCInteractionName = LOCTEXT("Name_Dialogue", "대화하기");

}

bool UNPCInteract_Dialogue::IsCanNPCInteract(AActionPFPlayerController* InteractPlayer) const
{
	return IsValid(InteractPlayer) && DialogueSession != nullptr;
}

FReply UNPCInteract_Dialogue::NPCInteract(AActionPFPlayerController* InteractPlayer)
{
	check(DialogueSession);
	InteractPlayer->GetPlayerDialogueMCComponent()->EnterDialogue(DialogueSession);

	return FReply::Handled();
}





///////////////////////////// InteractionSystemComponent_NPC ////////////////////////////////////
UInteractionSystemComponent_NPC::UInteractionSystemComponent_NPC()
{
	InteractionName = LOCTEXT("NPC InteractionSystem Name", "말 걸기");
}

void UInteractionSystemComponent_NPC::BeginPlay()
{
	Super::BeginPlay();

	for (auto Interaction : NPCInteractions) {
		if (Interaction == nullptr) {
			PFLOG(Error, TEXT("NPC Interaction is nullptr."));
			continue;
		}

		Interaction->SetOwnerSystem(this);
	}
	
}



bool UInteractionSystemComponent_NPC::CanInteract_CPP(AActor* InteractActor) const
{
	if(!IsValid(InteractActor)) return false;

	AActionPFPlayerController* InteractPlayerController = InteractActor->GetInstigatorController<AActionPFPlayerController>();
	if(InteractPlayerController == nullptr) return false;

	return true;
}



void UInteractionSystemComponent_NPC::Interact_CPP(AActor* InteractActor)
{
	ensure(IsValid(InteractActor));

	AActionPFPlayerController* InteractPlayerController = InteractActor->GetInstigatorController<AActionPFPlayerController>();
	if(!IsValid(InteractPlayerController)) return;

	CreateInteractionSlate(InteractPlayerController);
}

const UDialogueSession* UInteractionSystemComponent_NPC::GetGreetingDialogue(const AActionPFPlayerController* PlayerController) const
{
	if(GreetingDialogueSession.IsEmpty()) return nullptr;

	int32 Num = GreetingDialogueSession.Num();

	int32 Idx = UKismetMathLibrary::RandomInteger(Num);

	return GreetingDialogueSession[Idx];
}

const TArray<UNPCInteract*> UInteractionSystemComponent_NPC::GetAbleNPCInteractions(AActionPFPlayerController* PlayerController) const
{
	TArray<UNPCInteract*> ReturnInteractions;

	for (auto NPCInteract : NPCInteractions)
	{
		if(NPCInteract->IsCanNPCInteract(PlayerController)) ReturnInteractions.Add(NPCInteract);
	}

	return ReturnInteractions;
}

void UInteractionSystemComponent_NPC::CreateInteractionSlate_Implementation(AActionPFPlayerController* Player)
{
	FSlateStyleSet* StyleSet = FActionPortfolioWidgetStyle::Get();
	const FButtonStyle& ButtonStyle = StyleSet->GetWidgetStyle<FButtonStyle>(InteractionStyle::ButtonStyle::Default);
	FVector2D ButtonSize = StyleSet->GetVector(InteractionStyle::ButtonStyle::DefaultSize);
	const FTextBlockStyle& TextStyle = StyleSet->GetWidgetStyle<FTextBlockStyle>(InteractionStyle::TextStyle::Default);

	TSharedPtr<SNPCInteractionSlate> InteractionSlate;
	SAssignNew(InteractionSlate, SNPCInteractionSlate);

	for (auto& Interaction : NPCInteractions)
	{
		InteractionSlate->AddButton(Interaction->CreateInteractionButton(Player));
	}

	TWeakPtr<SNPCInteractionSlate> WeakSlate = InteractionSlate;
	
	InteractionSlate->AddButton(
		SNew(SButton)
		.ButtonStyle(&ButtonStyle)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.OnClicked_Lambda([Player, WeakSlate]()->FReply {
			if (WeakSlate.IsValid())
			{
				GEngine->GameViewport->RemoveViewportWidgetContent(WeakSlate.Pin().ToSharedRef());
				Player->RemoveCustomFocuseWidgetStack();
			}
			return FReply::Handled();
			})
			[
				SNew(SBox)
				.Visibility(EVisibility::HitTestInvisible)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.HeightOverride(ButtonSize.Y)
				.WidthOverride(ButtonSize.X)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("그만두기", "그만두기"))
					.TextStyle(&TextStyle)
				]
			]
	);

	GEngine->GameViewport->AddViewportWidgetContent(InteractionSlate.ToSharedRef(), 100);

	Player->AddCustomFocuseWidget(InteractionSlate.ToSharedRef());
}

#undef LOCTEXT_NAMESPACE

