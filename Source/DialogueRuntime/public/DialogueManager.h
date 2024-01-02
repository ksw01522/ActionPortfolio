// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Manager/DialogueMCManagerInterface.h"
#include "Manager/DialoguerManagerInterface.h"
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

public:
	bool IsValidDialogue() const;
};
 
 struct UDialogueElelement;
 
UCLASS(BlueprintType, Config = CustomDialogue)
class DIALOGUERUNTIME_API UDialogueManager : public UGameInstanceSubsystem, public IDialogueMCManagerInterface, public IDialoguerManagerInterface
{
	GENERATED_BODY()
	
private:
	UDialogueManager();

private:
	static UDialogueManager* ManagerInstance;

public:
	static UDialogueManager* GetManagerInstance() {return ManagerInstance;}
	static IDialoguerManagerInterface* GetDialoguerManager();
	static IDialogueMCManagerInterface* GetDialogueMCManager();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


private:
	uint32 GDialogueHandleID = 0;

	TMap<FDialogueHandle, FActingDialogueData> ActingDialogueMap;

	UPROPERTY(Config)
	EDialogueLanguage CurrentLanguage;

	TMap<FString, TWeakObjectPtr<UDialoguerComponent>> DialoguerMap;

	const FActingDialogueData* GetActingDialogueData(const FDialogueHandle& Handle) const;
	FActingDialogueData* GetActingDialogueData(const FDialogueHandle& Handle);




/////////////////////////////////// Dialogue MC Manager ///////////////////////////////
private:
	virtual void EnterDialogue(FDialogueHandle& Handle, const UDialogueSession* Session) override;
	virtual bool ExitDialogue(FDialogueHandle& Handle) override;

	virtual bool EnterNextNode(FDialogueElementContainer& OutElements, FDialogueHandle& Handle, FNextDialogueNodeOptionalStruct* OptionalStruct = nullptr) override;

	virtual bool CanEnterNextNode(FDialogueHandle& Handle) const override;

	virtual bool IsActingDialogue(FDialogueHandle& Handle) const override;


public:
	inline EDialogueLanguage GetCurrentLanguage() const { return CurrentLanguage; }
	void SetCurrentLanguage(EDialogueLanguage Lan);


	/////////////////Events
private:
	void RegisterAndCallEvents(FActingDialogueData* DialogueData);

	void CallDialogueEventsEnd(FActingDialogueData* Data, bool bIsCancelled);


public:



///////////////// Dialoguer Manager ///////////////////////////
private:
	virtual void RegisterDialoguer(UDialoguerComponent* NewDialoguer) override;
	virtual void UnregisterDialoguer(UDialoguerComponent* TargetDialoguer) override;

public:
	UDialoguerComponent* GetDialoguer(const FString& DialoguerID) const;

///////////////// Animation
public:
	void PlayAnimationInDialogue(TArray<FAnimInDialogueStruct>& AnimInDialogueStructs);
};
