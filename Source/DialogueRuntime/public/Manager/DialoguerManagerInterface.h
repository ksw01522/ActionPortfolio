// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialoguerManagerInterface.generated.h"

class UDialogueMCComponent;
class UDialogueSession;
class UDialoguerComponent;

enum class EDialogueNodeType : uint8;
struct FDialogueElement;
struct FDialogueHandle;
struct FDialogueElementContainer;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDialoguerManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIALOGUERUNTIME_API IDialoguerManagerInterface
{
	GENERATED_BODY()

	friend UDialoguerComponent;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
private:
	virtual void RegisterDialoguer(UDialoguerComponent* NewDialoguer) = 0;
	virtual void UnregisterDialoguer(UDialoguerComponent* TargetDialoguer) = 0;
};
