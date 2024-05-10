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

#include "Ability/Ability/Ability_Rigidity.h"

#include "Character/LockOnStateWidget.h"
#include "LockOn/LockOnTargetComponent.h"

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
	AttributeSet = CreateDefaultSubobject<UActionPFAttributeSet>(TEXT("AttributeSet"));



	PrimaryActorTick.bCanEverTick = true;

	bDestroyOnDie = true;
}

EDataValidationResult AActionPortfolioCharacter::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult SuperResult = Super::IsDataValid(ValidationErrors);
	EDataValidationResult Result = EDataValidationResult::Valid;

	for (int Idx = 0; Idx < CharacterAbilities.Num(); Idx++)
	{
		if (!CharacterAbilities[Idx].Class)
		{
			ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("Idx {%d} Ability Class Is Null"), Idx)));
			Result = EDataValidationResult::Invalid;
		}
		
	}
	
	return SuperResult == EDataValidationResult::Valid ? Result : SuperResult;
}

UAbilitySystemComponent* AActionPortfolioCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}


void AActionPortfolioCharacter::CheckInAir()
{
	bool bIsInAirNew = GetCharacterMovement()->IsFalling() || GetCharacterMovement()->IsFlying();

	if (!(bIsInAirNew ^ IsInAir())) return;

	if (bIsInAirNew)
	{
		AbilitySystem->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.IsInAir"));
	}
	else
	{
		AbilitySystem->SetLooseGameplayTagCount(FGameplayTag::RequestGameplayTag("State.Etc.IsInAir"), 0);
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

void AActionPortfolioCharacter::InitializeAttributes()
{
	ensure(IsValid(AbilitySystem));

	if (!DefaultAttributes)
	{
		PFLOG(Warning, TEXT("Can't find DefaultAttributes for %s. fill DefaultAttributes."), *GetName());
		return;
	}

	UGameplayEffect* NewAttributeEffect = NewObject<UGameplayEffect>(this, DefaultAttributes);

	AttributeSet->InitializeStatus(1, NewAttributeEffect);
}

void AActionPortfolioCharacter::AddCharacterAbilities()
{
	if(AbilitySystem->bCharacterAbilitiesGiven) return;

	for (auto& AbilityStruct : CharacterAbilities) {
		FGameplayAbilitySpecHandle TempHandle = AbilitySystem->GiveAbility(FGameplayAbilitySpec(AbilityStruct.Class, AbilityStruct.Level, -1, this));
		if(TempHandle.IsValid()) OnAddedAbility(TempHandle);
	}

	AbilitySystem->bCharacterAbilitiesGiven = true;
}

void AActionPortfolioCharacter::AddStartupEffects()
{
	if(AbilitySystem->bStartupEffectsApplied) return;


	FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystem->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystem->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystem);
		}
	}

	AbilitySystem->bStartupEffectsApplied = true;
}

void AActionPortfolioCharacter::ClearCharacterAbilities()
{
	if(GetLocalRole() != ROLE_Authority || !AbilitySystem->bCharacterAbilitiesGiven) return;

	for (auto& CharAbilityClass : CharacterAbilities)
	{
		AbilitySystem->ClearAbilityWithClass(CharAbilityClass.Class);
	}

	AbilitySystem->bCharacterAbilitiesGiven = false;
}

void AActionPortfolioCharacter::OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	//PFLOG(Warning, TEXT("수정필요"));
}

void AActionPortfolioCharacter::OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved)
{
	//PFLOG(Warning, TEXT("수정필요"));

}

void AActionPortfolioCharacter::InitializeAttributeChangedDelegate()
{
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AActionPortfolioCharacter::OnHealthChanged);
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AActionPortfolioCharacter::OnMaxHealthChanged);
}

void AActionPortfolioCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	//PFLOG(Warning, TEXT("수정필요"));
}

void AActionPortfolioCharacter::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	//PFLOG(Warning, TEXT("수정필요"));

}

void AActionPortfolioCharacter::InitializeAbilitySystem()
{
	AbilitySystem->InitAbilityActorInfo(this, this);

	AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &AActionPortfolioCharacter::OnActiveGameplayEffectAddedCallback);
	AbilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &AActionPortfolioCharacter::OnRemoveGameplayEffectCallback);

	InitializeAttributeChangedDelegate();

	InitializeAttributes();
	AddStartupEffects();
	AddCharacterAbilities();
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

	InitializeAbilitySystem();
	InitializeMovement();

	if (LockOnStateWidgetClass)
	{
		LockOnStateWidget = CreateWidget<ULockOnStateWidget>(GetWorld(), LockOnStateWidgetClass, "LockOnStateWidget");
	}
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
	FGameplayEventData Payload;
	Payload.Instigator = this;
	
	AbilitySystem->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("CommonEvent.OnLanded"), &Payload);

	Super::Landed(Hit);
}

FGenericTeamId AActionPortfolioCharacter::GetGenericTeamId() const
{
	IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(GetController());
	if (TeamAgent) {
		return TeamAgent->GetGenericTeamId();
	}
	else {
		return FGenericTeamId::NoTeam;
	}
}

void AActionPortfolioCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AActionPFPlayerController* PlayerController = Cast<AActionPFPlayerController>(NewController))
	{
		SetGenericTeamId(1);
	}
	else
	{
		SetGenericTeamId(2);
	}

	AbilitySystem->InitAbilityActorInfo(this, this);
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

void AActionPortfolioCharacter::AttachComponentToCharacter(USceneComponent* InComponent, const FAttachmentTransformRules& AttachmentRules, const FName& SocketName)
{
	InComponent->AttachToComponent(GetMesh(), AttachmentRules, SocketName);
}

UAnimMontage* AActionPortfolioCharacter::GetRigidityAnim(float RigidityTime, const FHitResult* HitResult) const
{
	if (HitResult != nullptr)
	{
	}

	return HitReactionAnimations.Front;
}



void AActionPortfolioCharacter::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bDestroyOnDie && !bInterrupted) {
		Destroy();
	}
}

void AActionPortfolioCharacter::CharacterDie()
{
	if(IsCharacterDie()) return;

	const FGameplayTag DeathTag = FGameplayTag::RequestGameplayTag("State.Etc.Death");
	AbilitySystem->AddLooseGameplayTag(DeathTag);

	FGameplayEventData EventData;
	EventData.Instigator = this;
	EventData.Target = this;
	EventData.EventTag = DeathTag;
	AbilitySystem->HandleGameplayEvent(DeathTag, &EventData);

	if(OnCharacterDie.IsBound())
	{
		OnCharacterDie.Broadcast(this);
		OnCharacterDie.Clear();
	}
	
	if (HitReactionAnimations.Death != nullptr)
	{
		FOnMontageEnded DeathMontageEnded;
		DeathMontageEnded.BindUObject(this, &AActionPortfolioCharacter::OnDeathMontageEnded);

		GetMesh()->GetAnimInstance()->Montage_Play(HitReactionAnimations.Death);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(DeathMontageEnded, HitReactionAnimations.Death);
	}
	else if(bDestroyOnDie)
	{
		Destroy();
	}
	
}

void AActionPortfolioCharacter::OnDamageEvent(float DamageAmount, AActor* DamageInstigator)
{
	if (OnDamagedDel.IsBound())
	{
		OnDamagedDel.Broadcast(DamageAmount, DamageInstigator);
	}
}

void AActionPortfolioCharacter::OnAttackEvent(float DamageAmount, AActor* Target)
{
	
}


bool AActionPortfolioCharacter::IsCharacterDie() const
{
	return AbilitySystem->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
}

