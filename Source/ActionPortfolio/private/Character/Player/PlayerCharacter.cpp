// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/PlayerCharacter.h"
#include "ActionPortfolio.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemInterface.h"
#include "Interaction/InteractionSystemComponent.h"

#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/Ability/GameplayAbility_Meelee.h"
#include "Ability/ActionPFAttributeSet.h"
#include "Character/CharacterStatusComponent.h"

#include "Items/ItemBase.h"
#include "Items/ItemManagerSubsystem.h"

#include "CustomInputHelper.h"

#include "Character/Player/ActionPFPlayerController.h"

#include "Ability/Widget/SAbilitySlot.h"
#include "Ability/Widget/SAbilityIcon.h"
#include "Character/Player/Ability/SkillHotKeyWindow.h"
#include "Ability/Widget/AbilitySlotWidget.h"
#include "Misc/DataValidation.h"
#include "Ability/Slot/AbilitySlot_HotKey.h"

#include "PlayerMappableInputConfig.h"
#include "CustomInputSettingSubsystem.h"
#include "Items/ItemDataAsset.h"

#include "Engine/AssetManager.h"
#include "Instance/CharacterDataManager.h"
#include "Ability/Effects/AttributeEffect.h"
#include "Ability/ActionPFAttributeSet.h"

APlayerCharacter::APlayerCharacter()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocationAndRotation(FVector(0, 0, -70), FRotator(-40, 90, 0));
	CameraBoom->TargetArmLength = 1800.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = false;

	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritPitch = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CSC = CreateDefaultSubobject<UCharacterStatusComponent>(TEXT("CharacterStatus"));

	UActionPFAbilitySystemComponent* ASC = Cast<UActionPFAbilitySystemComponent>(GetAbilitySystemComponent());
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	SetGenericTeamId(1);
}





void APlayerCharacter::GetAbilityHotKeySlots(TArray<UAbilitySlot_HotKey*>& OutArray)
{
	OutArray.Empty();

	for (auto& AbilityHotKeySlot : AbilityHotKeySlots)
	{
		OutArray.Add(AbilityHotKeySlot);
	}
}

void APlayerCharacter::BeginPlay()
{
	PFLOG(Warning, TEXT(""));
	Super::BeginPlay();
	
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetPlayerLevelAttribute())
		.AddUObject(this, &APlayerCharacter::OnPlayerLevelChanged);

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->GenericConfirmInputID = 0;

		ASC->GenericCancelInputID = 1;
	}
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}

void APlayerCharacter::UnPossessed()
{
	Super::UnPossessed();
}


void APlayerCharacter::AddStartupAbilities()
{
	Super::AddStartupAbilities();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (FGameplayAbilitySpec* WeakAttackSpec = ASC->FindAbilitySpecFromClass(WeakAttackClass))
		{
			WeakAttackSpec->InputID = 0;
		}

		if (FGameplayAbilitySpec* StrongAttackSpec = ASC->FindAbilitySpecFromClass(StrongAttackClass))
		{
			StrongAttackSpec->InputID = 1;
		}
	}
}

void APlayerCharacter::InitializeAttributes()
{
	bool bFoundStartLevel;
	float StartLevel = GetAbilitySystemComponent()->GetGameplayAttributeValue(UPlayerAttributeSet::GetPlayerLevelAttribute(), bFoundStartLevel);

	PFLOG(Warning, TEXT("Start Level : %.1f"), StartLevel);
	OnPlayerLevelChanged(bFoundStartLevel ? StartLevel : 1);

}



#if WITH_EDITOR

EDataValidationResult APlayerCharacter::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = Super::IsDataValid(ValidationErrors);

	for (int I = 0 ; I < AbilityHotKeySlots.Num(); I++)
	{
		if (AbilityHotKeySlots[I]->GetHotKeyAction() == nullptr)
		{
			Result = EDataValidationResult::Invalid;

			ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("Hot Key Action Is null : %d"), I)));
		}
	}

	return Result;
}


void APlayerCharacter::PostCDOCompiled(const FPostCDOCompiledContext& Context)
{
	Super::PostCDOCompiled(Context);
}

void APlayerCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}



#endif




