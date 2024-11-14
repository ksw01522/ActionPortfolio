// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueNotify_StateIcon.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONPORTFOLIO_API UGameplayCueNotify_StateIcon : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UGameplayCueNotify_StateIcon();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Icon", meta = (AllowPrivateAccess = "true"))
	FSlateBrush IconBrush;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Icon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UStateIconWidget> WidgetClass;

protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

	virtual bool WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;


};
