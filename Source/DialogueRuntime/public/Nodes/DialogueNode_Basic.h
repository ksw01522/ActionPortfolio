// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNode.h"
#include "DialogueNode_Basic.generated.h"

/**
 * 
 */

 class UAnimMontage;



UCLASS()
class DIALOGUERUNTIME_API UDialogueNode_Basic : public UDialogueNode
{
	GENERATED_BODY()

#if WITH_EDITOR
	DECLARE_DYNAMIC_DELEGATE(FOnChangedDialogueStyle);
#endif


private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	FName DialoguerNameCode;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	FName DialogueStringCode;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	TObjectPtr<class UDataTable> TargetTable;

	UPROPERTY(VisibleDefaultsOnly, Category = "DialogueEditor", AdvancedDisplay, meta = (AllowPrivateAccess = "true"))
	bool CanVisible_DialoguerName = false;

	UPROPERTY(Transient, VisibleDefaultsOnly, Category = "DialogueEditor", AdvancedDisplay, meta = (AllowPrivateAccess = "true"))
	bool CanVisible_DialogueString = false;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "DialogueEditor", BlueprintGetter = GetDialoguerName_Original_InEditor, meta = (DisplayAfter = "DialoguerNameCode", EditCondition = "CanVisible_DialoguerName", EditConditionHides, AllowPrivateAccess = "true"))
	FString Original_DialoguerName;

	UPROPERTY(Transient, VisibleInstanceOnly,  Category = "DialogueEditor", BlueprintGetter = GetDialoguerName_InEditor, meta = (DisplayAfter = "DialoguerNameCode", EditCondition = "CanVisible_DialoguerName", EditConditionHides, AllowPrivateAccess = "true"))
	FString DialoguerName;

	UPROPERTY(Transient, EditInstanceOnly, Category = "DialogueEditor", BlueprintGetter = GetDialogueString_Original_InEditor, meta = (DisplayAfter = "DialogueStringCode", EditCondition = "CanVisible_DialogueString", EditConditionHides, AllowPrivateAccess = "true"))
	FString Original_DialogueString;

	UPROPERTY(Transient, EditInstanceOnly, Category = "DialogueEditor", BlueprintGetter = GetDialogueString_InEditor, meta = (DisplayAfter = "DialogueStringCode", EditCondition = "CanVisible_DialogueString", EditConditionHides, AllowPrivateAccess = "true"))
	FString DialogueString;

	UPROPERTY(EditInstanceOnly, Category = "DialogueEditor", meta = (AllowPrivateAccess = "true"))
	bool bSlateDeco;

#endif

	UPROPERTY(EditAnywhere, Category = "DialogueTextStyle")
	TArray<TSubclassOf<class URichTextBlockDecorator>> DialogueUMGDecoratorClasses;

	UPROPERTY(EditAnywhere, Category = "DialogueTextStyle")
	TArray<TSubclassOf<class USRichTextBlockDecorator>> DialogueSlateDecoratorClasses;

public:
	UDialogueNode_Basic();

	const TArray<TSubclassOf<class URichTextBlockDecorator>>& GetUMGDecoClasses() const;
	const TArray<TSubclassOf<class USRichTextBlockDecorator>>& GetSlateDecoClasses() const;

	FString GetDialoguerName() const;
	FString GetDialogueString() const;

#if WITH_EDITOR
public:
	UFUNCTION(BlueprintCallable, BlueprintGetter, Category = "Dialogue")
	FString GetDialoguerName_InEditor() const;

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category = "Dialogue")
	FString GetDialogueString_InEditor() const;

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category = "Dialogue")
	FString GetDialoguerName_Original_InEditor() const;

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category = "Dialogue")
	FString GetDialogueString_Original_InEditor() const;
	
	bool IsSlateRichTextStyle() const { return bSlateDeco; }

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION()
	virtual void ImportDialogue();

	virtual void RebuildNode() override;

private:
	virtual void OnChanged_DialoguerNameCode();
	virtual void OnChanged_DialogueStringCode();

	virtual void OnChanged_PreviewLanguage() override;

public:
	FName GetDialoguerNameCode() const {return DialoguerNameCode;}
	FName GetDialogueStringCode() const {return DialogueStringCode;}

#endif
	
protected:
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<FAnimInDialogueStruct> DialoguerAnimations;


};
