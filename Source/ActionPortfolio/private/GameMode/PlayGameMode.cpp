// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PlayGameMode.h"
#include "GameMode/GameState/PlayGameState.h"

APlayGameMode::APlayGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ActionPFCharacter/Player/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> BPPlayerControllerClass(TEXT("/Game/ActionPFCharacter/Player/Blueprints/BP_PlayerController.BP_PlayerController_C"));
	if (BPPlayerControllerClass.Succeeded())
	{
		PlayerControllerClass = BPPlayerControllerClass.Class;
	}

	GameStateClass = APlayGameState::StaticClass();
}
