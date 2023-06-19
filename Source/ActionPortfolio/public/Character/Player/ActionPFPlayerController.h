// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "ActionPFPlayerController.generated.h"

/**
 * 
 */
class UDialoguerComponent;
class SNPCInteractWidget;

UENUM()
enum class EActionPFDialogueType : uint8
{
	NPC,
	Auto
};

UCLASS()
class ACTIONPORTFOLIO_API AActionPFPlayerController : public APlayerController, public IAbilitySystemInterface
{
	GENERATED_BODY()

	AActionPFPlayerController();

private:
	FTimerHandle DialogueAnimHandle_NPC;
	FTimerDelegate DialogueAnimDel_NPC;

protected:
	TSharedPtr<SNPCInteractWidget> NPCInteractWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialoguer")
	UDialoguerComponent* PlayerDialoguer;

private:
	void EnterNextDialogue(EActionPFDialogueType Type);
	void AnimDialogue(EActionPFDialogueType Type);

	void ForceDialogueAnimComplete(EActionPFDialogueType Type);

	void OnCompleteDialogueAnim(EActionPFDialogueType Type);

protected:
	void ChangeUIInputMode();
	void ChangeGameInputMode();

public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	void InteractWithNPC(class UInteractionSystemComponent_NPC* NPCInteracts);
	
	void ExitInteractNPC();
	void OnMouseButtonDownInDialogueBox_NPC();

	FString GetDialoguerID() const;

	void OnEnterDialogue(EActionPFDialogueType Type);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

};
