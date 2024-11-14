// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ActionPortfolioCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
 enum class EEquipmentPart : uint8;

UCLASS()
class ACTIONPORTFOLIO_API APlayerCharacter : public AActionPortfolioCharacter
{
	GENERATED_BODY()
public:
	APlayerCharacter();

private:
		/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerMappableInputConfig> OptionalInputConfig;

	/** Move Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Look Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> WeakAttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> StrongAttackAction;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess ="true"))
	TObjectPtr<class UCharacterStatusComponent> CSC;

public:
	UCharacterStatusComponent* GetCharacterStatusComponent() const { return CSC; }

private:
	void OnPlayerLevelChanged(const FOnAttributeChangeData& ChangeData);
	void OnPlayerLevelChanged(float NewLevel);
private:
//Character Ability
	//Key = InputID, Value = Act Ability By Receive Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability, Instanced, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UAbilitySlot_HotKey>> AbilityHotKeySlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UActionPFGameplayAbility> WeakAttackClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UActionPFGameplayAbility> StrongAttackClass;

public:
	void GetAbilityHotKeySlots(TArray<UAbilitySlot_HotKey*>& OutArray);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void PressInputAbility(const FInputActionValue& Value, UAbilitySlot_HotKey* HotKey);
	void ReleaseInputAbility(const FInputActionValue& Value, UAbilitySlot_HotKey* HotKey);
	
	void PressInputAbilityID(const FInputActionValue& Value, int ID);
	void ReleaseInputAbilityID(const FInputActionValue& Value, int ID);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void AddStartupAbilities() override;
	virtual void InitializeAttributes() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	//UFUNCTION(BlueprintCallable, Category = "Player|Ability")
	//TSubclassOf<class UActionPFGameplayAbility> GetPlayerAbilityClass(int ID);

#if WITH_EDITOR
protected:
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;

	virtual void PostCDOCompiled(const FPostCDOCompiledContext& Context) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
};
