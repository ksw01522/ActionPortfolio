// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "DialogueEdge.generated.h"

/**
 * 
 */

 class UDialogueNode;
 class UDialogueSession;



UCLASS()
class DIALOGUERUNTIME_API UDialogueEdge : public UObject
{
	GENERATED_BODY()

public:
	UDialogueEdge();
	virtual ~UDialogueEdge();

	UPROPERTY(VisibleAnywhere, Category = "DialogueEdge")
	TObjectPtr<UDialogueSession> DialogueSession;

	UPROPERTY(BlueprintReadOnly, Category = "DialogueEdge")
	TObjectPtr<UDialogueNode> StartNode;

	UPROPERTY(BlueprintReadOnly, Category = "DialogueEdge")
	TObjectPtr<UDialogueNode> EndNode;

	UFUNCTION(BlueprintPure, Category = "DialogueEdge")
	UDialogueSession* GetDialogueSession();


#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "DialogueNode_Editor")
	bool bShouldDrawTitle = false;

	UPROPERTY(EditDefaultsOnly, Category = "DialogueNode_Editor")
	FText NodeTitle;

	UPROPERTY(EditDefaultsOnly, Category = "DialogueEdge")
	FLinearColor EdgeColor = FLinearColor(0.9f, 0.9f, 0.9f, 1.0f);
#endif

#if WITH_EDITOR
	virtual FText GetNodeTitle() const { return NodeTitle; }
	FLinearColor GetEdgeColor() { return EdgeColor; }

	virtual void SetNodeTitle(const FText& NewTitle);
#endif
private:
	UPROPERTY(EditAnywhere, Category = "DialogueCondition", instanced)
	TArray<TObjectPtr<UEnterNextCondtion>> EnterNextConditions;

public:
	bool CanEnterNextNode() const;
};

UCLASS(DefaultToInstanced, ClassGroup = (Dialogue), BlueprintType, Abstract, Blueprintable, EditInlineNew)
class DIALOGUERUNTIME_API UEnterNextCondtion : public UObject
{
	GENERATED_BODY()
	friend class UDialogueEdge;

protected:
	virtual bool CanEnterNextNode() const { return true; };
};