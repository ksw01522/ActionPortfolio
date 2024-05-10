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

UENUM(BlueprintType, meta = (Bitflags, UseEnumValueAsMaskValuesInEidtor = "true"))
enum class EDebuffState : uint8
{
	None = 0,
	Poison = 1 << 0,
	Flame = 1 << 1,
	Stun = 1 << 2,
	ElectricShock = 1 << 3
};

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
		Front = nullptr;
		Back = nullptr;
		Right = nullptr;
		Left = nullptr;

		Death = nullptr;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> Front;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> Back;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
	TObjectPtr<UAnimMontage> Death;
};

USTRUCT(BlueprintType)
struct FAbilityDataForGive
{
	GENERATED_BODY()
public:
	FAbilityDataForGive() : Level(1)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<class UActionPFGameplayAbility> Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (ClampMin = "1"))
	int Level;
};

UCLASS(config=Game)
class AActionPortfolioCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface, public ICustomAbilityHelperInterface
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

#if WITH_EDITOR
protected:
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif // WITH_EDITOR

///////////////// Team //////////////////////////
public:
	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;

/////////////////// AbilitySystem ////////////////////
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UActionPFAbilitySystemComponent> AbilitySystem;

	UPROPERTY(BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UActionPFAttributeSet> AttributeSet;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TArray<FAbilityDataForGive> CharacterAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//Initialize Ability System Component
private:
	void InitializeAbilitySystem();
		void InitializeAttributes();
		void AddCharacterAbilities();
		void AddStartupEffects();
		
	void ClearCharacterAbilities();

	virtual void OnAddedAbility(FGameplayAbilitySpecHandle Handle) {}
		
protected:
	virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	virtual void OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved);



///////////////// Attributte /////////////////
protected:


	virtual void InitializeAttributeChangedDelegate();
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data);

public:


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool CanCharacterMove() const;


//////////////////////////////// CustomAbilityHelperInterface ////////////////////////////////
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitReaction", meta = (AllowPrivateAccess = "true"))
	FHitReactionAnimations HitReactionAnimations;

protected:
	virtual void AttachComponentToCharacter(class USceneComponent* InComponent, const FAttachmentTransformRules& AttachmentRules, const FName& SocketName) override;
	virtual UAnimMontage* GetRigidityAnim(float RigidityTime, const FHitResult* HitResult) const override;


//////////////////////////////// Character State ////////////////////////////////
private:
	uint8 DebuffState;

public:
	uint8 GetDebuffState() const { return DebuffState; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death", meta = (AllowPrivateAccess = "true"))
	bool bDestroyOnDie;

public:
	UPROPERTY(BlueprintAssignable)
	FOnCharcterDieDelegate OnCharacterDie;
	
	
	FOnCharacterDamagedDelegate OnDamagedDel;

private:
	void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CharacterState")
	bool IsCharacterDie() const;

	virtual void CharacterDie();

	virtual void OnDamageEvent(float DamageAmount, AActor* DamageInstigator);

	virtual void OnAttackEvent(float DamageAmount, AActor* Target);

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

//////////////////////// Lock On State Widget ////////////////////
private:
	UPROPERTY(EditDefaultsOnly, Category = "LockOnState")
	TSubclassOf<class ULockOnStateWidget> LockOnStateWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<ULockOnStateWidget> LockOnStateWidget;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ULockOnStateWidget* GetLockOnStateWidget() const { return LockOnStateWidget.Get(); }
};

