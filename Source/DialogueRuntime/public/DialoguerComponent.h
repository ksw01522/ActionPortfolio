// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueManager.h"
#include "DialoguerComponent.generated.h"


class UAnimInstance;
class UAnimMontage;

class UDialogueManager;
class IDialoguerManagerInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIALOGUERUNTIME_API UDialoguerComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class UDialogueManager;

public:	
	// Sets default values for this component's properties
	UDialoguerComponent();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialoguer", meta = (AllowPrivateAccess = "true"))
	FString DialoguerID;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;


public:		
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void OnEnteredDialogue(const FDialogueHandle& Handle);

public:

	FString GetDialoguerID() const {return DialoguerID;}
	


//애니메이션
private:
	TWeakObjectPtr<UAnimInstance> DialoguerAnimInstance;

	bool AllocateAnimInstance();
	bool PlayAnimationMontage(UAnimMontage* ToPlayMontage, float PlayLate = 1);

};
