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
class UWidget_PlayerMainUI;
class UUserWidget;
class UInteractionSystemComponent_NPC;
class UPlayerDialogueMCComponent;
class SActionPFMainSlate;
class SNPCInteractButton;
class UDialogueSession;

struct FDialogueElementContainer;

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

protected:
	virtual void Tick(float DeltaSeconds) override;


///////////////////입력
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* ControllerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* OpenMenuAction;

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;

	

///////////////////메뉴 위젯
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UserWidget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWidget_PlayerMainUI> PlayerMainUIClass;

	UPROPERTY()
	UWidget_PlayerMainUI* PlayerMainUI;

	UWidget_PlayerMainUI* CreatePlayerMainUI();

	int MainUIHideCount = 0;


public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Player|UI")
	void HideMainUI();
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Player|UI")
	void DisplayMainUI();


private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UserWidget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY()
	UUserWidget* MenuWidget;
	
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Player")
	void OpenMenu();

	UFUNCTION(BlueprintCallable, Category = "ActionPF|Player")
	void CloseMenu();

	UUserWidget* GetMenuWidget();

protected:
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void ChangeUIInputMode();

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void ChangeGameInputMode();

public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;



//////////////////////// Team /////////////////////
private:
	FGenericTeamId TeamID;
public:
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const { return TeamID; }

/////////////////////// Slate /////////////////////
private:
	TSharedPtr<SActionPFMainSlate> PlayerMainSlate;

	void CreatePlayerSlates();

private:
	int ForceHiddenSlateCount;

public:
	void HideMainSlate();
	void DisplayMainSlate();

/////////////////////// Player Dialogue MC ////////////////
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	UPlayerDialogueMCComponent* PlayerDialogueMC;

public:
	UPlayerDialogueMCComponent* GetPlayerDialogueMCComponent() const { return PlayerDialogueMC; };

/////////////////////// Interact NPC /////////////////////
private:
	TWeakPtr<SVerticalBox> NPCInteractBTNBox;
	TArray<TWeakPtr<SNPCInteractButton>> NPCInteractBTNs;

	void AssginNPCInteractButton(UInteractionSystemComponent_NPC* NPCInteracts);
	void CheckAndCreateNPCinteractBTNs(int TargetCount);

	void ShowNPCInteractBTNs();
	void HideNPCInteractBTNs();

public:
	void InteractWithNPC(UInteractionSystemComponent_NPC* NPCInteracts);

	void ExitInteractNPC();
	FReply OnClickExitInteractNPC();

	void EnterDialogueInNPCInteract(const UDialogueSession* NewSession);
};
