// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Events/DialogueEvent.h"
#include "DialogueEvent_PlaySound.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EPlaySoundType : uint8
{
	FromActor,
	Anywhere
};

UCLASS()
class DIALOGUERUNTIME_API UDialogueEvent_PlaySound : public UDialogueEvent
{
	GENERATED_BODY()
	
public:
	UDialogueEvent_PlaySound();

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "DialogueSound", meta = (EditCondition = "PlaySoundType == EPlaySoundType::FromActor", EditConditonHides, AllowPrivateAccess = "true"))
	FString LocationDialoguerID;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "DialogueSound", meta = (AllowPrivateAccess = "true"))
	EPlaySoundType PlaySoundType;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "DialogueSound", meta = (AllowPrivateAccess = "true"))
	USoundBase* SoundSource;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "DialogueSound", meta = (AllowPrivateAccess = "true"))
	bool bWantStopWhenEventEnd = true;

	TWeakObjectPtr<UAudioComponent> InstanceAudio;

private:

	UFUNCTION(Client, unreliable)
	void Client_PlaySound2D();

	void Client_PlaySound2D_Implementation();

	UFUNCTION(Client, unreliable)
	void Client_PlaySoundFromActor();
	void Client_PlaySoundFromActor_Implementation();

	virtual void OnEndEvent_Implementation(bool bIsCancelled) override;
	virtual void OnCalledEvent_Implementation() override;

};
