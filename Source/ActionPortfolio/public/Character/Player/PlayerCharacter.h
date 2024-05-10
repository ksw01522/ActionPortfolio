// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ActionPortfolioCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
 enum class EEquipmentPart : uint8;

struct FAddedPlayerAbilityStruct
{
	FAddedPlayerAbilityStruct(const FGameplayAbilitySpecHandle& InHandle, const TSubclassOf<UActionPFGameplayAbility>& InClass, TSharedPtr<class SAbilityIcon> InIcon) :
						Handle(InHandle), AbilityClass(InClass), Icon(InIcon)
	{}
	virtual ~FAddedPlayerAbilityStruct() {}

	FGameplayAbilitySpecHandle Handle;

	TSubclassOf<UActionPFGameplayAbility> AbilityClass;

	TSharedPtr<SAbilityIcon> Icon;
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
	TObjectPtr<class USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerMappableInputConfig> CharacterInputConfig;

	/** Move Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Look Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

//Player Main UI
private:
	/* UIClass For Create UI On Possessed By Player Controller */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UWidget_PlayerMainUI> MainUIClass;

	UPROPERTY(Transient)
	TObjectPtr<UWidget_PlayerMainUI> MainUI;

	/* Create UI On Possessed By Player Controller */
	void TryCreateMainUI();
	
	virtual void PreCreateMainUI() {}

	/* Call Function On Complete Create Main UI */
	virtual void PostCreateMainUI(){}

	/* Remove UI On Unpossessed By Player Controller */
	void TryRemoveMainUI();

	virtual void PreRemoveMainUI() {}

	/* Call Function On Remove UI */
	virtual void PostRemoveMainUI() {}

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess ="true"))
	TObjectPtr<class UCharacterStatusComponent> CSC;

public:
	UCharacterStatusComponent* GetCharacterStatusComponent() const { return CSC; }


public:
	UWidget_PlayerMainUI* GetPlayerMainUI() const { return MainUI; }

//Character Ability
private:
	//Key = InputID, Value = Act Ability By Receive Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability, Instanced, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UAbilitySlotWithInput>> AbilitiesWithInput;

	//On Added Active Ability Container
	TArray<FAddedPlayerAbilityStruct> AddedActiveAbilities;

	//On Added Passive AbilityContainer
	TArray<FAddedPlayerAbilityStruct> AddedPassiveAbilities;

private:
	//Event for Give CharacterAbility
	virtual void OnAddedAbility(FGameplayAbilitySpecHandle Handle) override;

	//virtual void OnHealthChanged(const FOnAttributeChangeData& Data) override;
	//virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data) override;

public:
	const TSharedPtr<SAbilityIcon>& GetGivenAbilityIcon(TSubclassOf<UActionPFGameplayAbility> InAbilityClass) const;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void PressInputAbility(const FInputActionValue& Value, int ID);
	void ReleaseInputAbility(const FInputActionValue& Value, int ID);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

public:
	virtual void LinkSkillHotKeyWindow(class USkillHotKeyWindow* SkillWindow);

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
