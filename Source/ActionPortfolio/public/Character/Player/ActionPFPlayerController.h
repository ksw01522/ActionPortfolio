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


enum class EItemType : uint8;
enum class EEquipmentPart : uint8;



UCLASS()
class ACTIONPORTFOLIO_API AActionPFPlayerController : public APlayerController, public IGenericTeamAgentInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()


	AActionPFPlayerController();

	///////////// Override
protected:
	virtual void Tick(float DeltaSeconds) override;

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;


///////////////////입력
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> OpenMenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> OpenInventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerMappableInputConfig> ControllerInput;

protected:
	virtual void SetupInputComponent() override;

private:
	TArray<TSharedRef<SWidget>> FocusedWidgetStack;
	
public:
	void AddCustomFocuseWidget(SWidget& InWidget, bool bSaveToStack = true);
	void AddCustomFocuseWidget(TSharedRef<SWidget> InWidget, bool bSaveToStack = true);

	void RemoveCustomFocuseWidgetStack();

///////////////////메뉴 위젯
private:
	UPROPERTY(Transient)
	TObjectPtr<UWidget_PlayerMainUI> PlayerMainUI;

	int MainUIHideCount = 0;

public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Player|UI")
	void HideMainUI();
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Player|UI")
	void DisplayMainUI();

	UWidget_PlayerMainUI* GetPlayerMainUI() const { return PlayerMainUI; }
	
	void SetPlayerMainUI(UWidget_PlayerMainUI* InMainUI);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UserWidget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> MenuWidget;
	
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Player")
	void OpenMenu();

	
public:
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void ChangeUIInputMode();

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void ChangeGameInputMode();


//////////////////////// Team /////////////////////
private:
	FGenericTeamId TeamID;
public:
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const { return TeamID; }

///////////////////////// Interaction //////////////////////////
private:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug|Interaction", meta = (AllowPrivateAccess = "true"))
	bool bDrawTraceInteractionLine;
#endif

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

	TWeakObjectPtr<AActor> CurrentFocusedActor;
	int FocusedInteractionIDX;
	TArray<UInteractionSystemComponent*> FocusedInteractions;

	TArray<AActor*> PrevTracedInteractionActors;
	TArray<AActor*> TracedInteractionActors;

private:
	void EmptyInteractions();

	void TraceInteractions();

	void OnChangedFocusedInteractionActor();
	
	void FocusNextInteraction();

	void InteractFocusedInteraction();

/////////////////////// Player Dialogue MC ////////////////
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerDialogueMCComponent> PlayerDialogueMC;

public:
	UPlayerDialogueMCComponent* GetPlayerDialogueMCComponent() const { return PlayerDialogueMC; };

	/////////////////////// Inventory /////////////////////
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryComponent> Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	TSubclassOf<class UUserWidget_PlayerInventory> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget_PlayerInventory> InventoryWidget;

	void CreateInventorySlate();

	void OpenInventory();

public:
	void CloseInventory();

public:
	UInventoryComponent* GetInventory() const {return Inventory;}

	void PickUpItem(class ADroppedItem* Target);


	/////////////////////// Lock On /////////////////////
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LockOn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LockOnAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LockOn, meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float LockOnSearchDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LockOn, meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float LockOnSustainableDistance;

	TWeakObjectPtr<class ULockOnTargetComponent> LockOnTarget;

private:
	void Tick_LockOn(float DeltaSeconds);

protected:
	virtual void AddPitchInput(float Val) override;

	virtual void AddYawInput(float Val) override;

	virtual bool CanLockOnTarget(ULockOnTargetComponent* InTarget) const;

	void PressLockOnAction();

	void SearchLockOnTarget();

	void ClearLockOnTarget();

public:
	ULockOnTargetComponent* GetLockOnTarget() const { return LockOnTarget.Get(); }


////////////////// IAbilitySystemComponent //////////////////////
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UCharacterStatusComponent* GetStatusComponent() const;
};
