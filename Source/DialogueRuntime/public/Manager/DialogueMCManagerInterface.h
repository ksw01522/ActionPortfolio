// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialogueMCManagerInterface.generated.h"

class UDialogueMCComponent;
class UDialogueSession;
class UDialoguerComponent;

enum class EDialogueNodeType : uint8;
struct FDialogueElement;
struct FDialogueHandle;
struct FDialogueElementContainer;
struct FNextDialogueNodeOptionalStruct;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDialogueMCManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIALOGUERUNTIME_API IDialogueMCManagerInterface
{
	GENERATED_BODY()

	friend UDialogueMCComponent;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
private:
	virtual void EnterDialogue(FDialogueHandle& Handle, const UDialogueSession* Session) = 0;
	virtual bool ExitDialogue(FDialogueHandle& Handle) = 0;
	virtual bool EnterNextNode(FDialogueElementContainer& OutElements, FDialogueHandle& Handle, FNextDialogueNodeOptionalStruct* OptionalStruct) = 0;
	virtual bool CanEnterNextNode(FDialogueHandle& Handle) const = 0;
	virtual bool IsActingDialogue(FDialogueHandle& Handle) const = 0;
};
