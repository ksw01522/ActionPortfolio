// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Engine/CompositeDataTable.h"
//#include "DialogueStructs.generated.h"


struct FDialogueHandle
{
	friend class UDialogueManager;

	FDialogueHandle() {}

private:
	FDialogueHandle(const uint32 ID) : ID(ID) {}

private:
	void Invalidate() { ID = INDEX_NONE; }
	void SetHandleID(uint32 NewID) { ID = NewID; }

	FDialogueHandle& operator=(const FDialogueHandle& Other){ID = Other.ID;}

public:
	bool IsValid() const { return ID != INDEX_NONE; }

	bool operator==(const FDialogueHandle& Other) const { return ID == Other.ID; }
	bool operator!=(const FDialogueHandle& Other) const { return ID != Other.ID; }

	bool operator==(const uint32& Other) const { return ID == Other; }
	bool operator!=(const uint32& Other) const { return ID != Other; }

	friend uint32 GetTypeHash(const FDialogueHandle& InHandle)
	{
		return InHandle.ID;
	}

private:
	uint32 ID = INDEX_NONE;
};







