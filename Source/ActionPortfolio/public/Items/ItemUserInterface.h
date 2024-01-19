// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

//Ability연동을 위한 헤더
#include "GameplayAbilitySpecHandle.h"
#include "ActiveGameplayEffectHandle.h"

#include "ItemUserInterface.generated.h"


class UItemBase_Equipment;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemUserInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONPORTFOLIO_API IItemUserInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
private:
	virtual void InitializeItemUser() = 0;

	virtual void OnEquipItem(UItemBase_Equipment* NewItem){};
	virtual void OnUnequipItem(UItemBase_Equipment* NewItem){};
public:

	virtual class UActionPFAbilitySystemComponent* GetASCForItemUser() const = 0;


//EquipmentItem
	virtual bool CanEquipItem(UItemBase_Equipment* NewItem) const;
	virtual bool EquipItem(UItemBase_Equipment* NewItem);
	
	virtual bool CanUnequipItem(UItemBase_Equipment* NewItem) const;
	virtual bool UnequipItem(UItemBase_Equipment* NewItem);

#if WITH_EDITOR
	virtual FName GetUserName() const = 0;
#endif
};
