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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDamagedDelegate, float, DamageAmount, AActor*, DamageInstigator);

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
	FHitReactionAnimations() {
		Ground_Front = nullptr;
		Ground_Back = nullptr;
		Ground_Right = nullptr;
		Ground_Left = nullptr;
		InAir_Front = nullptr;
		InAir_Back = nullptr;
		InAir_Right = nullptr;
		InAir_Left = nullptr;
		Down = nullptr;
		DownedRecovery = nullptr;
		Death = nullptr;
	}

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

	UPROPERTY(Transient)
	TMap<FName, class UShapeComponent*> AttackShapeMap;

	TWeakObjectPtr<AActionPortfolioCharacter> LastAttackedTarget;
public:
	AActionPortfolioCharacter* GetLastAttackedTarget() const {return LastAttackedTarget.Get(); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TArray<TSubclassOf<class UActionPFGameplayAbility>> CharacterAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

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
	FOnCharacterDamagedDelegate OnDamagedDel;

private:
	void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted);


public:
	UFUNCTION(BlueprintCallable, Category = "ActionPF|Character")
	void HitReact(EHitReactionDirection Direction, bool bForceDown, UAnimMontage* ForceHitReactionAnim = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ActionPF|Attributes")
	bool IsCharacterDie() const;

	virtual void CharacterDie();

	virtual void OnDamageEvent(float DamageAmount, AActor* DamageInstigator);

	virtual void OnAttackEvent(float DamageAmount, AActor* Target);


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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool CanCharacterMove() const;

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
	virtual void Tick(float DeltaSeconds) override;



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
public:
	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual void PossessedBy(AController* NewController) override;


///////////////// Movement //////////////////////////
private:
	bool bDefaultOrientRotationToMovement;
	float DefaultWalkSpeed;
	float DefaultMaxAcceleration;

protected:
	void InitializeMovement();

public:
	void ResetMovement();



///////////////// For AI //////////////////////////
public:
	virtual bool CanBasicAct() const;
};

