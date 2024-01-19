// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionSystemComponent.h"
#include "InteractionSystemComponent_NPC.generated.h"

class AActionPFPlayerController;
class UDialogueSession;
class UDialoguerComponent;

UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class ACTIONPORTFOLIO_API UInteractionSystemComponent_NPC : public UInteractionSystemComponent
{
	GENERATED_BODY()
public:	
	UInteractionSystemComponent_NPC();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPCInteraction", Instanced, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UNPCInteract>> NPCInteractions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPCInteraction", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UDialogueSession>> GreetingDialogueSession;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPCInteraction", meta = (AllowPrivateAccess = "true"))
	TArray<FString> ExtraDialoguerIDs;

	TWeakObjectPtr<UDialoguerComponent> DialoguerComponent;

private:
	virtual bool CanInteract_CPP(AActor* InteractActor) const override;
	virtual void Interact_CPP(AActor* InteractActor) override;

protected:
	virtual void BeginPlay() override;


public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NPCInteraction")
	const UDialogueSession* GetGreetingDialogue(const AActionPFPlayerController* PlayerController) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NPCInteraction")
	const TArray<UNPCInteract*>& GetNPCInteractions() const {return NPCInteractions;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NPCInteraction")
	const TArray<UNPCInteract*> GetAbleNPCInteractions(class AActionPFPlayerController* PlayerController) const;

};



////////////////////////////// NPCInteract ////////////////////////////
UCLASS(DefaultToInstanced, ClassGroup = (NPCInteract), BlueprintType, Abstract, Blueprintable, EditInlineNew)
class ACTIONPORTFOLIO_API UNPCInteract : public UObject
{
	GENERATED_BODY()

public:
	UNPCInteract();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPCInteraction", BlueprintGetter = GetNPCInteractionName, meta = (AllowPrivateAccess = "true"))
	FText NPCInteractionName;

	TWeakObjectPtr<UInteractionSystemComponent_NPC> OwnerSystem;

public:
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "NPCInteraction", BlueprintGetter)
	const FText& GetNPCInteractionName() const { return NPCInteractionName; }

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category = "NPCInteraction")
	bool IsCanNPCInteract(AActionPFPlayerController* InteractPlayer) const;
	virtual bool IsCanNPCInteract_Implementation(AActionPFPlayerController* InteractPlayer) const { return false; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "NPCInteraction")
	void NPCInteract(AActionPFPlayerController* InteracPlayer) const;
	virtual void NPCInteract_Implementation(AActionPFPlayerController* InteractPlayer) const {}

	void SetOwnerSystem(UInteractionSystemComponent_NPC* NewOwner) {OwnerSystem = NewOwner;}
};
/**
 *
 */
UCLASS()
class ACTIONPORTFOLIO_API UNPCInteract_Dialogue : public UNPCInteract
{
	GENERATED_BODY()

public:
	UNPCInteract_Dialogue();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCDialogue", meta = (AllowPrivateAccess = "true"))
	class UDialogueSession* DialogueSession;


public:

public:
	virtual bool IsCanNPCInteract_Implementation(AActionPFPlayerController* InteractPlayer) const;
	virtual void NPCInteract_Implementation(AActionPFPlayerController* InteractPlayer) const;
};

