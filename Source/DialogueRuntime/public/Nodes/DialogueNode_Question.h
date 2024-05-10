// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNode_Basic.h"
#include "DialogueNode_Answer.h"
#include "DialogueNode_Question.generated.h"

/**
 * 
 */
struct FNextDialogueNodeOptionalStruct_Question : public FNextDialogueNodeOptionalStruct
{
	FNextDialogueNodeOptionalStruct_Question() : AnswerIndex(-1)
	{}

	FNextDialogueNodeOptionalStruct_Question(int Idx) : AnswerIndex(Idx)
	{}

	~FNextDialogueNodeOptionalStruct_Question(){}

public:
	int AnswerIndex;
};

UCLASS()
class DIALOGUERUNTIME_API UDialogueNode_Question : public UDialogueNode_Basic
{
	GENERATED_BODY()
public:
	UDialogueNode_Question();

protected:
#if WITH_EDITOR
	//virtual bool CanCreateConnection(UDialogueNode* Other, FText& ErrorMessage) override;

	virtual bool CanCreateConnectionTo(UDialogueNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage) const override;

	virtual FText GetNodeTitle() override;
	#endif
};
