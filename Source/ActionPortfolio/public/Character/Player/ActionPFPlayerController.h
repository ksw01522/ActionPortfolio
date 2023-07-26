// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
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
	Basic,
	Auto
};

UCLASS()
class ACTIONPORTFOLIO_API AActionPFPlayerController : public APlayerController, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	AActionPFPlayerController();

private:
	FTimerHandle DialogueAnimHandle_NPC;
	FTimerDelegate DialogueAnimDel_NPC;

protected:
	virtual void Tick(float DeltaSeconds) override;


///////////////////입력
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* ControllerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* OpenMenuAction;

protected:
	virtual void SetupInputComponent() override;

	

///////////////////메뉴 위젯
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UserWidget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> MenuWidgetClass;

	UUserWidget* MenuWidget;
	
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Player")
	void OpenMenu();

	UFUNCTION(BlueprintCallable, Category = "ActionPF|Player")
	void CloseMenu();

	UUserWidget* GetMenuWidget();

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


protected:
	TSharedPtr<class SDialogueSlate> DialogueSlate;
	FTimerHandle DialogueAnimHandle_Basic;
	FTimerDelegate DialogueAnimDel_Basic;

	void EndDialogueSlate();

public:
	void OnMouseButtonDownInDialogueBox();

	UFUNCTION(BlueprintCallable, Category = "ActionPF|Dialogue")
	void EnterDialogueBasic(class UDialogueSession* NewSession);

//////////////////////// Team /////////////////////
private:
	FGenericTeamId TeamID;
public:
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const { return TeamID; }
};
