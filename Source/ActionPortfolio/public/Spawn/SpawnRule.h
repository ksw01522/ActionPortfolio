// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpawnRule.generated.h"

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable, BlueprintType, EditInlineNew)
class ACTIONPORTFOLIO_API USpawnRule : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ACharacterEnemy> Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	int Count = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn", BlueprintGetter = GetSpawnStartTime, meta = (AllowPrivateAccess = "true"))
	float StartTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Repeat", meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	int RepeatCount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Repeat", meta = (AllowPrivateAccess = "true", EditConditionHides, EditCondition = "1<RepeatCount"))
	float Interval;

	int CurrentRepeatCount = 0;

protected:
	UFUNCTION()
	virtual void OnSpawnEnemy() {};

public:
	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION(BlueprintGetter)
	float GetSpawnStartTime() const {return StartTime;}

	TSubclassOf<class ACharacterEnemy> GetEnemyClass() const { return Class; }
	int GetSpawnCount() const {return Count;}
};

UCLASS(Blueprintable, BlueprintType, EditInlineNew, meta = (DisplayName = "Random In Donut"))
class ACTIONPORTFOLIO_API USpawnRule_RandomInDonut : public USpawnRule
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, Category = "Range", meta = (AllowPrivateAccess = "true", CampMin = 0))
	float InnerCircleRadius;

	UPROPERTY(EditAnywhere, Category = "Range", meta = (AllowPrivateAccess = "true", CampMin = 0))
	float OuterCircleRadius;

protected:

	virtual void OnSpawnEnemy() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

};
