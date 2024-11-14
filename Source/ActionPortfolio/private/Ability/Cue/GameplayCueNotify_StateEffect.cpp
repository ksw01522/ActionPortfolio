// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Cue/GameplayCueNotify_StateEffect.h"
#include "NiagaraComponent.h"
#include "ActionPortfolio.h"
#include "Engine/NetDriver.h"
#include "AbilitySystemInterface.h"
#include "Ability/ActionPFAbilitySystemComponent.h"

AGameplayCueNotify_StateEffect::AGameplayCueNotify_StateEffect()
{
	bAutoDestroyOnRemove = false;

	EffectNiagara = CreateDefaultSubobject<UNiagaraComponent>("EffectNiagara");

	SetRootComponent(EffectNiagara);

	ScaleOverride = FVector(1);

	bReplicates = false;

	EffectNiagara->SetAutoActivate(true);

	bAutoAttachToOwner = false;	

	bAllowMultipleOnActiveEvents = false;
	bAllowMultipleWhileActiveEvents = false;
	bAllowReceiveTickEventOnDedicatedServer = false;
	bAllowTickBeforeBeginPlay = false;
}

#if WITH_EDITOR
EDataValidationResult AGameplayCueNotify_StateEffect::IsDataValid(TArray<FText>& ValidationErrors)
{
	

	return ((ValidationErrors.Num() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}

#endif

bool AGameplayCueNotify_StateEffect::Recycle()
{
	Super::Recycle();

#if WITH_EDITOR
	UWorld* World = GetWorld();

	bool bIsClient = World->IsNetMode(ENetMode::NM_Client);

	FString TempString = bIsClient ? "Client" : "Server";

	PFLOG(Warning, TEXT("%s : Recycle"), *TempString);
#endif

	return false;
}
void AGameplayCueNotify_StateEffect::ReuseAfterRecycle()
{
#if WITH_EDITOR
	UWorld* World = GetWorld();

	bool bIsClient = World->IsNetMode(ENetMode::NM_Client);

	FString TempString = bIsClient ? "Client" : "Server";

	PFLOG(Warning, TEXT("%s : Reuse After Recycle"), *TempString);
#endif

	Super::ReuseAfterRecycle();

}
bool AGameplayCueNotify_StateEffect::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
{
#if WITH_EDITOR
	UWorld* World = GetWorld();

	bool bIsClient = World->IsNetMode(ENetMode::NM_Client);

	FString TempString = bIsClient ? "Client" : "Server";

	PFLOG(Warning, TEXT("%s : On Execute"), *TempString);
#endif

	return false;
}

bool AGameplayCueNotify_StateEffect::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
#if WITH_EDITOR
	UWorld* World = GetWorld();

	bool bIsClient = World->IsNetMode(ENetMode::NM_Client);

	FString TempString = bIsClient ? "Client" : "Server";

	PFLOG(Warning, TEXT("%s : On Active"), *TempString);
#endif

	return false;
}

bool AGameplayCueNotify_StateEffect::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	UWorld* World = GetWorld();

#if WITH_EDITOR

	bool bIsClient = World->IsNetMode(ENetMode::NM_Client);

	FString TempString = bIsClient ? "Client" : "Server";

	PFLOG(Warning, TEXT("%s : WhileActive"), *TempString);
#endif

	ENetMode NetMode = World->GetNetMode();
	if (NetMode == ENetMode::NM_DedicatedServer)
	{
		Destroy();
		return false;
	}

	if (IAbilitySystemInterface* IAS = Cast<IAbilitySystemInterface>(MyTarget))
	{
		bAutoDestroyOnRemove = false;
	}
	else
	{
		bAutoDestroyOnRemove = true;
	}


	if (RootComponent->GetAttachmentRootActor() != MyTarget)
	{
		FAttachmentTransformRules ATR(EAttachmentRule::KeepWorld, false);

		AttachToActor(MyTarget, ATR, SocketName);

		FVector NewLocation = bOverrideLocation ? LocationOverride : FVector::ZeroVector;
		FRotator NewRotation = bOverrideRotation ? RotationOverride : FRotator::ZeroRotator;
		FVector NewScale = bOverrideScale ? ScaleOverride : FVector::OneVector;
		FTransform NewTransform(NewRotation, NewLocation, NewScale);

		SetActorRelativeTransform(NewTransform);

		EffectNiagara->SetActive(true);
	}
	
	return false;
}

bool AGameplayCueNotify_StateEffect::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
#if WITH_EDITOR
	UWorld* World = GetWorld();

	bool bIsClient = World->IsNetMode(ENetMode::NM_Client);

	FString TempString = bIsClient ? "Client" : "Server";

	PFLOG(Warning, TEXT("%s : On Remove"), *TempString);
#endif

	if (MyTarget == nullptr && !bAutoDestroyOnRemove)
	{
		GameplayCueFinishedCallback();
		return false;
	}

	if (IAbilitySystemInterface* IAS = Cast<IAbilitySystemInterface>(MyTarget))
	{
		UAbilitySystemComponent* ASC = IAS->GetAbilitySystemComponent();

	 	int CueTagCount = ASC->GetGameplayTagCount(GameplayCueTag);
		if (CueTagCount <= 0)
		{
			GameplayCueFinishedCallback();
		}
		else
		{
			bHasHandledOnRemoveEvent = false;
		}
	}
	else
	{
		GameplayCueFinishedCallback();
	}

	return false;
}

