// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueManager.h"
#include "Templates/SharedPointer.h"
#include "DialogueRuntime.h"
#include "Components/AudioComponent.h"
#include "SRichTextBlockDecorator.h"
#include "Components/RichTextBlockDecorator.h"
#include "Nodes/DialogueNode_Basic.h"
#include "Nodes/DialogueNode_Start.h"
#include "Engine/CompositeDataTable.h"
#include "DialogueMCComponent.h"
#include "DialogueSession.h"
#include "Events/DialogueEvent.h"
#include "DialoguerComponent.h"
#include "DialogueNode.h"
#include "DialogueDeveloperSettings.h"
#include "DialogueEdge.h"

////////////////////////////////////////// FActingDialogueData /////////////////////////////////////////////
FActingDialogueData::FActingDialogueData(const UDialogueSession* Session)
{
	DialogueEvents.Empty();
	DialogueSession = Session;

	if (DialogueSession.IsValid()) {
		CurrentNode = Session->GetStartNode();
	} else {CurrentNode.Reset(); }
}

FActingDialogueData::~FActingDialogueData()
{
	for (auto& Event : DialogueEvents)
	{
		Event->CallEndEvent(true);
	}
}

bool FActingDialogueData::IsValidDialogue() const
{
	return (DialogueSession.IsValid() && CurrentNode.IsValid());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

UDialogueManager* UDialogueManager::ManagerInstance = nullptr;

UDialogueManager::UDialogueManager()
{
}

void UDialogueManager::SetCurrentLanguage(EDialogueLanguage Lan)
{
	CurrentLanguage = Lan;
	SaveConfig();
}

void UDialogueManager::Initialize(FSubsystemCollectionBase& Collection)
{
	GDialogueHandleID = 0;
	ActingDialogueMap.Empty();
	DialoguerMap.Empty();

	ManagerInstance = this;

	const UDialogueDeveloperSettings* DeveloperSetting = GetDefault<UDialogueDeveloperSettings>();

	DialogueTextStyleSet = DeveloperSetting->GetDialogueTextStyleSet();
}

void UDialogueManager::Deinitialize()
{
	ManagerInstance = nullptr;

	ActingDialogueMap.Empty();
	DialoguerMap.Empty();
}



///////////////////////////////////////// Dialogue MC Manager ///////////////////////////////////////////
void UDialogueManager::EnterDialogue(FDialogueHandle& Handle, const UDialogueSession* Session)
{
	check(Session);

	ExitDialogue(Handle, true);
	
	Handle.SetHandleID(++GDialogueHandleID);
	FActingDialogueData Data(Session);

	ActingDialogueMap.Add(Handle, Data);
}

bool UDialogueManager::ExitDialogue(FDialogueHandle& Handle, bool bCancelled)
{
	bool bIsActing = IsActingDialogue(Handle);
	if (bIsActing)
	{
		ActingDialogueMap.Find(Handle)->GetEndDelegate().Broadcast(bCancelled);
		ActingDialogueMap.Remove(Handle);
	}
	Handle.Invalidate();

	return bIsActing;
}


bool UDialogueManager::TryEnterNextNode(FDialogueHandle& Handle, const UDialogueNode* NextNode)
{
	check(IsActingDialogue(Handle) && NextNode);

	FActingDialogueData* Data = GetActingDialogueData(Handle);

	const UDialogueNode* PrevNode = Data->CurrentNode.Get();
	const UDialogueEdge* NextEdge = PrevNode->GetEdge(NextNode);

	if(NextEdge == nullptr || !NextEdge->CanEnterNextNode()) return false;
	
	TArray<UDialogueEvent*>& Events = Data->DialogueEvents;

	for (auto& Event : Events)
	{
		if(!Event->CanEnterNextNode()) return false;
	}

	CallDialogueEventsEnd(Data, true);

	Data->CurrentNode = NextNode;

	RegisterAndCallEvents(Data);
	
	return true;
}

bool UDialogueManager::IsActingDialogue(FDialogueHandle& Handle) const
{
	if (!Handle.IsValid()) return false;

	if (!ActingDialogueMap.Contains(Handle)) {
		Handle.Invalidate();
		return false;
	}

	return true;
}



//////////////////////////////////////////////// Get ////////////////////////////////////////////////////////

// 등록된 대화 데이터 가져오기
const FActingDialogueData* UDialogueManager::GetActingDialogueData(const FDialogueHandle& Handle) const
{
	const FActingDialogueData* ReturnData = ActingDialogueMap.Find(Handle);

	return ReturnData;
}

FActingDialogueData* UDialogueManager::GetActingDialogueData(const FDialogueHandle& Handle)
{
	FActingDialogueData* ReturnData = ActingDialogueMap.Find(Handle);

	return ReturnData;
}


/////////////////////////////////////// Dialoguer ///////////////////////////////////////////////////

void UDialogueManager::RegisterAndCallEvents(FActingDialogueData* DialogueData)
{
	check(DialogueData);
	check(DialogueData->CurrentNode.IsValid());

	const TArray<UDialogueEvent*>& Events = DialogueData->CurrentNode->GetDialogueEvents();
	for (auto Event : Events)
	{
		UDialogueEvent* Inst = Event->GetEventForCall();
		if (Inst->GetInstancingPolicy() != EDialougeEventInstancingPolicy::NotInstanced)
		{
			DialogueData->DialogueEvents.Add(Inst);
		}

		Inst->CallEvent();
	}
}

void UDialogueManager::CallDialogueEventsEnd(FActingDialogueData* Data, bool bIsCancelled)
{
	ensure(Data);

	for (auto Event : Data->DialogueEvents) {
		Event->CallEndEvent(bIsCancelled);
	}

	Data->DialogueEvents.Empty();
}

void UDialogueManager::RegisterDialoguer(UDialoguerComponent* NewDialoguer)
{
	if(!IsValid(NewDialoguer) || DialoguerMap.Find(NewDialoguer->GetDialoguerID()) != nullptr) return;

	DialoguerMap.Add(NewDialoguer->GetDialoguerID(), NewDialoguer);
}

void UDialogueManager::UnregisterDialoguer(UDialoguerComponent* TargetDialoguer)
{
	if (!IsValid(TargetDialoguer)) return;

	DialoguerMap.Remove(TargetDialoguer->GetDialoguerID());
}

UDialoguerComponent* UDialogueManager::GetDialoguer(const FString& DialoguerID) const
{
	ensureMsgf(DialoguerMap.Contains(DialoguerID), TEXT("Can't find Dialoguer By : %s"), *DialoguerID);

	TWeakObjectPtr<UDialoguerComponent> ReturnDialoguer = *DialoguerMap.Find(DialoguerID);
	return ReturnDialoguer.Get();
}



void UDialogueManager::PlayAnimationInDialogue(TArray<FAnimInDialogueStruct>& AnimInDialogueStructs)
{
	for (auto& E : AnimInDialogueStructs)
	{
		if (!DialoguerMap.Contains(E.DialoguerID)) {
			LOG_ERROR(TEXT("Can't find Dialoguer for play Animation by key : %s"), *E.DialoguerID);
			continue;
		}

		UDialoguerComponent* TargetDialoguer = DialoguerMap[E.DialoguerID].Get();
		if (TargetDialoguer == nullptr)
		{
			LOG_ERROR(TEXT("Can't find Dialoguer for play Animation by key : %s"), *E.DialoguerID);
			DialoguerMap.Remove(E.DialoguerID);
			continue;
		}

		TargetDialoguer->PlayAnimationMontage(E.MontageToPlay);
	}
}



