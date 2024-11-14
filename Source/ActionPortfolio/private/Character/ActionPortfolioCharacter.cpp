// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/ActionPortfolioCharacter.h"
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
#include "Ability/ActionPFGameplayAbility.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Character/Player/ActionPFPlayerController.h"
#include "Character/ActionPFAnimInstance.h"

#include "Ability/Ability/Ability_Rigidity.h"

#include "Character/LockOnStateWidget.h"
#include "LockOn/LockOnTargetComponent.h"

#include "Net/UnrealNetwork.h"

#include "Ability/Effects/CharacterRegenEffect.h"

#include "Ability/Effects/AttributeEffect.h"
#include "Instance/CharacterDataManager.h"

#include "Ability/Effects/AttributeEffect.h"


//////////////////////////////////////////////////////////////////////////
// AActionPortfolioCharacter

AActionPortfolioCharacter::AActionPortfolioCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->bForceMaxAccel = false;


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	AbilitySystem = CreateDefaultSubobject<UActionPFAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));
	AbilitySystem->AddAttributeSetSubobject<UCharacterAttributeSet>(AttributeSet);

	SetReplicates(true);

	PrimaryActorTick.bCanEverTick = true;

	bAutoDestroyOnDie = true;
}

EDataValidationResult AActionPortfolioCharacter::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult SuperResult = Super::IsDataValid(ValidationErrors);
	EDataValidationResult Result = EDataValidationResult::Valid;

	for (int Idx = 0; Idx < StartupAbilities.Num(); Idx++)
	{
		if (!StartupAbilities[Idx])
		{
			ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("Idx {%d} Ability Class Is Null"), Idx)));
			Result = EDataValidationResult::Invalid;
		}
		
	}
	
	return SuperResult == EDataValidationResult::Valid ? Result : SuperResult;
}

void AActionPortfolioCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
}

UAbilitySystemComponent* AActionPortfolioCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}


void AActionPortfolioCharacter::CheckInAir()
{
	if(GetLocalRole() != ROLE_Authority) return;

	bool bIsInAirNew = GetCharacterMovement()->IsFalling() || GetCharacterMovement()->IsFlying();

	if (!(bIsInAirNew ^ IsInAir())) return;

	if (bIsInAirNew)
	{
		AbilitySystem->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.IsInAir"), 1);
	}
	else
	{
		AbilitySystem->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.IsInAir"), 1);
	}
}

bool AActionPortfolioCharacter::IsInAir() const
{
	return AbilitySystem->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.IsInAir"));
}

bool AActionPortfolioCharacter::IsDown() const
{
	return AbilitySystem->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.Down"));
}

void AActionPortfolioCharacter::AddStartupAbilities()
{
	if (GetLocalRole() != ROLE_Authority || bStartupAbilitiesGiven) return;
	bStartupAbilitiesGiven = true;

	for (auto& StartupAbility : StartupAbilities) {
		FGameplayAbilitySpecHandle TempHandle = AbilitySystem->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, -1, this));
	}
}

void AActionPortfolioCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || bStartupEffectsAdded) return;
	bStartupEffectsAdded = true;

	FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	//StartUp Effects 적용
	for (const TSubclassOf<UGameplayEffect>& GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystem->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystem->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		}
	}
}

void AActionPortfolioCharacter::InitializeAttributes()
{
	const UCharacterDataManager* DataManager = UCharacterDataManager::GetCharacterDataManager();

	const FCharacterAttribute* CharacterData = DataManager->GetCharacterData(GetCharacterCode());
	check(CharacterData);

	FGameplayEffectContextHandle ContextHandle = AbilitySystem->MakeEffectContext();
	
	FGameplayEffectSpec EffectSpec(GetDefault<UAttributeEffect_Base>(), ContextHandle, 1);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetCharacterLevelName(), CharacterData->Level);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetMaxHealthName(), CharacterData->MaxHealth);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetHealthRegenName(), CharacterData->HealthRegen);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetMaxStaminaName(), CharacterData->MaxStamina);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetStaminaRegenName(), CharacterData->StaminaRegen);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetAttackPName(), CharacterData->AttackP);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetDefensePName(), CharacterData->DefenseP);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetFireResistanceName(), CharacterData->FireResistance);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetIceResistanceName(), CharacterData->IceResistance);
	EffectSpec.SetSetByCallerMagnitude(UCharacterAttributeSet::GetElectricResistanceName(), CharacterData->ElectricResistance);

	AbilitySystem->ApplyGameplayEffectSpecToSelf(EffectSpec);
}




void AActionPortfolioCharacter::ClearStartupAbilities()
{
	if(GetLocalRole() != ROLE_Authority || !bStartupAbilitiesGiven) return;

	for (auto& StartupAbility : StartupAbilities)
	{
		AbilitySystem->ClearAbilityWithClass(StartupAbility);
	}
}

void AActionPortfolioCharacter::ClearStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !bStartupEffectsAdded) return;

	AbilitySystem->RemoveActiveEffects(
				FGameplayEffectQuery(
					FActiveGameplayEffectQueryCustomMatch::CreateUObject
					(this, &AActionPortfolioCharacter::ConditionRemoveStartupEffects) ) );
}

