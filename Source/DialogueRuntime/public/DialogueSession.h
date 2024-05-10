// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameplayTagContainer.h"
#include "DialogueSession.generated.h"

/**
 * 
 */
 class UDialogueNode_Start;
 class UDialogueNode;
 class UDialogueNode_Basic;
 class UDataTable;

 enum class EDialogueLanguage : uint8;

 UENUM(BlueprintType)
	 enum class ECustomNodeTypeHidden : uint8
 {
	 Visible,
	 Hidden
 };


UCLASS(BlueprintType)
class DIALOGUERUNTIME_API UDialogueSession : public UObject
{
	GENERATED_BODY()

private:
#if WITH_EDITOR
public:
#endif
	UPROPERTY()
	ECustomNodeTypeHidden bHideNodeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue", meta = (Categories = "Dialogue", AllowPrivateAccess = "true"))
	FGameplayTagContainer DialogueTags;

	UPROPERTY()
	TObjectPtr<UDialogueNode_Start> StartNode = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UDialogueNode>> AllNodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Graph", meta = (AllowPrivateAccess = "true"))
	bool bEdgeEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (RequiredAssetDataTags = "RowStructure=/Script/DialogueRuntime.DialogueLocalization", AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> DialoguerNameTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (RequiredAssetDataTags = "RowStructure=/Script/DialogueRuntime.DialogueLocalization", AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> DialogueStringTable;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialogue")
	UDialogueNode_Start* GetStartNode() const;

	FString GetDialoguerName(const UDialogueNode_Basic& InNode) const;
	FString GetDialogueString(const UDialogueNode_Basic& InNode) const;
	

#if WITH_EDITOR
	//UFUNCTION(BlueprintCallable, Category = "Dialogue")
	//virtual void Print(bool ToConsole = true, bool ToScreen = true);
	void ClearGraph();

	FString GetDialoguerName_InEditor(const UDialogueNode_Basic& InNode) const;
	FString GetDialogueString_InEditor(const UDialogueNode_Basic& InNode) const;

	FString GetDialoguerName_Original(const UDialogueNode_Basic& InNode) const;
	FString GetDialogueString_Original(const UDialogueNode_Basic& InNode) const;


#endif


#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<class UEdGraph> EdGraph;

	UPROPERTY(EditDefaultsOnly, Category = "Dialogue_Editor")
	bool bCanRenameNode;

	UPROPERTY(EditDefaultsOnly, Category = "Dialogue_Editor")
	bool bCanBeCyclical;
#endif

public:
	UDialogueSession();
	virtual ~UDialogueSession() {}

public:


#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "DialogueEditor")
	EDialogueLanguage PreviewLanguage;
#endif



#if WITH_EDITOR
	
public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void UpdateDialogueTableByNode(class UDialogueNode_Basic& InNode);

private:
	void OnChaged_PreviewLanguage();



#endif
};
