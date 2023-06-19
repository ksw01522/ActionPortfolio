// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionPortfolioGameMode.h"
#include "Character/Player/PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Character/Player/ActionPFPlayerController.h"

AActionPortfolioGameMode::AActionPortfolioGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ActionPFCharacter/Player/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> BPPlayerControllerClass(TEXT("/Game/ActionPFCharacter/Player/Blueprints/BP_PlayerController.BP_PlayerController_C"));
	if (BPPlayerControllerClass.Succeeded())
	{
		PlayerControllerClass = BPPlayerControllerClass.Class;
	}

}
