// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Manager/DialogueHandle.h"
#include "DialogueMCComponent.generated.h"


class UDialogueSession;
class UDialogueNode;
class UDialogueManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract )
class DIALOGUERUNTIME_API UDialogueMCComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueMCComponent();

private:
	FDialogueHandle Handle;

	const UDialogueSession* CurrentSession = nullptr;
	const UDialogueNode* CurrentNode = nullptr;

public:
	const UDialogueSession* GetCurrentSession() const {return CurrentSession;}
	const UDialogueNode* GetCurrentNode() const {return CurrentNode;}

protected:
	bool bCanEnterNextNodeInMC;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void EnterDialogue(const UDialogueSession* Session);

	void TryExitDialogue(bool bCancelled);

	bool TryEnterNextNode(const UDialogueNode* NextNode);

protected:
	virtual void OnExitDialogue(bool bIsCancelled){};
	virtual void OnEnterDialogue(){};
	virtual void OnSuccedEnterNextNode(const UDialogueNode* InNode) {};
	virtual void OnFailedEnterNextNode(const UDialogueNode* InNode) {};

public:	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Dialogue", BlueprintPure)
	bool IsActingDialogue() const;

	const FDialogueHandle& GetDialogueHandle() const { return Handle; }
	FDialogueHandle& GetDialogueHandle() {return Handle;}
};
