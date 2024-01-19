// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_HPBar_Basic.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UWidget_HPBar_Basic : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HPBar", meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UProgressBar> HPBar;

protected:
	TWeakObjectPtr<class AActionPortfolioCharacter> RegistedCharacter;

private:
	UFUNCTION()
	float GetHPBarPercent();

public:
	UFUNCTION(BlueprintCallable, Category = "HPBar")
	virtual void RegistCharacter(AActionPortfolioCharacter* NewCharacter);
};
