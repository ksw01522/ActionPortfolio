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

UAbilitySystemComponent* AActionPortfolioCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}


void AActionPortfolioCharacter::CheckInAir()
{
	bool NewState = GetCharacterMovement()->IsFalling();
	if (!(NewState ^ IsInAirHandle.IsValid())) return;

	if (NewState)
	{
		UGameplayEffect* InAirEffect = NewObject<UGameplayEffect>(GetTransientPackage(), "IsInAir");
		InAirEffect->InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.IsInAir"));
		InAirEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;

		FGameplayEffectContextHandle Context = AbilitySystem->MakeEffectContext();
		FGameplayEffectSpec Spec(InAirEffect, Context);
		IsInAirHandle = AbilitySystem->ApplyGameplayEffectSpecToSelf(Spec);
	}
	else
	{
		AbilitySystem->RemoveActiveGameplayEffect(IsInAirHandle);
		IsInAirHandle.Invalidate();
	}
}

bool AActionPortfolioCharacter::IsInAir() const
{
	return IsInAirHandle.IsValid();
}

bool AActionPortfolioCharacter::IsDown() const
{
	return DownHandle.IsValid();
}

void AActionPortfolioCharacter::SetDown(bool NewState)
{
	if (!(NewState ^ DownHandle.IsValid())) return;

	if (NewState)
	{
		UGameplayEffect* DownEffect = NewObject<UGameplayEffect>(GetTransientPackage(), "Down");
		DownEffect->InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Down"));
		DownEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;

		FGameplayEffectContextHandle Context = AbilitySystem->MakeEffectContext();
		FGameplayEffectSpec Spec(DownEffect, Context);
		DownHandle = AbilitySystem->ApplyGameplayEffectSpecToSelf(Spec);
	}
	else
	{
		AbilitySystem->RemoveActiveGameplayEffect(DownHandle);
		DownHandle.Invalidate();
	}
}

void AActionPortfolioCharacter::InitializeAttributes()
{
	ensure(IsValid(AbilitySystem));

	if (!DefaultAttributes)
	{
		PFLOG(Warning, TEXT("Can't find DefaultAttributes for %s. fill DefaultAttributes."), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	

	FGameplayEffectSpecHandle NewHandle = AbilitySystem->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystem->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystem);
	}
}

void AActionPortfolioCharacter::AddCharacterAbilities()
{
	if(AbilitySystem->bCharacterAbilitiesGiven) return;

	for (auto Ability : CharacterAbilities) {
		AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability, 1, -1, this) ) ;
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

void AActionPortfolioCharacter::RigidityTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (0 < NewCount)
	{
		
	}
	else
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying( LastHitReactAnim.Get() ) )
		{
			GetMesh()->GetAnimInstance()->Montage_Stop(0.3, LastHitReactAnim.Get());
		}
		else if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(HitReactionAnimations.Down))
		{
			GetMesh()->GetAnimInstance()->Montage_Play(HitReactionAnimations.DownedRecovery);

			DownRecoveryMontageEndedDelegate.BindUObject(this, &AActionPortfolioCharacter::OnDownRecoveryMontageEnded);
			GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(DownRecoveryMontageEndedDelegate, HitReactionAnimations.DownedRecovery);
		}
	}
}


void AActionPortfolioCharacter::OnDownRecoveryMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	SetDown(false);
}

void AActionPortfolioCharacter::InitializeAbilitySystem()
{
	AbilitySystem->InitAbilityActorInfo(this, this);

	AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &AActionPortfolioCharacter::OnActiveGameplayEffectAddedCallback);
	AbilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &AActionPortfolioCharacter::OnRemoveGameplayEffectCallback);

	AbilitySystem->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("State.Etc.Rigidity")).AddUObject(this, &AActionPortfolioCharacter::RigidityTagChanged);

	InitializeAttributeChangedDelegate();
	InitializeAttributes();
	AddStartupEffects();
	AddCharacterAbilities();

}

int32 AActionPortfolioCharacter::GetCharacterLevel() const
{
	return static_cast<int32>(AttributeSet->GetCharacterLevel());
}

float AActionPortfolioCharacter::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AActionPortfolioCharacter::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

bool AActionPortfolioCharacter::CanCharacterMove() const
{
	bool CanMove = true;
	FGameplayTagContainer TagCon;
	TagCon.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
	TagCon.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Down"));
	TagCon.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Rigidity"));
	TagCon.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.BlockMove"));
	CanMove = !AbilitySystem->HasAnyMatchingGameplayTags(TagCon);
	return CanMove;
}

void AActionPortfolioCharacter::RemoveRigidityHandle()
{
	if (!RigidityHandle.IsValid()) {
		return;
	}

	AbilitySystem->RemoveActiveGameplayEffect(RigidityHandle);
	RigidityHandle.Invalidate();
	RigidityTime = 0;
	FGameplayEventData Payload;
	Payload.Instigator = this;
	Payload.EventTag = FGameplayTag::RequestGameplayTag("State.Etc.Rigidity.Recovery");
	AbilitySystem->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("State.Etc.Rigidity.Recovery"), &Payload);
}

