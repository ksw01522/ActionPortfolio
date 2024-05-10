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
#include "Character/Player/Widget_PlayerMainUI.h"
#include "Ability/Widget/AbilitySlotWidget.h"
#include "Misc/DataValidation.h"
#include "Ability/Slot/AbilitySlotWithInput.h"

#include "PlayerMappableInputConfig.h"
#include "CustomInputSettingSubsystem.h"

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

	CSC = CreateDefaultSubobject<UCharacterStatusComponent>(TEXT("CharacterStatus"));

	MainUI = nullptr;
}

void APlayerCharacter::TryCreateMainUI()
{
	AActionPFPlayerController* PlayerController = GetController<AActionPFPlayerController>();

	PreCreateMainUI();

	MainUI = CreateWidget<UWidget_PlayerMainUI>(PlayerController, MainUIClass, "CharacterMainUI");
	if (MainUI)
	{
		MainUI->GetSkillHotkeyWindow()->SetOwnerCharacter(this);

		PostCreateMainUI();

		MainUI->AddToViewport(0);

		PlayerController->SetPlayerMainUI(MainUI);
	}

	
}




void APlayerCharacter::TryRemoveMainUI()
{
	if (MainUI == nullptr) return;

	PreRemoveMainUI();

	MainUI->RemoveFromParent();
	MainUI->Destruct();
	MainUI = nullptr;

	PostRemoveMainUI();
}

void APlayerCharacter::OnAddedAbility(FGameplayAbilitySpecHandle Handle)
{
	if(!Handle.IsValid()) return;

	UActionPFAbilitySystemComponent* System = static_cast<UActionPFAbilitySystemComponent*>(GetAbilitySystemComponent());
	FGameplayAbilitySpec* Spec = System->FindAbilitySpecFromHandle(Handle);

	UActionPFGameplayAbility* Ability = static_cast<UActionPFGameplayAbility*>(Spec->Ability.Get());
	TSharedPtr<SAbilityIcon> IconPtr = Ability->CreateAbilityIcon();
	IconPtr->LinkAbilitySystem(System);

	EAbilityType Type = Ability->GetAbilityType();

	if (Type == EAbilityType::Active)
	{
		AddedActiveAbilities.Add(FAddedPlayerAbilityStruct( Handle, Ability->GetClass(), IconPtr));
	}
	else if(Type == EAbilityType::Passive)
	{
		AddedPassiveAbilities.Add(FAddedPlayerAbilityStruct(Handle, Ability->GetClass(), IconPtr));
	}

	AActionPFPlayerController* TempController = GetController<AActionPFPlayerController>();
	UWidget_PlayerMainUI* PlayerMainUI = nullptr;
	USkillHotKeyWindow* SkillWindow = nullptr;

	if (TempController)
	{
		PlayerMainUI = TempController->GetPlayerMainUI();
		if (PlayerMainUI)
		{
			SkillWindow = PlayerMainUI->GetSkillHotkeyWindow();
		}
	}

	for (auto& InputAbilitySlot : AbilitiesWithInput)
	{
		if (Ability->GetClass() == InputAbilitySlot->GetAbilityClass())
		{
			Spec->InputID = InputAbilitySlot->GetInputID();
		}
	}
}


const TSharedPtr<SAbilityIcon>& APlayerCharacter::GetGivenAbilityIcon(TSubclassOf<UActionPFGameplayAbility> InAbilityClass) const
{
	for (const auto& AddedActiveAbility : AddedActiveAbilities)
	{
		if(AddedActiveAbility.AbilityClass == InAbilityClass){
			return AddedActiveAbility.Icon;
		}
	}

	for (const auto& AddedPassiveAbility : AddedPassiveAbilities)
	{
		if (AddedPassiveAbility.AbilityClass == InAbilityClass) return AddedPassiveAbility.Icon;
	}

	return SAbilityIcon::GetEmptyIcon();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	for (auto& InputAbilitySlot : AbilitiesWithInput)
	{
		InputAbilitySlot->SetOwnerPlayer(this);
	}

}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AActionPFPlayerController* PlayerController = Cast<AActionPFPlayerController>(NewController))
	{
		PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddPlayerMappableConfig(CharacterInputConfig);
		
		TryCreateMainUI();

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			

			for (auto& AbilityWithInput : AbilitiesWithInput)
			{
				const FName& KeyboardMappingKey = AbilityWithInput->GetKeyboardMappingKey();
				const FName& GamepadMappingKey = AbilityWithInput->GetGamepadMappingKey();

				FKey KeyboardKey = Subsystem->GetPlayerMappedKey(KeyboardMappingKey);
				if (!KeyboardKey.IsValid())
				{
					KeyboardKey = CharacterInputConfig->GetMappingByName(KeyboardMappingKey).Key;
				}

				FKey GamepadKey = Subsystem->GetPlayerMappedKey(GamepadMappingKey);
				if (!GamepadKey.IsValid())
				{
					GamepadKey = CharacterInputConfig->GetMappingByName(GamepadMappingKey).Key;
				}


				AbilityWithInput->SetHotKeysDirect(KeyboardKey, GamepadKey);
			}
		}
	}
}

