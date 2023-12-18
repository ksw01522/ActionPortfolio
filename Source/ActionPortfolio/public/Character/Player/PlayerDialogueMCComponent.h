// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueMCComponent.h"
#include "PlayerDialogueMCComponent.generated.h"

class SDialogueBox;
class UActionPFPlayerController;
class SConstraintCanvas;
class SVerticalBox;
class SButton;
class FReply;
class SAnswerButton;
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

	virtual void OnSuccedEnterNextNode(FDialogueElementContainer& Container) override;

	virtual void OnEnterDialogue() override;

	virtual void OnExitDialogue(bool bIsCancelled) override;

//////////////// Dialouge Slate
private:
	TWeakPtr<SDialogueBox> DefaultDialogueBox;
	
	TWeakPtr<SVerticalBox> AnswersBox;
	
	TArray<TWeakPtr<SAnswerButton>> AnswerButtons;

private:
	void CheckAnswerButtonCount(int Target);
	void HideAndShowAnswerButtons(int Target);

	void OnMouseButtonDownDialogueBox();

	FReply AnswerForQuestion(int Index);

	void SetAnswerButtonText(const FDialogueElementContainer& Container);

public:
	void BindDialogueBox(const TSharedPtr<SDialogueBox>& NewDialogueBox);
	void BindAnswerBox(const TSharedPtr<SVerticalBox>& NewAnswersBox);

	void SetMainDialogueBoxVisible(bool NewState);

////////////// Text Animation
private:
	FTimerHandle TextAnimHandle;

	bool bNeedShowAnswerBox;

private:
	void AnimText();
	void ForceTextAnimComplete();
	void OnTextAnimCompleted();

public:
	bool IsTextAnimating() const;
};