void AActionPortfolioCharacter::CharacterRigidity(float NewRigidityTime)
{
	if (NewRigidityTime <= 0 || IsCharacterDie()) return;


	if (!RigidityHandle.IsValid()) {
		UGameplayEffect* RigidityEffect = NewObject<UGameplayEffect>(GetTransientPackage(), "Rigidity");
		RigidityEffect->InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("State.Etc.Rigidity"));
		RigidityEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;

		FGameplayEffectContextHandle Context = AbilitySystem->MakeEffectContext();
		FGameplayEffectSpec Spec(RigidityEffect, Context);
		RigidityHandle = AbilitySystem->ApplyGameplayEffectSpecToSelf(Spec);

		FGameplayEventData Payload;
		Payload.Instigator = this;
		Payload.Target = this;
		Payload.EventTag = FGameplayTag::RequestGameplayTag("State.Etc.Rigidity");
		AbilitySystem->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("State.Etc.Rigidity"), &Payload);
		RigidityTime = NewRigidityTime;

	}
	else {
		RigidityTime = NewRigidityTime;
	}
}

bool AActionPortfolioCharacter::IsRigidity() const
{
	return AbilitySystem->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.Rigidity"));
}

void AActionPortfolioCharacter::RecoveryHitReaction()
{
	if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(LastHitReactAnim.Get())) { return; }

	GetMesh()->GetAnimInstance()->Montage_Stop(0, LastHitReactAnim.Get());

}


void AActionPortfolioCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	InitializeAbilitySystem();
	InitForAttackShape();
	InitializeMovement();
}

void AActionPortfolioCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckInAir();

	if (RigidityHandle.IsValid()) {
		RigidityTime -= DeltaSeconds;

		if (IsInAir())
		{
			RigidityTime = FMath::Max<float>(RigidityTime, 1);
		}

		if (RigidityTime <= 0.0f) {
			RemoveRigidityHandle();
		}
	}
}

void AActionPortfolioCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AActionPortfolioCharacter::InitForAttackShape()
{
	for (auto TagName : AttackShapeTags)
	{
		TArray<UActorComponent*> AttackShapes = GetComponentsByTag(UShapeComponent::StaticClass(), TagName);

		if (AttackShapes.IsEmpty()) {
			PFLOG(Warning, TEXT("Can't find AttackShape by tag from %s"), *UKismetSystemLibrary::GetDisplayName(this));
		}
		else {
			if (2 <= AttackShapes.Num()) {
				PFLOG(Warning, TEXT("Find Multi AttackShapes by tag from %s"), *UKismetSystemLibrary::GetDisplayName(this));
			}

			AttackShapeMap.Add(TagName, Cast<UShapeComponent>(AttackShapes[0]));
		}
	}
}

UShapeComponent* AActionPortfolioCharacter::GetAttackShape(FName AttackShapeTag)
{
	if (!AttackShapeMap.Contains(AttackShapeTag)) {
		PFLOG(Warning, TEXT("Can't find Attackshpae by Tag"));
		return nullptr;
	}

	UShapeComponent* AttackShape = *AttackShapeMap.Find(AttackShapeTag);
	return AttackShape;
}

void AActionPortfolioCharacter::ActivateActionPFAbility(TSubclassOf<class UActionPFGameplayAbility> AbilityClass)
{
	AbilitySystem->TryActivatePFAbilityByClass(AbilityClass);

	
}



void AActionPortfolioCharacter::Landed(const FHitResult& Hit)
{
	if (AbilitySystem->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.Rigidity")))
	{
		SetDown(true);
		GetMesh()->GetAnimInstance()->Montage_Play(HitReactionAnimations.Down);
	}

	FGameplayEventData Payload;
	AbilitySystem->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Ability.PublicEvent.OnLand"), &Payload);

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




void AActionPortfolioCharacter::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bDestroyOnDie) {
		Destroy();
	}
}

void AActionPortfolioCharacter::CharacterDie()
{
	FGameplayTag DeathTag = FGameplayTag::RequestGameplayTag("State.Etc.Death");

	UGameplayEffect* DeathEffect = NewObject<UGameplayEffect>();
	DeathEffect->InheritableOwnedTagsContainer.AddTag(DeathTag);
	DeathEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	FGameplayEffectContextHandle Context = AbilitySystem->MakeEffectContext();
	FGameplayEffectSpec Spec(DeathEffect, Context);
	AbilitySystem->ApplyGameplayEffectSpecToSelf(Spec);

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
	AActionPortfolioCharacter* Char = Cast<AActionPortfolioCharacter>(Target);

	if (IsValid(Char)) {
		LastAttackedTarget = Char;
	}
}

void AActionPortfolioCharacter::HitReact(EHitReactionDirection Direction, bool bForceDown, UAnimMontage* ForceHitReactionAnim)
{
	if(!IsRigidity() || IsCharacterDie()) return;

	LastHitReactAnim = ForceHitReactionAnim;

	if (ForceHitReactionAnim != nullptr) {
		GetMesh()->GetAnimInstance()->Montage_Play(ForceHitReactionAnim);
		return;
	}

	bool bIsInAir = bForceDown || GetCharacterMovement()->IsFalling();
	switch (Direction)
	{
		case EHitReactionDirection::Front:
			LastHitReactAnim = bIsInAir ? HitReactionAnimations.InAir_Front : HitReactionAnimations.Ground_Front;
			break;

		case EHitReactionDirection::Back:
			LastHitReactAnim = bIsInAir ? HitReactionAnimations.InAir_Back : HitReactionAnimations.Ground_Back;
			break;

		case EHitReactionDirection::Right:
			LastHitReactAnim = bIsInAir ? HitReactionAnimations.InAir_Right : HitReactionAnimations.Ground_Right;
			break;

		case EHitReactionDirection::Left:
			LastHitReactAnim = bIsInAir ? HitReactionAnimations.InAir_Left : HitReactionAnimations.Ground_Left;
			break;
	}

	GetMesh()->GetAnimInstance()->Montage_Play(LastHitReactAnim.Get());
}

bool AActionPortfolioCharacter::IsCharacterDie() const
{
	return AbilitySystem->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Etc.Death"));
}

