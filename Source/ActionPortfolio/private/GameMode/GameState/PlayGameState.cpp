// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameState/PlayGameState.h"
#include "ActionPortfolio.h"
#include "Character/Enemy/CharacterEnemy.h"

FDelegateHandle APlayGameState::AddCharacterDieEvent(const TDelegate<void(AActionPortfolioCharacter*)>& InDel)
{
    if (InDel.IsBound())
    {
        return OnCharacterDieDelegate.Add(InDel);
    }

    return FDelegateHandle();
}

bool APlayGameState::RemoveCharacterDieEvent(FDelegateHandle& InHandle)
{
    if (OnCharacterDieDelegate.Remove(InHandle))
    {
        InHandle.Reset();
        return true;
    }
    else
    {
        return false;
    }
}

void APlayGameState::OnCharacterDie(AActionPortfolioCharacter* InChar)
{
    check(InChar != nullptr);
#if WITH_EDITOR
    PFLOG(Warning, TEXT(""))
#endif
    OnCharacterDieDelegate.Broadcast(InChar);

    if (ACharacterEnemy* Enemy = Cast<ACharacterEnemy>(InChar))
    {
        
    }
}
