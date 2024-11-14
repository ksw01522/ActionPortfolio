// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DialogueWorldSubsystem.h"
#include "DialogueRuntime.h"
#include "Engine/SceneCapture2D.h"
#include "DialogueDeveloperSettings.h"
#include "Animation/DialogueAnimationActor.h"

void UDialogueWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	const UDialogueDeveloperSettings* DDS = GetDefault<UDialogueDeveloperSettings>();

	const FVector CameraSpawnLocation = FVector(0, 0, 1000000);

	SceneCapture = GetWorld()->SpawnActor<ASceneCapture2D>(DDS->GetDialogueSceneCaptureClass(), CameraSpawnLocation, FRotator::ZeroRotator);
}

void UDialogueWorldSubsystem::RegisterAnimActor(ADialogueAnimationActor* InActor, FName InCode)
{
	check(InActor);
	check(!AnimActorMap.Contains(InCode));
	AnimActorMap.Add(InCode, InActor);
}

void UDialogueWorldSubsystem::UnregisterAnimActor(FName InCode)
{
	check(AnimActorMap.Contains(InCode));
	if (AnimActorMap[InCode].IsValid())
	{
		AnimActorMap[InCode]->Destroy();
	}
	AnimActorMap.Remove(InCode);
}

void UDialogueWorldSubsystem::ClearAnimationActor()
{
	for (auto& AnimActor : AnimActorMap)
	{
		if(AnimActor.Value.IsValid()) AnimActor.Value->Destroy();
	}

	AnimActorMap.Empty();
}
