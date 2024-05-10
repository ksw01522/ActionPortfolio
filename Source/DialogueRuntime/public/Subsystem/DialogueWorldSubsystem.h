// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUERUNTIME_API UDialogueWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	class ASceneCapture2D* SceneCapture;

protected:
	//virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	//virtual void PostInitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

private:
	TMap<FName, TWeakObjectPtr<class ADialogueAnimationActor>> AnimActorMap;

public:
	void RegisterAnimActor(ADialogueAnimationActor* InActor, FName InCode);
	void UnregisterAnimActor(FName InCode);

	void ClearAnimationActor();
};
