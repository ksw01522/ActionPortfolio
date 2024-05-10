// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueMCComponent.h"
#include "PlayerDialogueMCComponent.generated.h"

class SDialogueMainSlate;
class UActionPFPlayerController;
class SConstraintCanvas;

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UPlayerDialogueMCComponent : public UDialogueMCComponent
{
	GENERATED_BODY()
	
	UPlayerDialogueMCComponent();

protected:
	virtual void BeginPlay() override;

private:
	virtual void OnExitDialogue(bool bIsCancelled) override;
	virtual void OnEnterDialogue() override;
	virtual void OnSuccedEnterNextNode(const class UDialogueNode* InNode) override;

private:
	FReply ClickAnswerButton(const UDialogueNode* InAnswerNode);

//////////////// Dialouge Slate
private:
	TSharedPtr<SDialogueMainSlate> DefaultDialogueSlate;

private:
	void OnActNextDialogue();
	void OnDialogueBoxAnimComplete(bool bForced);

};
