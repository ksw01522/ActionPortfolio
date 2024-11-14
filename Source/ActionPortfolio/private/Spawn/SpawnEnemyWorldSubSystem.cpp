// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn/SpawnEnemyWorldSubSystem.h"
#include "GameMode/PlayGameMode.h"
#include "GameMode/GameState/PlayGameState.h"
#include "Spawn/SpawnRule.h"
#include "ActionPortfolio.h"
#include "GameMode/GameState/PlayGameState.h"

bool USpawnEnemyWorldSubSystem::ShouldCreateSubsystem(UObject* Outer) const
{

	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}

	FString WorldName = Outer->GetName();
	
	if (WorldName.Contains("Play"))
	{
		return true;
	}


	return false;
}

void USpawnEnemyWorldSubSystem::OnWorldBeginPlay(UWorld& InWorld)
{
	PFLOG(Warning, TEXT("On World Begin Play"));

	APlayGameState* PlayGameState = InWorld.GetGameState<APlayGameState>();
	
	ActableSpawnRules = PlayGameState->GetSpawnRules();

	StartSpawn();
}

void USpawnEnemyWorldSubSystem::Tick(float DeltaTime)
{
	if(!bSpawnStarted) return;

	if (!ActableSpawnRules.IsEmpty())
	{
		float CurrentGameTime = GetWorld()->GetTimeSeconds() - SpawnStartTime;

		if (ActableSpawnRules.Last()->GetSpawnStartTime() <= CurrentGameTime)
		{
			USpawnRule* TempRule = ActableSpawnRules.Pop();

			ActivatedSpawnRules.Add(TempRule);
			TempRule->SpawnEnemy();
		}
	}

	while (!RemoveTargetSpawnRules.IsEmpty())
	{
		USpawnRule* RemoveTarget = RemoveTargetSpawnRules.Pop(false);
		ActivatedSpawnRules.RemoveSingle(RemoveTarget);
	}
}

TStatId USpawnEnemyWorldSubSystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(USpawnEnemyWorldSubSystem, STATGROUP_Tickables);
}

void USpawnEnemyWorldSubSystem::StartSpawn()
{
	SpawnStartTime = GetWorld()->GetTimeSeconds();
	bSpawnStarted = true;
}

FVector USpawnEnemyWorldSubSystem::GetSpawnCenterLocation() const
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();

	FVector ReturnLocation = PlayerController->GetPawn()->GetActorLocation();

	return ReturnLocation;
}

void USpawnEnemyWorldSubSystem::RemoveActivatedSpawnRule(USpawnRule* InRule)
{
	if(InRule == nullptr) return;

	RemoveTargetSpawnRules.Add(InRule);
}
