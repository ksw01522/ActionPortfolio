// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogueStructs.h"
#include "DialogueManager.generated.h"

/**
 * 
 */
 class UDialoguerComponent;
 class UDialogueNode;
 class UDialogueNode_Basic;
 class UDialogueSession;
 class UDialogueEvent;
 struct FAnimInDialogueStruct;

USTRUCT(BlueprintType)
struct FDialogueHandle
{
	GENERATED_BODY()

	friend UDialogueManager;

	FDialogueHandle() {}
	FDialogueHandle(uint32 ID) : ID(ID) {}

private:
	void Invalidate() { ID = INDEX_NONE; }

public:
	bool IsValid() const { return ID != INDEX_NONE; }

	bool operator==(const FDialogueHandle& Other) const { return ID == Other.ID; }
	bool operator!=(const FDialogueHandle& Other) const { return ID != Other.ID; }

	friend uint32 GetTypeHash(const FDialogueHandle& InHandle)
	{
		return InHandle.ID;
	}

private:
	uint32 ID = INDEX_NONE;
 };

USTRUCT(BlueprintType)
struct DIALOGUERUNTIME_API FActingDialogueData
{
	GENERATED_BODY()

	friend UDialogueManager;

	FActingDialogueData() : DialogueSession(nullptr), CurrentNode(nullptr) { Dialoguers.Empty(); DialogueEvents.Empty(); }
	FActingDialogueData(TArray<UDialoguerComponent*> NewDialoguers, UDialogueSession* Session);
	~FActingDialogueData() {}

private:
	TArray<TWeakObjectPtr<UDialoguerComponent>> Dialoguers;

	TWeakObjectPtr<UDialogueSession> DialogueSession;

	TWeakObjectPtr<UDialogueNode> CurrentNode;

	UPROPERTY(Transient)
	TArray<UDialogueEvent*> DialogueEvents;
private:

public:
	bool IsValidDialogue() const;
};
 


UCLASS(BlueprintType, Config = CustomDialogue)
class DIALOGUERUNTIME_API UDialogueManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UDialogueManager() {}

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


private:
	uint32 GDialogueHandleID = 0;

	UPROPERTY(Transient)
	TMap<FDialogueHandle, FActingDialogueData> ActingDialogueMap;

	EDialogueLanguage CurrentLanguage = EDialogueLanguage::Korean;

	TMap<FString, TWeakObjectPtr<UDialoguerComponent>> DialoguerMap;



private:

	void RemoveDialogue(const FDialogueHandle& Target);

	bool GetElementFromNode(TArray<FDialogueElement>& OutElements, UDialogueNode_Basic* BasicNode) const;
	void GetElementsFromData(TArray<FDialogueElement>& OutElements, FActingDialogueData* Data) const;

	FActingDialogueData* GetActingDialogueData(const FDialogueHandle& Handle);



public:
	UFUNCTION(BlueprintCallable, Category = "DialogueManager")
	FDialogueHandle EnterDialogue(const TArray<FString>& DialoguerIDs, UDialogueSession* Session);

	UFUNCTION(BlueprintCallable, Category = "DialogueManager")
	EDialogueNodeType EnterNextNode(TArray<FDialogueElement>& OutElements, UDialoguerComponent* Dialoguer);

	void SetCurrentLanguage(EDialogueLanguage Lan) { CurrentLanguage = Lan; SaveConfig(); }

	UFUNCTION(BlueprintCallable, Category = "DialoguerManager")
	bool GetDialoguersInDialog(TArray<UDialoguerComponent*>& OutDialoguers, const FDialogueHandle& Handle);

	bool CheckDialogueFromHandle(const FDialogueHandle& Handle);

	/////////////////Events
private:
	void RemoveAllEvents(FDialogueHandle& Handle);
	void RemoveAllEvents(FActingDialogueData* Data);

	void CallEndEvents(FActingDialogueData* Data, bool bIsCancelled);

	bool IsCanEnterNextNode(FDialogueHandle& Handle);

public:
	bool AddDialogueEvent(UDialogueEvent* NewEvent, const FDialogueHandle& Handle);
	bool RemoveDialogueEvent(UDialogueEvent* RemoveEvent, const FDialogueHandle& Handle);

	void RegisterDialoguer(UDialoguerComponent* NewDialoguer);
	void UnregisterDialoguer(UDialoguerComponent* TargetDialoguer);

private:
	TMap<int, TArray<UCustomDialogueEventObject*>> CustomEventMap;

public:
	void CallCustomEvent(int ID);

	UFUNCTION(BlueprintCallable, Category = "DialogueManager")
	class UCustomDialogueEventObject* MakeCustomEvent(int NewEventID, TSubclassOf<UCustomDialogueEventObject> EventClass);

/////////////////Events
public:
	void PlayAnimationInDialogue(TArray<FAnimInDialogueStruct>& AnimInDialogueStructs);
};