//////////////////////////////////////////////////////////////////////////
// Input

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent);

	AActionPFPlayerController* PlayerController = GetController<AActionPFPlayerController>();
	check(PlayerController);

	if (OptionalInputConfig != nullptr)
	{
		FModifyContextOptions Option;
		Option.bForceImmediately = true;
		PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddPlayerMappableConfig(OptionalInputConfig, Option);
	}

	//Moving
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

	//Looking 일단 필요없음
	//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

	EnhancedInputComponent->BindAction(WeakAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PressInputAbilityID, 0);
	EnhancedInputComponent->BindAction(WeakAttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::ReleaseInputAbilityID, 0);

	EnhancedInputComponent->BindAction(StrongAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PressInputAbilityID, 1);
	EnhancedInputComponent->BindAction(StrongAttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::ReleaseInputAbilityID, 1);

	for (auto& HotKeySlot : AbilityHotKeySlots)
	{
		UInputAction* InputAction = HotKeySlot->GetHotKeyAction();
		check(InputAction);

		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PressInputAbility, HotKeySlot.Get());
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &APlayerCharacter::ReleaseInputAbility, HotKeySlot.Get());
	}
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if(!CanCharacterMove()) return;

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();


	//if (Controller != nullptr)
	//{
	//	// find out which way is forward
	//	const FRotator Rotation = Controller->GetControlRotation();
	//	const FRotator YawRotation(0, Rotation.Yaw, 0);

	//	// get forward vector
	//	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	//	// get right vector 
	//	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//	// add movement 
	//	AddMovementInput(ForwardDirection, MovementVector.Y);
	//	AddMovementInput(RightDirection, MovementVector.X);
	//}

	if (Controller != nullptr)
	{
		AddMovementInput(FVector::XAxisVector, -MovementVector.X);
		AddMovementInput(FVector::YAxisVector, MovementVector.Y);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::PressInputAbility(const FInputActionValue& Value, UAbilitySlot_HotKey* HotKey)
{
	UActionPFAbilitySystemComponent* ASC = StaticCast<UActionPFAbilitySystemComponent*>(GetAbilitySystemComponent());
	ASC->AbilityLocalInputPressedByClass(HotKey->GetAbilityInSlot());
}

void APlayerCharacter::ReleaseInputAbility(const FInputActionValue& Value, UAbilitySlot_HotKey* HotKey)
{
	UActionPFAbilitySystemComponent* ASC = StaticCast<UActionPFAbilitySystemComponent*>(GetAbilitySystemComponent());
	ASC->AbilityLocalInputReleassedByClass(HotKey->GetAbilityInSlot());
}

void APlayerCharacter::PressInputAbilityID(const FInputActionValue& Value, int ID)
{
	UActionPFAbilitySystemComponent* ASC = StaticCast<UActionPFAbilitySystemComponent*>(GetAbilitySystemComponent());
	ASC->PressInputID(ID);
}

void APlayerCharacter::ReleaseInputAbilityID(const FInputActionValue& Value, int ID)
{
	UActionPFAbilitySystemComponent* ASC = StaticCast<UActionPFAbilitySystemComponent*>(GetAbilitySystemComponent());
	ASC->ReleaseInputID(ID);
}


#define ADD_SET_ATTRIBUTE_PLAYER(PropertyName)\
{\
	FRealCurve * FindedCurve = PlayerCurveTable->FindCurve(FName(TempName + #PropertyName), ""); \
	FKeyHandle FindedKeyHandle = FindedCurve->FindKey(NewLevel); \
	float FindedValue = FindedCurve->GetKeyValue(FindedKeyHandle); \
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::Get##PropertyName##Name(), FindedValue);\
}

void APlayerCharacter::OnPlayerLevelChanged(const FOnAttributeChangeData& ChangeData)
{
	OnPlayerLevelChanged(ChangeData.NewValue);
}

void APlayerCharacter::OnPlayerLevelChanged(float NewLevel)
{
	const UCharacterDataManager* DataManager = UCharacterDataManager::GetCharacterDataManager();
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

	const UCurveTable* PlayerCurveTable = DataManager->GetPlayerDataTable();

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();

	FString TempName = GetCharacterCode().ToString() + ".";

	FGameplayEffectSpec EffectSpec(GetDefault<UAttributeEffect_Base>(), ContextHandle, 1);

	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetCharacterLevelName(), NewLevel);

	ADD_SET_ATTRIBUTE_PLAYER(MaxHealth)
	ADD_SET_ATTRIBUTE_PLAYER(HealthRegen)
	ADD_SET_ATTRIBUTE_PLAYER(MaxStamina)
	ADD_SET_ATTRIBUTE_PLAYER(StaminaRegen)
	ADD_SET_ATTRIBUTE_PLAYER(AttackP)
	ADD_SET_ATTRIBUTE_PLAYER(DefenseP)
	ADD_SET_ATTRIBUTE_PLAYER(FireResistance)
	ADD_SET_ATTRIBUTE_PLAYER(IceResistance)
	ADD_SET_ATTRIBUTE_PLAYER(ElectricResistance)

	ASC->ApplyGameplayEffectSpecToSelf(EffectSpec);
}
#undef ADD_SET_ATTRIBUTE_PLAYER
