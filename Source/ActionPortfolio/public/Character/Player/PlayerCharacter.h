// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ActionPortfolioCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerAbilityInputID : uint8
{
	Ability_LMB = 0,
	Ability_RMB = 1,
	Ability_Q = 2,
	Ability_E = 3,
	Ability_R = 4
};

USTRUCT(BlueprintType)
struct FPlayerInputAbilityStruct 
{
	GENERATED_BODY()
	FPlayerInputAbilityStruct(){}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UActionPFGameplayAbility> AbilityClass;

};

UCLASS()
class ACTIONPORTFOLIO_API APlayerCharacter : public AActionPortfolioCharacter
{
	GENERATED_BODY()
public:
	APlayerCharacter();

private:
		/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FPlayerInputAbilityStruct Ability_LMB_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FPlayerInputAbilityStruct Ability_RMB_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FPlayerInputAbilityStruct Ability_Q_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FPlayerInputAbilityStruct Ability_E_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FPlayerInputAbilityStruct Ability_R_Action;


private:
	virtual void AddCharacterAbilities() override;
	//virtual void OnHealthChanged(const FOnAttributeChangeData& Data) override;
	//virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data) override;

protected:
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void ActivateInputAbility(const FInputActionValue& Value, EPlayerAbilityInputID ID);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	TArray<TWeakObjectPtr<UInteractionSystemComponent>> InteractionSystemArray;

	void InteractFocusedInteraction();
public:
	void AddInteraction(UInteractionSystemComponent* NewSystem);
	void RemoveInteraction(UInteractionSystemComponent* TargetSystem);
	UInteractionSystemComponent* GetFocusedInteraction();
	void ChangeFocusInteraction();

	UFUNCTION(BlueprintCallable, Category = "Player|Ability")
	TSubclassOf<class UActionPFGameplayAbility> GetPlayerAbilityClass(EPlayerAbilityInputID ID);
};
