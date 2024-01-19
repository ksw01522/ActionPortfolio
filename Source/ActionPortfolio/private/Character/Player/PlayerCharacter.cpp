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

APlayerCharacter::APlayerCharacter()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	bInitializedItemUser = false;
}

void APlayerCharacter::AddCharacterAbilities()
{
	UActionPFAbilitySystemComponent* ThisAbilitySystem = Cast<UActionPFAbilitySystemComponent>(GetAbilitySystemComponent());
	if(ThisAbilitySystem->bCharacterAbilitiesGiven) return;
	
	ThisAbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability_LMB_Action.AbilityClass, 1, -1, this));
	ThisAbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability_RMB_Action.AbilityClass, 1, -1, this));
	ThisAbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability_E_Action.AbilityClass, 1, -1, this));
	ThisAbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability_Q_Action.AbilityClass, 1, -1, this));
	ThisAbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability_R_Action.AbilityClass, 1, -1, this));

	Super::AddCharacterAbilities();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	InitializeItemUser();
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

TSubclassOf<class UActionPFGameplayAbility> APlayerCharacter::GetPlayerAbilityClass(EPlayerAbilityInputID ID)
{
	switch (ID)
	{
	case EPlayerAbilityInputID::Ability_LMB:
		return Ability_LMB_Action.AbilityClass;
		break;

	case EPlayerAbilityInputID::Ability_RMB:
		return Ability_RMB_Action.AbilityClass;
		break;

	case EPlayerAbilityInputID::Ability_E:
		return Ability_E_Action.AbilityClass;
		break;

	case EPlayerAbilityInputID::Ability_Q:
		return Ability_Q_Action.AbilityClass;
		break;

	case EPlayerAbilityInputID::Ability_R:
		return Ability_R_Action.AbilityClass;
		break;
	}

	return nullptr;
}

void APlayerCharacter::InitializeItemUser()
{
	if(bInitializedItemUser) return;

	EquipmentSlots.Add(EEquipmentPart::Body, nullptr);
	EquipmentSlots.Add(EEquipmentPart::Pants, nullptr);
	EquipmentSlots.Add(EEquipmentPart::Head, nullptr);
	EquipmentSlots.Add(EEquipmentPart::Arm, nullptr);
	EquipmentSlots.Add(EEquipmentPart::Foot, nullptr);

	bInitializedItemUser = true;
}



void APlayerCharacter::OnEquipItem(UItemBase_Equipment* NewItem)
{
	EquipmentSlots[NewItem->GetEquipmentPart()] = NewItem;
}

void APlayerCharacter::OnUnequipItem(UItemBase_Equipment* NewItem)
{
	EquipmentSlots[NewItem->GetEquipmentPart()] = nullptr;
}

UActionPFAbilitySystemComponent* APlayerCharacter::GetASCForItemUser() const
{
	return Cast<UActionPFAbilitySystemComponent>(GetAbilitySystemComponent());
}

bool APlayerCharacter::CanEquipItem(UItemBase_Equipment* NewItem) const
{
	bool bResult = IItemUserInterface::CanEquipItem(NewItem) && EquipmentSlots.Contains(NewItem->GetEquipmentPart());
	
	return bResult;
}



bool APlayerCharacter::EquipItem(UItemBase_Equipment* NewItem)
{
	if(!CanEquipItem(NewItem)) return false;
	UnequipItem(EquipmentSlots[NewItem->GetEquipmentPart()]);

	bool bResult = NewItem->TryEquipItem(this);
	if(bResult) OnEquipItem(NewItem);
	return bResult;
}

bool APlayerCharacter::UnequipItem(UItemBase_Equipment* NewItem)
{
	if (!IsValid(NewItem)) return false;
	bool bResult = NewItem->TryUnequipItem(this);
	if(bResult) OnUnequipItem(NewItem);

	return bResult;
}

UItemBase_Equipment* APlayerCharacter::GetEquipment(EEquipmentPart Part) const
{
	if(!EquipmentSlots.Contains(Part)) return nullptr;
	return EquipmentSlots[Part];
}

#if WITH_EDITOR
FName APlayerCharacter::GetUserName() const
{
	return FName("PlayerCharacter");
}
#endif



//////////////////////////////////////////////////////////////////////////
// Input

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {


		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		EnhancedInputComponent->BindAction(Ability_LMB_Action.InputAction, ETriggerEvent::Started, this, &APlayerCharacter::ActivateInputAbility, EPlayerAbilityInputID::Ability_LMB);
		EnhancedInputComponent->BindAction(Ability_RMB_Action.InputAction, ETriggerEvent::Started, this, &APlayerCharacter::ActivateInputAbility, EPlayerAbilityInputID::Ability_RMB);
		EnhancedInputComponent->BindAction(Ability_E_Action.InputAction, ETriggerEvent::Started, this, &APlayerCharacter::ActivateInputAbility, EPlayerAbilityInputID::Ability_E);
		EnhancedInputComponent->BindAction(Ability_Q_Action.InputAction, ETriggerEvent::Started, this, &APlayerCharacter::ActivateInputAbility, EPlayerAbilityInputID::Ability_Q);
		EnhancedInputComponent->BindAction(Ability_R_Action.InputAction, ETriggerEvent::Started, this, &APlayerCharacter::ActivateInputAbility, EPlayerAbilityInputID::Ability_R);
	}

}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if(!CanCharacterMove()) return;

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
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

void APlayerCharacter::ActivateInputAbility(const FInputActionValue& Value, EPlayerAbilityInputID ID)
{
	switch (ID) 
	{
		case EPlayerAbilityInputID::Ability_LMB:
			ActivateActionPFAbility(Ability_LMB_Action.AbilityClass);
			break;
		
		case EPlayerAbilityInputID::Ability_RMB:
			ActivateActionPFAbility(Ability_RMB_Action.AbilityClass);
			break;

		case EPlayerAbilityInputID::Ability_E:
			ActivateActionPFAbility(Ability_E_Action.AbilityClass);
			break;

		case EPlayerAbilityInputID::Ability_Q:
			ActivateActionPFAbility(Ability_Q_Action.AbilityClass);
			break;

		case EPlayerAbilityInputID::Ability_R:
			ActivateActionPFAbility(Ability_R_Action.AbilityClass);
			break;
	}
}


