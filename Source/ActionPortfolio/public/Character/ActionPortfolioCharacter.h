// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GenericTeamAgentInterface.h"
#include "Ability/CustomAbilityHelperInterface.h"
#include "ActionPortfolioCharacter.generated.h"


class UInteractionSystemComponent;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharcterDieDelegate, class AActionPortfolioCharacter*, OnDieCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDamagedDelegate, float, DamageAmount, AActor*, DamageInstigator);


UCLASS(config=Game)
class AActionPortfolioCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AActionPortfolioCharacter();

////////////////// Pawn Override //////////////////////////
protected:
	// To add mapping context
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void PostInitializeComponents() override;

	virtual void Landed(const FHitResult& Hit) override;

	virtual USceneComponent* GetDefaultAttachComponent() const override;

#if WITH_EDITOR
protected:
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif // WITH_EDITOR

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	FName CharacterCode;
public:
	UFUNCTION(BlueprintPure)
	FName GetCharacterCode() const { return CharacterCode; }

///////////////// Team //////////////////////////
private:
	FGenericTeamId TeamId;

public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

/////////////////// AbilitySystem ////////////////////
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UActionPFAbilitySystemComponent> AbilitySystem;

	bool bStartupAbilitiesGiven = false;
	bool bStartupEffectsAdded = false;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	int32 GetCharacterLevel() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class UActionPFGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;


protected:
	virtual void AddStartupAbilities();
	void AddStartupEffects();
	virtual void InitializeAttributes();

	virtual void ClearStartupAbilities();
	
	void ClearStartupEffects();

	UFUNCTION()
	virtual bool ConditionRemoveStartupEffects(const FActiveGameplayEffect& ActiveEffect);

///////////////// Attribute /////////////////
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterAttributeSet> AttributeSet;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool CanCharacterMove() const;


	//////////////////////////////// Character State ////////////////////////////////

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death", meta = (AllowPrivateAccess = "true"))
	bool bAutoDestroyOnDie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death", meta = (AllowPrivateAccess = "true"))
	float DestroyTime;

	UFUNCTION()
	void DestroyCharacter();

public:
	UPROPERTY(BlueprintAssignable)
	FOnCharcterDieDelegate OnCharacterDieDelegate;

protected:
	virtual void OnCharacterDie(const FGameplayEffectModCallbackData& Data) {}

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CharacterState")
	bool IsCharacterDie() const;

	void CharacterDie(const FGameplayEffectModCallbackData& Data);

protected:

private:
	void CheckInAir();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	bool IsInAir() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	bool IsDown() const;


///////////////// Movement //////////////////////////
private:
	bool bDefaultOrientRotationToMovement;
	float DefaultWalkSpeed;
	float DefaultMaxAcceleration;
	float DefaultGravityScale;

protected:
	void InitializeMovement();

public:
	void ResetMovement();

public:
	virtual bool CanBasicAct() const;

	class UActionPFAnimInstance* GetAnimInstance() const; 
};

