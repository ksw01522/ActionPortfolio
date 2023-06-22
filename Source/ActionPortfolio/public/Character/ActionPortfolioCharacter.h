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
#include "ActionPortfolioCharacter.generated.h"


class UInteractionSystemComponent;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharcterDieDelegate);

UENUM(BlueprintType)
enum class EHitReactionDirection : uint8
{
	Front = 0,
	Back = 1,
	Right = 2,
	Left = 3
};

USTRUCT(BlueprintType)
struct FHitReactionAnimations
{
	GENERATED_BODY()
public:
	FHitReactionAnimations() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ground")
	UAnimMontage* Ground_Front;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ground")
	UAnimMontage* Ground_Back;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ground")
	UAnimMontage* Ground_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ground")
	UAnimMontage* Ground_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InAir")
	UAnimMontage* InAir_Front;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InAir")
	UAnimMontage* InAir_Back;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InAir")
	UAnimMontage* InAir_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InAir")
	UAnimMontage* InAir_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Down")
	UAnimMontage* Down;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Down")
	UAnimMontage* DownedRecovery;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
	UAnimMontage* Death;
};

UCLASS(config=Game)
class AActionPortfolioCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	class UActionPFAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	class UActionPFAttributeSet* AttributeSet;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class UActionPFGameplayAbility>> CharacterAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	UPROPERTY(Transient)
	TMap<FName, class UShapeComponent*> AttackShapeMap;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TArray<FName> AttackShapeTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitReaction")
	FHitReactionAnimations HitReactionAnimations;

	FOnMontageEnded DownRecoveryMontageEndedDelegate;


public:
	AActionPortfolioCharacter();
	
/////////////////// Hit React /////////////////////
private:
	TWeakObjectPtr<class UAnimMontage> LastHitReactAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death", meta = (AllowPrivateAccess = "true"))
	bool bDestroyOnDie;

public:
	FOnCharcterDieDelegate OnCharacterDie;

private:
	void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted);


public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Character")
	void HitReact(EHitReactionDirection Direction, bool bForceDown, UAnimMontage* ForceHitReactionAnim = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Attributes")
	bool IsCharacterDie() const;

	void CharacterDie();

/////////////////// AbilitySystem ////////////////////
private:
	FActiveGameplayEffectHandle IsInAirHandle;
	FActiveGameplayEffectHandle DownHandle;

private:
	void CheckInAir();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	bool IsInAir() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	bool IsDown() const;

	void SetDown(bool NewState);

protected:
	virtual void InitializeAttributes();
	virtual void AddCharacterAbilities();
	virtual void AddStartupEffects();

	virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	virtual void OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved);

	virtual void InitializeAttributeChangedDelegate();
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data);

	virtual void RigidityTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void OnDownRecoveryMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	void InitializeAbilitySystem();

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	float GetMaxHealth() const;


///////////////// Rigidity ///////////////////
private:
	FActiveGameplayEffectHandle RigidityHandle;
	float RigidityTime;
	
private:
	void RemoveRigidityHandle();

public:
	void CharacterRigidity(float NewRigidityTime);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	bool IsRigidity() const;

	void RecoveryHitReaction();

///////////////// Interaction //////////////////////////

protected:
	// To add mapping context
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds);



public:
	virtual void PostInitializeComponents() override;


///////////////// Attack //////////////////////////
private:
	void InitForAttackShape();


public:
	class UShapeComponent* GetAttackShape(FName AttackShapeTag);

	UFUNCTION(BlueprintCallable,  Category = "ActionPF|Ability")
	void ActivateActionPFAbility(TSubclassOf<class UActionPFGameplayAbility> AbilityClass);

	virtual void Landed(const FHitResult& Hit);


///////////////// Team //////////////////////////
private:
	FGenericTeamId TeamID;

public:

	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override { TeamID = NewTeamID; };

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }

	virtual void PossessedBy(AController* NewController) override;
};

