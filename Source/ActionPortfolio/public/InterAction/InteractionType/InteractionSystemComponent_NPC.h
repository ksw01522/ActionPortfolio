// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionSystemComponent.h"
#include "InteractionSystemComponent_NPC.generated.h"

class AActionPFPlayerController;



UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class ACTIONPORTFOLIO_API UInteractionSystemComponent_NPC : public UInteractionSystemComponent
{
	GENERATED_BODY()
public:	
	UInteractionSystemComponent_NPC();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPCInteraction", Instanced, meta = (AllowPrivateAccess = "true"))
	TArray<UNPCInteract*> NPCInteractions;

protected:
	virtual void BeginPlay() override;

public:
	virtual bool IsCanInteract_Implementation(AActor* InteractActor) const override;
	virtual void Interact_Implementation(AActor* InteractActor) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category = "NPCInteraction")
	FText GetGreetingDialogue() const;
	virtual FText GetGreetingDialogue_Implementation() const {return FText::FromString(FString("Redefine GetGreetingDialogue Function.")); }
};



////////////////////////////// NPCInteract ////////////////////////////
UCLASS(DefaultToInstanced, ClassGroup = (NPCInteract), BlueprintType, Abstract, Blueprintable, EditInlineNew)
class ACTIONPORTFOLIO_API UNPCInteract : public UObject
{
	GENERATED_BODY()

public:
	UNPCInteract();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPCInteraction", BlueprintGetter = GetNPCInteractionName, meta = (AllowPrivateAccess = "true"))
	FText NPCInteractionName;

	UInteractionSystemComponent_NPC* OwnerSystem;

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

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "NPCDialogue")
	class UDialogueSession* DialogueSession;

public:
	virtual bool IsCanNPCInteract_Implementation(AActionPFPlayerController* InteractPlayer) const { return true; }
};