void APlayerCharacter::UnPossessed()
{
	if (AActionPFPlayerController* PlayerController = Cast<AActionPFPlayerController>(Controller))
	{
		PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->RemovePlayerMappableConfig(CharacterInputConfig);
	
		TryRemoveMainUI();
	}

	Super::UnPossessed();
}

void APlayerCharacter::LinkSkillHotKeyWindow(USkillHotKeyWindow* SkillWindow)
{
	if(SkillWindow == nullptr) return;

	for (auto& InputAbilitySlot : AbilitiesWithInput)
	{
		UAbilitySlotWidget* TempSlot = SkillWindow->GetAbilitySlot(InputAbilitySlot->GetInputID());
		if (TempSlot != nullptr)
		{
			InputAbilitySlot->LinkAbilitySlotSlate(TempSlot->GetSlotSlate().ToSharedRef());
		}
	}

}

#if WITH_EDITOR

EDataValidationResult APlayerCharacter::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = Super::IsDataValid(ValidationErrors);
	
	if(Result == EDataValidationResult::NotValidated) return Result;

	TSet<int> TempKeys;
	TSet<int> ErrorKeys;
	bool bExistNullAbilityWithInputOBJ = false;

	for (auto& AbilityWithInput : AbilitiesWithInput)
	{
		if (AbilityWithInput == nullptr) {
			bExistNullAbilityWithInputOBJ = true;
			continue;
		}

		bool bIsAlready = false;

		TempKeys.Add(AbilityWithInput->GetInputID(), &bIsAlready);
		if (bIsAlready)
		{
			ErrorKeys.Add(AbilityWithInput->GetInputID(), nullptr);
		}
	}

	if (bExistNullAbilityWithInputOBJ)
	{
		ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("Exist None AbilityWithInput Object."))));
	}

	for (int& ErrorKey : ErrorKeys)
	{
		ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("{%d} Exist 2 or more."), ErrorKey)));
	}

	return ErrorKeys.IsEmpty() && !bExistNullAbilityWithInputOBJ ? Result : EDataValidationResult::Invalid;
}

void APlayerCharacter::PostCDOCompiled(const FPostCDOCompiledContext& Context)
{
	Super::PostCDOCompiled(Context);

	AbilitiesWithInput.Sort([](const UAbilitySlotWithInput& A, const UAbilitySlotWithInput& B) -> bool { return A.GetInputID() < B.GetInputID(); });
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
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		for (auto& AbilityWhitInput : AbilitiesWithInput)
		{
			check(AbilityWhitInput->GetAbilityInputAction() != nullptr);

			EnhancedInputComponent->BindAction(AbilityWhitInput->GetAbilityInputAction(), ETriggerEvent::Started, this, &APlayerCharacter::PressInputAbility, AbilityWhitInput->GetInputID());
			EnhancedInputComponent->BindAction(AbilityWhitInput->GetAbilityInputAction(), ETriggerEvent::Completed, this, &APlayerCharacter::ReleaseInputAbility, AbilityWhitInput->GetInputID());
		}

		
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

void APlayerCharacter::PressInputAbility(const FInputActionValue& Value, int ID)
{
#if WITH_EDITOR
	PFLOG(Warning, TEXT("Press : %d"), ID);
#endif
	GetAbilitySystemComponent()->PressInputID(ID);
}

void APlayerCharacter::ReleaseInputAbility(const FInputActionValue& Value, int ID)
{
	GetAbilitySystemComponent()->ReleaseInputID(ID);
}


