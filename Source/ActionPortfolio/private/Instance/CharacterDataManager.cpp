// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/CharacterDataManager.h"
#include "Engine/CurveTable.h"
#include "ActionPortfolio.h"

UCharacterDataManager* UCharacterDataManager::SingltonInstance = nullptr;

void UCharacterDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
    SingltonInstance = this;

    InitializeDataTables();
    InitializeDataArray();
}

void UCharacterDataManager::Deinitialize()
{
    SingltonInstance = nullptr;
}

void UCharacterDataManager::InitializeDataTables()
{  
    CharacterDataTables.Add(Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), this, TEXT("/Game/ActionPFCharacter/DataTable/CharacterData_Enemy.CharacterData_Enemy"))));

    PlayerDataCurveTable = Cast<UCurveTable>(StaticLoadObject(UCurveTable::StaticClass(), this, TEXT("/Game/ActionPFCharacter/DataTable/PlayerCharacterData.PlayerCharacterData")));
    
#if WITH_EDITOR
    if(PlayerDataCurveTable == nullptr)
    {
        FString ResourceFolderPath = FPaths::ProjectDir() + TEXT("Resources/Character/DataTable/");

        PlayerDataCurveTable = NewObject<UCurveTable>(this, "PlayerDataCurveTable");

        FString DataCSVString;
        FString DataCSVPath = ResourceFolderPath + TEXT("PlayerCharacterData.csv");

        if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*DataCSVPath))
        {
            FFileHelper::LoadFileToString(DataCSVString, *DataCSVPath);

            PlayerDataCurveTable->CreateTableFromCSVString(DataCSVString);
        }
        else
        {
            PFLOG(Error, TEXT("Can't find Player Curve Data CSV : %s"), *DataCSVPath);
        }
    }
#endif
}

void UCharacterDataManager::InitializeDataArray()
{  
    for (auto DataTable : CharacterDataTables)
    {
        const TMap<FName, uint8*> TempMap = DataTable->GetRowMap();
        for (auto& Element : TempMap)
        {
            CharacterDataArray.Add(Element.Key, reinterpret_cast<FCharacterAttribute*>(Element.Value));
        }
    }
}

const FCharacterAttribute* UCharacterDataManager::GetCharacterData(FName InCharacterCode) const
{
    const FCharacterAttribute* ReturnData = CharacterDataArray.FindRef(InCharacterCode);

    checkf(ReturnData, TEXT("Can't find Character Data : %s"), *InCharacterCode.ToString());

    return ReturnData;
}

const UCharacterDataManager* UCharacterDataManager::GetCharacterDataManager()
{
    return SingltonInstance;
}
