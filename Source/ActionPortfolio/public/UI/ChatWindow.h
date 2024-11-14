// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWindow.generated.h"

/**
 * 
 */
UENUM()
enum class EChatCategory : uint8
{
	None UMETA(Hidden),
	Common,
	System
	
};

ENUM_RANGE_BY_FIRST_AND_LAST(EChatCategory, EChatCategory::Common, EChatCategory::System);


UCLASS()
class ACTIONPORTFOLIO_API UChatNode : public UUserWidget
{
	GENERATED_BODY()


	
};

UCLASS()
class ACTIONPORTFOLIO_API UChatWindow : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> ChatBox;

protected:
	virtual void NativeConstruct() override;


public:
	
};