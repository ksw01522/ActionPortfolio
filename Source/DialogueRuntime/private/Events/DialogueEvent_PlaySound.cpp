// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/DialogueEvent_PlaySound.h"
#include "Kismet/GameplayStatics.h"
#include "DialogueRuntime.h"
#include "DialoguerComponent.h"
#include "Components/AudioComponent.h"
#include "DialogueBFL.h"


UDialogueEvent_PlaySound::UDialogueEvent_PlaySound()
{
	InstancingPolicy = EDialougeEventInstancingPolicy::InstancedPerExecution;
}

void UDialogueEvent_PlaySound::Client_PlaySound2D_Implementation()
{
	UAudioComponent* NewAudio = UGameplayStatics::SpawnSound2D(GetWorld(), SoundSource);
	InstanceAudio = NewAudio;
}

void UDialogueEvent_PlaySound::Client_PlaySoundFromActor_Implementation()
{
	UDialoguerComponent* Dialoguer = UDialogueBFL::GetDialoguer(LocationDialoguerID);
	UAudioComponent* NewAudio = UGameplayStatics::SpawnSoundAttached(SoundSource, Dialoguer->GetOwner()->GetRootComponent(), NAME_None, FVector(0, 0, 0), EAttachLocation::KeepRelativeOffset, true);
	InstanceAudio = NewAudio;
}

void UDialogueEvent_PlaySound::OnEndEvent_Implementation(bool bIsCancelled)
{
	if(!bWantStopWhenEventEnd || !InstanceAudio.IsValid()) return;

	InstanceAudio->Stop();
	InstanceAudio->DestroyComponent();
	MarkAsGarbage();
}

void UDialogueEvent_PlaySound::OnCalledEvent_Implementation()
{
	ensure(SoundSource);

	if (PlaySoundType == EPlaySoundType::FromActor) { Client_PlaySoundFromActor(); }
	else if (PlaySoundType == EPlaySoundType::Anywhere) { Client_PlaySound2D(); }

}
