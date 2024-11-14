// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "CharacterDataManager.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterAttribute : public FTableRowBase
{
	GENERATED_BODY();

	FCharacterAttribute() : AttributeType(), MaxHealth(0), HealthRegen(0), MaxStamina(0), StaminaRegen(0), AttackP(0), DefenseP(0),
		FireResistance(0), IceResistance(0), ElectricResistance(0)
	{}

	FCharacterAttribute(FName InType) : AttributeType(InType), MaxHealth(0), HealthRegen(0), MaxStamina(0), StaminaRegen(0), AttackP(0), DefenseP(0),
		FireResistance(0), IceResistance(0), ElectricResistance(0)
	{}

	FName AttributeType;

	UPROPERTY(EditAnywhere)
	float Level;

	UPROPERTY(EditAnywhere)
	float MaxHealth;

	UPROPERTY(EditAnywhere)
	float HealthRegen;

	UPROPERTY(EditAnywhere)
	float MaxStamina;

	UPROPERTY(EditAnywhere)
	float StaminaRegen;

	UPROPERTY(EditAnywhere)
	float AttackP;

	UPROPERTY(EditAnywhere)
	float DefenseP;

	UPROPERTY(EditAnywhere)
	float FireResistance;

	UPROPERTY(EditAnywhere)
	float IceResistance;

	UPROPERTY(EditAnywhere)
	float ElectricResistance;
};

USTRUCT()
struct FEnemyAttribute : public FCharacterAttribute
{
	GENERATED_BODY();

	FEnemyAttribute() : FCharacterAttribute(FName("Enemy")), BountyXP(0), BountyGold(0)
	{}

	UPROPERTY(EditAnywhere)
	float BountyXP;

	UPROPERTY(EditAnywhere)
	float BountyGold;
};


UCLASS()
class ACTIONPORTFOLIO_API UCharacterDataManager : public  UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	static UCharacterDataManager* SingltonInstance;
public:
	const static UCharacterDataManager* GetCharacterDataManager();

private:
	UPROPERTY()
	TArray<TObjectPtr<UDataTable>> CharacterDataTables;

	TMap<FName, const FCharacterAttribute*> CharacterDataArray;

	UPROPERTY()
	TObjectPtr<class UCurveTable> PlayerDataCurveTable = nullptr;

protected:
	//virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

private:
	void InitializeDataTables();
	void InitializeDataArray();

public:
	const FCharacterAttribute* GetCharacterData(FName InCharacterCode) const;

	const UCurveTable* GetPlayerDataTable() const { return PlayerDataCurveTable; }
};
