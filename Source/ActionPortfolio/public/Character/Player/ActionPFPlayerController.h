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
class UInteractionSystemComponent;
class UInteractionSystemComponent_NPC;
class UPlayerDialogueMCComponent;
class SActionPFMainSlate;
class SNPCInteractButton;
class UDialogueSession;
class APlayerCharacter;

class UInventoryComponent;

struct FDialogueElementContainer;

enum class EItemType : uint8;
enum class EEquipmentPart : uint8;



UCLASS()
class ACTIONPORTFOLIO_API AActionPFPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	AActionPFPlayerController();

protected:
	virtual void Tick(float DeltaSeconds) override;


///////////////////입력
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> OpenMenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerMappableInputConfig> ControllerInput;

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;
	

///////////////////메뉴 위젯
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UserWidget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWidget_PlayerMainUI> PlayerMainUIClass;

	UPROPERTY()
	TObjectPtr<UWidget_PlayerMainUI> PlayerMainUI;

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
	TObjectPtr<UUserWidget> MenuWidget;
	
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Player")
	void OpenMenu();

	UFUNCTION(BlueprintCallable, Category = "ActionPF|Player")
	void CloseMenu();

	UUserWidget* GetMenuWidget();


	TSharedPtr<class SInputKeyLabel> Temp;

protected:
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void ChangeUIInputMode();

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void ChangeGameInputMode();

public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;



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

///////////////////////// Interaction //////////////////////////
private:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug|Interaction", meta = (AllowPrivateAccess = "true"))
	bool bDrawTraceInteractionLine;
#endif

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InteractMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	int interactMappingPriority;

	bool bRegisteredInteractMapping;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Interaction", BlueprintSetter = SetCanInteract, meta = (AllowPrivateAccess = "true"))
	bool bCanInteract;

public:
	UFUNCTION(BlueprintSetter)
	void SetCanInteract(bool NewState);

private:

	TArray<TWeakObjectPtr<UInteractionSystemComponent>> PrevTracedInteractions;
	TWeakObjectPtr<UInteractionSystemComponent> FocusedInteraction;

	void TraceInteractions();

	void AddInteraction(UInteractionSystemComponent* NewInteraction);
	void RemoveInteraction(UInteractionSystemComponent* NewInteraction);
	void FocusInteraction(UInteractionSystemComponent* NewInteraction);

	void FocusNextInteraction();

	bool CheckFocusedInteraction();


	void InteractFocusedInteraction();

	void ClearForInteraction();

	void RegisterInteractMapping();
	void UnregisterInteractMapping();

/////////////////////// Player Dialogue MC ////////////////
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerDialogueMCComponent> PlayerDialogueMC;

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

	/////////////////////// Inventory /////////////////////
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryComponent> Inventory;

	UFUNCTION()
	void UpdateInventoryWidget(EItemType InventoryType, int Idx);

public:
	void PickUpItem(class ADroppedItem* Target);


public:
	bool TryEquipItemInInventory(APlayerCharacter* Target, int idx);
	bool TryUnequipItem(APlayerCharacter* Target, EEquipmentPart Part);
	
};
