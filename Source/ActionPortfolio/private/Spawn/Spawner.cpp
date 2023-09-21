// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn/Spawner.h"
#include "Character/ActionPortfolioCharacter.h"
#include "ActionPortfolio.h"

ASpawner::ASpawner()
{
	SpawnNodeStructs.Empty();

	CurrentIDX = -1;
	CurrentSpawnedCount = 0;
	bAutoSpawnOnAllDestroy = true;
	bDestroyAfterSpawn = true;
}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();
}

void ASpawner::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CurrentIDX = -1;
	CurrentSpawnedCount = 0;
}

void ASpawner::SpawnStart()
{
	if (SpawnStartEvent.IsBound()) SpawnStartEvent.Broadcast();
	SpawnCharacters();
}

bool ASpawner::IsSpawning() const
{
	return 0 <= CurrentIDX && CurrentIDX < SpawnNodeStructs.Num();
}

void ASpawner::SpawnCharacters()
{
	if (SpawnNodeStructs.Num() <= ++CurrentIDX)
	{
		OnSpawnEnd();
		return;
	}

	TArray<AActionPortfolioCharacter*> SpawnedCharacters;
	for (const FSpawnDataStruct& SpawnNode : SpawnNodeStructs[CurrentIDX].SpawnDataArray)
	{
		AActionPortfolioCharacter* SpawnChar = GetWorld()->SpawnActor<AActionPortfolioCharacter>(SpawnNode.SpawnTargetClass, SpawnNode.SpawnTransform);
		if (IsValid(SpawnChar))
		{
			SpawnedCharacters.Add(SpawnChar);
			SpawnChar->OnCharacterDie.AddDynamic(this, &ASpawner::OnSpawnedCharacterDie);
			CurrentSpawnedCount++;
		}
	}

	if(OnSpawningEvent.IsBound()) OnSpawningEvent.Broadcast(CurrentIDX, SpawnedCharacters);
}

void ASpawner::OnSpawnedCharacterDie(AActionPortfolioCharacter* SpawnedChar)
{
	CurrentSpawnedCount--;

#if WITH_EDITOR
	PFLOG(Warning, TEXT("Current Spawned Character Count : %d"), CurrentSpawnedCount);
#endif


	if(OnSpawnedCharacterDieEvent.IsBound()) OnSpawnedCharacterDieEvent.Broadcast(SpawnedChar);

	if (bAutoSpawnOnAllDestroy && CurrentSpawnedCount <= 0)
	{
		SpawnCharacters();
	}
}

void ASpawner::OnSpawnEnd()
{
	if(SpawnEndEvent.IsBound()) SpawnEndEvent.Broadcast();
	if (bDestroyAfterSpawn)
	{
		Destroy();
	}
	else {
		CurrentIDX = -1;
	}
}
