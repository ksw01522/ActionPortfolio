// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Manager/DialogueHandle.h"
#include "DialogueMCComponent.generated.h"


class UDialogueSession;
class UDialogueManager;
class IDialogueMCManagerInterface;

struct FDialogueElementContainer;
struct FNextDialogueNodeOptionalStruct;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract )
class DIALOGUERUNTIME_API UDialogueMCComponent : public UActorComponent
{
	GENERATED_BODY()


	DECLARE_MULTICAST_DELEGATE(FOnEnterDialogueDelegate);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnExitDialogueDelegate, bool);

private:
	FOnEnterDialogueDelegate OnEnterDel;
	FOnEnterDialogueDelegate OnEnterOnceDel;

	FOnExitDialogueDelegate OnExitDel;
	FOnExitDialogueDelegate OnExitOnceDel;

public:
	FOnEnterDialogueDelegate& GetEnterDialogueDelegate() { return OnEnterDel; }
	FOnEnterDialogueDelegate& GetEnterDialogueOnceDelegate() { return OnEnterOnceDel; }

	FOnExitDialogueDelegate& GetExitDialogueDelegate() { return OnExitDel; }
	FOnExitDialogueDelegate& GetExitDialogueOnceDelegate() { return OnExitOnceDel; }

public:	
	// Sets default values for this component's properties
	UDialogueMCComponent();


private:
	FDialogueHandle Handle;

protected:
	bool bCanEnterNextNodeInMC;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void EnterDialogue(const UDialogueSession* Session);

	void TryExitDialogue();

	bool TryEnterNextNode(FNextDialogueNodeOptionalStruct* OptionalStruct = nullptr);

protected:
	virtual void OnExitDialogue(bool bIsCancelled);
	virtual void OnEnterDialogue();
	virtual void OnSuccedEnterNextNode(FDialogueElementContainer& Container) {};
	virtual void OnFailedEnterNextNode() {};

public:	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Dialogue", BlueprintPure)
	bool IsActingDialogue() const;

	FDialogueHandle& GetDialogueHandle() const { return const_cast<FDialogueHandle&>(Handle); }
	FDialogueHandle& GetDialogueHandle() {return Handle;}
	
};
