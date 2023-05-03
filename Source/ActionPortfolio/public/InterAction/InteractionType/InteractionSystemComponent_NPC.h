// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionSystemComponent.h"
#include "InteractionSystemComponent_NPC.generated.h"

class AActionPFPlayerController;

UCLASS(ClassGroup = (NPCInteract), BlueprintType, Abstract, Blueprintable, EditInlineNew)
class ACTIONPORTFOLIO_API UNPCInteract : public UObject
{
	GENERATED_BODY()

public:
	UNPCInteract();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPCInteraction", meta = (AllowPrivateAccess = "true"))
	FText InteractName;

public:
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "NPCInteraction")
	FText GetInteractName() const { return InteractName; }

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category = "NPCInteraction")
	bool IsCanNPCInteract(AActionPFPlayerController* InteractPlayer) const;
	virtual bool IsCanNPCInteract_Implementation(AActionPFPlayerController* InteractPlayer) const { return false; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "NPCInteraction")
	void NPCInteract(AActionPFPlayerController* InteracPlayer) const;
	virtual void NPCInteract_Implementation(AActionPFPlayerController* InteractPlayer) const {}
};
/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UInteractionSystemComponent_NPC : public UInteractionSystemComponent
{
	GENERATED_BODY()
public:	
	UInteractionSystemComponent_NPC();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPCInteraction", Instanced, meta = (AllowPrivateAccess = "true"))
	TArray<UNPCInteract*> NPCInteracts;


public:
	virtual bool IsCanInteract_Implementation(AActor* InteractActor) const override;
	virtual void Interact_Implementation(AActor* InteractActor) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category = "NPCInteraction")
	virtual FString GetGreetingsDialogue() const;
	virtual FString GetGreetingsDialogue_Implemetation() const {return TEXT("Redefine GetGreetingsDialogue Function."); }
};
