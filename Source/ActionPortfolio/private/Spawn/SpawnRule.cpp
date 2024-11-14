// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn/SpawnRule.h"
#include "ActionPortfolio.h"
#include "Spawn/SpawnEnemyWorldSubSystem.h"
#include "Character/Enemy/CharacterEnemy.h"


void USpawnRule::SpawnEnemy()
{
	CurrentRepeatCount++;
	OnSpawnEnemy();

	if (CurrentRepeatCount < RepeatCount)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USpawnRule::SpawnEnemy, Interval, false);
	}
	else
	{
		USpawnEnemyWorldSubSystem* SES = GetWorld()->GetSubsystem<USpawnEnemyWorldSubSystem>();
		SES->RemoveActivatedSpawnRule(this);
	}
}

void USpawnRule_RandomInDonut::OnSpawnEnemy()
{
	USpawnEnemyWorldSubSystem* SES = GetWorld()->GetSubsystem<USpawnEnemyWorldSubSystem>();

	const FVector SpawnCenter = SES->GetSpawnCenterLocation();

	const int TempSpawnCount = GetSpawnCount();

	TSubclassOf<ACharacterEnemy> TempEnemyClass = GetEnemyClass();

	for (int i = 0; i < TempSpawnCount; i++)
	{
		const float SpawnRadius = FMath::RandRange(InnerCircleRadius, OuterCircleRadius);

		const int SpawnDegree = FMath::RandRange(0, 360);
		
		FVector SpawnLocation(SpawnRadius,0,0);
		SpawnLocation = SpawnLocation.RotateAngleAxis(SpawnDegree, FVector::ZAxisVector) + SpawnCenter;

		GetWorld()->SpawnActor<ACharacterEnemy>(TempEnemyClass, SpawnLocation, FRotator::ZeroRotator);
	}
}

#if WITH_EDITOR
void USpawnRule_RandomInDonut::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(USpawnRule_RandomInDonut, InnerCircleRadius) || PropertyName == GET_MEMBER_NAME_CHECKED(USpawnRule_RandomInDonut, OuterCircleRadius))
	{
		if (OuterCircleRadius < InnerCircleRadius) {
			OuterCircleRadius = InnerCircleRadius;
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);

}
#endif