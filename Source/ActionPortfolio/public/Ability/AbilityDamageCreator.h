// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ScalableFloat.h"
#include "GameplayTagContainer.h"
#include "AbilityDamageCreator.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew, Blueprintable, HideCategories = (Hidden))
class ACTIONPORTFOLIO_API UAbilityDamageCreator : public UObject
{
	GENERATED_BODY()

public:
	UAbilityDamageCreator();

protected:
	UPROPERTY(EditAnywhere, meta = (Categories = "DamageType"))
	FGameplayTagContainer DamageTypeTagContainer;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", EditConditionHides, EditCondition = "bCanVisibleMeelee"))
	FScalableFloat MeeleeDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", EditConditionHides, EditCondition = "bCanVisibleFire"))
	FScalableFloat FireDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", EditConditionHides, EditCondition = "bCanVisibleIce"))
	FScalableFloat IceDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", EditConditionHides, EditCondition = "bCanVisibleElectric"))
	FScalableFloat ElectricDamage;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = Hidden,VisibleAnywhere)
	bool bCanVisibleMeelee;

	UPROPERTY(Category = Hidden, VisibleAnywhere)
	bool bCanVisibleFire;

	UPROPERTY(Category = Hidden, VisibleAnywhere)
	bool bCanVisibleIce;

	UPROPERTY(Category = Hidden, VisibleAnywhere)
	bool bCanVisibleElectric;
#endif

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	UGameplayEffect* CreateDamageEffect(class UAbilitySystemComponent* SourceASC = nullptr);
};

