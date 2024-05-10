// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Manager/DialogueHandle.h"
#include "DialogueManager.generated.h"

/**
 * 
 */
 class UDialoguerComponent;
 class UDialogueNode;
 class UDialogueNode_Basic;
 class UDialogueSession;
 class UDialogueEvent;
 class UDataTable;
 class UDialogueMCComponent;

 struct FAnimInDialogueStruct;
 struct FDialogueElement;
 struct FNextDialogueNodeOptionalStruct;

 enum class EDialogueNodeType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueEndDelegate, bool, bCancelled);

UENUM(BlueprintType)
enum class EDialogueLanguage : uint8
{
	Korean		UMETA(DisplayName = "Korean"),
	English		UMETA(DisplayName = "English")
};




USTRUCT(BlueprintType)
struct DIALOGUERUNTIME_API FActingDialogueData
{
	GENERATED_BODY()

	friend UDialogueManager;

	FActingDialogueData() : DialogueSession(nullptr), CurrentNode(nullptr) {DialogueEvents.Empty(); }
	FActingDialogueData(const UDialogueSession* Session);
	~FActingDialogueData();

private:
	TWeakObjectPtr<const UDialogueSession> DialogueSession;

	TWeakObjectPtr<const UDialogueNode> CurrentNode;

	UPROPERTY(Transient)
	TArray<UDialogueEvent*> DialogueEvents;

private:
	FOnDialogueEndDelegate OnDialogueEnd;

public:
	bool IsValidDialogue() const;
	FOnDialogueEndDelegate& GetEndDelegate() {return OnDialogueEnd;}
};
 
 struct UDialogueElelement;
 
UCLASS(BlueprintType, Config = CustomDialogue)
class DIALOGUERUNTIME_API UDialogueManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UDialogueManager();

private:
	static UDialogueManager* ManagerInstance;

public:
	static UDialogueManager* GetManagerInstance() {return ManagerInstance;}

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UPROPERTY(Config)
	EDialogueLanguage CurrentLanguage;

public:
	inline EDialogueLanguage GetCurrentLanguage() const { return CurrentLanguage; }
	void SetCurrentLanguage(EDialogueLanguage Lan);

private:
	uint32 GDialogueHandleID = 0;

private:
	UPROPERTY()
	TObjectPtr<UDataTable> DialogueTextStyleSet;

public:
	UDataTable* GetDialogueTextStyleSet() const { return DialogueTextStyleSet; }

private:
	TMap<FDialogueHandle, FActingDialogueData> ActingDialogueMap;

public:
	const FActingDialogueData* GetActingDialogueData(const FDialogueHandle& Handle) const;
	FActingDialogueData* GetActingDialogueData(const FDialogueHandle& Handle);

public:
	void EnterDialogue(FDialogueHandle& Handle, const UDialogueSession* Session);
	bool ExitDialogue(FDialogueHandle& Handle, bool bCancelled = false);

	bool IsActingDialogue(FDialogueHandle& Handle) const;

	bool TryEnterNextNode(FDialogueHandle& Handle, const UDialogueNode* NextNode);







	/////////////////Events
private:
	void RegisterAndCallEvents(FActingDialogueData* DialogueData);

	void CallDialogueEventsEnd(FActingDialogueData* Data, bool bIsCancelled);


public:


///////////////////// Dialoguer /////////////////////////////
private:
	TMap<FString, TWeakObjectPtr<UDialoguerComponent>> DialoguerMap;

public:
	virtual void RegisterDialoguer(UDialoguerComponent* NewDialoguer);
	virtual void UnregisterDialoguer(UDialoguerComponent* TargetDialoguer);

	UDialoguerComponent* GetDialoguer(const FString& DialoguerID) const;

///////////////// Animation
public:
	void PlayAnimationInDialogue(TArray<FAnimInDialogueStruct>& AnimInDialogueStructs);
};
