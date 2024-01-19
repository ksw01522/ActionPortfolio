// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Items/ItemUserInterface.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
 enum class EEquipmentPart : uint8;

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
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UActionPFGameplayAbility> AbilityClass;

};

UCLASS()
class ACTIONPORTFOLIO_API APlayerCharacter : public AActionPortfolioCharacter, public IItemUserInterface
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

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

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

public:
	UFUNCTION(BlueprintCallable, Category = "Player|Ability")
	TSubclassOf<class UActionPFGameplayAbility> GetPlayerAbilityClass(EPlayerAbilityInputID ID);

////////////////////////////////////////// ItemUserInterface /////////////////////////////////
private:
	UPROPERTY(Transient)
	TMap<EEquipmentPart, TObjectPtr<UItemBase_Equipment>> EquipmentSlots;

	bool bInitializedItemUser;

	virtual void InitializeItemUser() override;

	

	virtual void OnEquipItem(UItemBase_Equipment* NewItem) override;
	virtual void OnUnequipItem(UItemBase_Equipment* NewItem) override;

public:
	virtual class UActionPFAbilitySystemComponent* GetASCForItemUser() const override;

	virtual bool CanEquipItem(UItemBase_Equipment* NewItem) const;

	virtual bool EquipItem(UItemBase_Equipment* NewItem) override;

	virtual bool UnequipItem(UItemBase_Equipment* NewItem) override;

	UItemBase_Equipment* GetEquipment(EEquipmentPart Part) const;

#if WITH_EDITOR
	virtual FName GetUserName() const override;
#endif
};