bool AActionPortfolioCharacter::ConditionRemoveStartupEffects(const FActiveGameplayEffect& ActiveEffect)
{
	const UGameplayEffect* InEffect =  ActiveEffect.Spec.Def.Get();

	if(InEffect == GetDefault<UCharacterRegenEffect>()) return true;

	for (auto& StartupEffectClass : StartupEffects)
	{
		if(InEffect == StartupEffectClass.GetDefaultObject()) return true;
	}

	return false;
}



int32 AActionPortfolioCharacter::GetCharacterLevel() const
{
	return AttributeSet->GetCharacterLevel();
}

float AActionPortfolioCharacter::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AActionPortfolioCharacter::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AActionPortfolioCharacter::GetHealthPercent() const
{
	return AttributeSet->GetHealth() / AttributeSet->GetMaxHealth();
}

bool AActionPortfolioCharacter::CanCharacterMove() const
{
	bool CanMove = true;
	FGameplayTagContainer TagCon;
	TagCon.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
	TagCon.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Down"));
	TagCon.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Rigidity"));
	TagCon.AddTag(FGameplayTag::RequestGameplayTag("State.Block.Move"));
	CanMove = !AbilitySystem->HasAnyMatchingGameplayTags(TagCon);
	return CanMove;
}


void AActionPortfolioCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		InitializeAttributes();
		AddStartupEffects();
		AddStartupAbilities();
	}

	InitializeMovement();
}

void AActionPortfolioCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckInAir();
}

void AActionPortfolioCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}


void AActionPortfolioCharacter::Landed(const FHitResult& Hit)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		FGameplayEventData Payload;
		Payload.Instigator = this;

		FGameplayEffectContextHandle EventContextHandle = AbilitySystem->MakeEffectContext();
		EventContextHandle.AddHitResult(Hit, true);
		Payload.ContextHandle = EventContextHandle;

		AbilitySystem->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("CommonEvent.OnLanded"), &Payload);
	}

	Super::Landed(Hit);
}

USceneComponent* AActionPortfolioCharacter::GetDefaultAttachComponent() const
{
	return GetMesh();
}

void AActionPortfolioCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamId = NewTeamID;
}

FGenericTeamId AActionPortfolioCharacter::GetGenericTeamId() const
{
	return TeamId;
}

void AActionPortfolioCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AActionPortfolioCharacter::InitializeMovement()
{
	bDefaultOrientRotationToMovement = GetCharacterMovement()->bOrientRotationToMovement;
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	DefaultMaxAcceleration = GetCharacterMovement()->MaxAcceleration;
	DefaultGravityScale = GetCharacterMovement()->GravityScale;
}

void AActionPortfolioCharacter::ResetMovement()
{
	GetCharacterMovement()->bOrientRotationToMovement = bDefaultOrientRotationToMovement;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetCharacterMovement()->bForceMaxAccel = false;
	GetCharacterMovement()->MaxAcceleration = DefaultMaxAcceleration;
	GetCharacterMovement()->GravityScale = DefaultGravityScale;
}

bool AActionPortfolioCharacter::CanBasicAct() const
{
	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
	CheckTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Down"));
	CheckTags.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Rigidity"));

	return !GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(CheckTags);
}

UActionPFAnimInstance* AActionPortfolioCharacter::GetAnimInstance() const
{
	return Cast<UActionPFAnimInstance>( GetMesh()->GetAnimInstance());
}


void AActionPortfolioCharacter::DestroyCharacter()
{
	Destroy();
}


void AActionPortfolioCharacter::CharacterDie(const FGameplayEffectModCallbackData& Data)
{
	if(IsCharacterDie()) return;

	OnCharacterDie(Data);

	const FGameplayTag DeathTag = FGameplayTag::RequestGameplayTag("State.Etc.Death");
	AbilitySystem->AddLooseGameplayTag(DeathTag);

	FGameplayEventData EventData;
	EventData.ContextHandle = Data.EffectSpec.GetEffectContext();
	EventData.Instigator = this;
	EventData.Target = this;
	EventData.EventTag = DeathTag;
	AbilitySystem->HandleGameplayEvent(DeathTag, &EventData);

	UActionPFAnimInstance* AnimInstance = GetAnimInstance();
	if (UAnimMontage* DeathAnim = AnimInstance->GetAnimMontageByTag("Animation.Death"))
	{
		AnimInstance->Montage_Play(DeathAnim);
	}


	if(OnCharacterDieDelegate.IsBound())
	{
		OnCharacterDieDelegate.Broadcast(this);
	}

	if (bAutoDestroyOnDie)
	{
		if (0 < DestroyTime)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AActionPortfolioCharacter::DestroyCharacter, DestroyTime, false);
		}
		else
		{
			DestroyCharacter();
		}
	}
}


bool AActionPortfolioCharacter::IsCharacterDie() const
{
	return AbilitySystem->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
}

