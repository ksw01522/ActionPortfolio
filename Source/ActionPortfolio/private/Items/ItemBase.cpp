// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "Ability/ActionPFAbilityBFL.h"

#include "Items/Euipment/EquipmentAbility.h"
#include "Ability/ActionPFAttributeSet.h"

#include "Items/ItemManagerSubsystem.h"
#include "Items/ItemWorldSubsystem.h"

#include "Components/BoxComponent.h"

#include "ActionPortfolio.h"

//비동기 로드를 위한 헤더
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

#include "Curves/CurveVector.h"

#include "Character/Player/ActionPFPlayerController.h"


void UItemBase::InitializeItem(const FName& NewItemCode, const FItemData_Base* Data)
{
	if(Data == nullptr || NewItemCode.IsNone()) return;
	if (bInitialized) {
		PFLOG(Error, TEXT("Already Initialized Item"));
		return;
	}

	ItemCode = NewItemCode;
	ItemType = Data->ItemType;
	Grade = Data->Grade;
	Name = Data->Name;
	Description = Data->Description;
	Icon = Data->Icon;
	ItemMesh = Data->ItemMesh;
	Price = Data->Price;
	bStackable = Data->bStackable;
	StackSize = Data->StackSize;

	if(!bStackable) Count = 1;

	bInitialized = true;
}

void UItemBase::AddCount(int InCount)
{
	ensure(0 < InCount);
	Count = FMath::Max(StackSize, Count + InCount);
}

void UItemBase::RemoveCount(int InCount)
{
	ensure(InCount < 0);
	Count = FMath::Max(0, Count + InCount);
}

void UItemBase::SetCount(int InCount)
{
	check(0 <= InCount);
	Count = FMath::Clamp(InCount, 0, StackSize);
}

bool UItemBase::CanStackableWithOther(const UItemBase* Target) const
{
	bool bResult = Target != nullptr && bStackable &&Target->GetItemCode() == GetItemCode();
	return bResult;
}

bool UItemBase::IsSame(const UItemBase* Other) const
{
	if(Other == nullptr) return false;

	return GetItemCode() == Other->GetItemCode();
}


bool UItemBase_Equipment::CanEquipItem(UActionPFAbilitySystemComponent* ASC) const
{
	return EquipmentAbility.GetDefaultObject()->DoesAbilitySatisfyTagRequirements(*ASC);
}

bool UItemBase_Equipment::IsSame(const UItemBase* Other) const
{
	return this == Other;
}


UGameplayEffect* UItemBase_Equipment::MakeAddStatusEffect() const
{
	//기본 스테이터스 증가 Effect 만들기
	//기본 설정
	UGameplayEffect* AddStatusEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("AddStatusEffect")));
	AddStatusEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	AddStatusEffect->Modifiers.SetNum(4);

	//MaxHP설정
	FGameplayModifierInfo& MaxHPInfo = AddStatusEffect->Modifiers[0];
	MaxHPInfo.Attribute = UActionPFAttributeSet::GetMaxHealthAttribute();
	MaxHPInfo.ModifierOp = EGameplayModOp::Additive;
	MaxHPInfo.ModifierMagnitude = FScalableFloat(MaxHP);

	//MaxStamina
	FGameplayModifierInfo& MaxStaminaInfo = AddStatusEffect->Modifiers[1];
	MaxStaminaInfo.Attribute = UActionPFAttributeSet::GetMaxStaminaAttribute();
	MaxStaminaInfo.ModifierOp = EGameplayModOp::Additive;
	MaxStaminaInfo.ModifierMagnitude = FScalableFloat(MaxStamina);

	//AttackP
	FGameplayModifierInfo& AttackPInfo = AddStatusEffect->Modifiers[2];
	AttackPInfo.Attribute = UActionPFAttributeSet::GetAttackPAttribute();
	AttackPInfo.ModifierOp = EGameplayModOp::Additive;
	AttackPInfo.ModifierMagnitude = FScalableFloat(AttackP);

	//DefenseP
	FGameplayModifierInfo& DefensePInfo = AddStatusEffect->Modifiers[3];
	DefensePInfo.Attribute = UActionPFAttributeSet::GetDefensePAttribute();
	DefensePInfo.ModifierOp = EGameplayModOp::Additive;
	DefensePInfo.ModifierMagnitude = FScalableFloat(DefenseP);

	return AddStatusEffect;
}


void UItemBase_Equipment::InitializeItem(const FName& NewItemCode, const FItemData_Base* Data)
{
	if (Data == nullptr || NewItemCode.IsNone()) return;
	if (bInitialized) {
		PFLOG(Error, TEXT("Already Initialized Item"));
		return;
	}
	const FItemData_Equipment* EquipmentData = static_cast<const FItemData_Equipment*>(Data);
	if (EquipmentData == nullptr)
	{
		PFLOG(Error, TEXT("EquipmentData Can't Initialized By FItemData_Base"));
		return;
	}

	Super::InitializeItem(NewItemCode, Data);

	EquipmentAbility = EquipmentData->EquipmentAbility;

	EquipmentPart = EquipmentData->EquipmentPart;

	MaxHP = EquipmentData->MaxHP;

	MaxStamina = EquipmentData->MaxStamina;

	AttackP = EquipmentData->AttackP;

	DefenseP = EquipmentData->DefenseP;
}


///////////////////////////////////////// 드랍 아이템 ///////////////////////////////////
ADroppedItem::ADroppedItem() : DroppedItem(nullptr), MagnetizedTarget(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(FName("Box Collider"));
	RootComponent = BoxCollider;

	//Collision 프리셋 만들어야함
	BoxCollider->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	BoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	BoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	BoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	BoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	BoxCollider->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	BoxCollider->BodyInstance.bLockXRotation = true;
	BoxCollider->BodyInstance.bLockYRotation = true;
	BoxCollider->SetCanEverAffectNavigation(false);

	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Item Mesh"), true);
	ItemMeshComponent->SetupAttachment(BoxCollider);
	ItemMeshComponent->SetRelativeLocation(FVector(0,0, 60));
	ItemMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	ItemMeshComponent->SetGenerateOverlapEvents(false);
	ItemMeshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	ItemMeshComponent->SetCanEverAffectNavigation(false);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ADroppedItem::OnBeginOverlapColliderWithOther);

}

void ADroppedItem::BeginPlay()
{
	Super::BeginPlay();

	//Bounce Timeline 초기화
	{
		float MinBounceTime, MaxBounceTime;
		BounceCurve->GetTimeRange(MinBounceTime, MaxBounceTime);

		FOnTimelineEvent BounceFinishedEvent;
		BounceFinishedEvent.BindUFunction(this, FName("MagnetizedEvent"));

		FOnTimelineFloat BounceEvent;
		BounceEvent.BindUFunction(this, FName("BounceTimelineFunction"));
		BounceTimeline.AddInterpFloat(BounceCurve, BounceEvent);
		BounceTimeline.SetTimelineLength(MaxBounceTime);
		BounceTimeline.SetTimelineFinishedFunc(BounceFinishedEvent);
	}
	
	//Magnetized Timeline 초기화
	{
		float MinMagnetizedTime, MaxMagnetizedTime;

		FOnTimelineEvent MagnetizedFinishedEvent;
		MagnetizedFinishedEvent.BindUFunction(this, FName("MagnetizedComplete"));

		FOnTimelineFloat DefaultMagnetizedEvent;
		DefaultMagnetizedEvent.BindUFunction(this, FName("MagnetizedTimelineFunction"));
		MagnetizedTimeline.AddInterpFloat(MangetizedCurve, DefaultMagnetizedEvent);

		MangetizedCurve->GetTimeRange(MinMagnetizedTime, MaxMagnetizedTime);
		MagnetizedTimeline.SetTimelineLength(MaxMagnetizedTime);

		MagnetizedTimeline.SetTimelineFinishedFunc(MagnetizedFinishedEvent);
	}
}

void ADroppedItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BounceTimeline.TickTimeline(DeltaTime);
	MagnetizedTimeline.TickTimeline(DeltaTime);
}

void ADroppedItem::OnBeginOverlapColliderWithOther(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PFLOG(Warning, TEXT("On Begin Overlap DroppedItem"));

	if(MagnetizedTarget.IsValid() || !OtherActor->ActorHasTag(FName("PlayerCharacter")) ) 
	{
		return;
	}

	PFLOG(Warning, TEXT("On Begin Overlap DroppedItem With Player"));

	MagnetizedTarget = Cast<APawn>(OtherActor);

	BounceItem();
}

void ADroppedItem::SetDICollisionActivate(bool NewState)
{
	ECollisionEnabled::Type NewCollisionEnabled = NewState ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;

	//꺼야할 시 먼저
	if (!NewState) BoxCollider->SetSimulatePhysics(NewState);

	BoxCollider->SetCollisionEnabled(NewCollisionEnabled);

	//켜야할 시 이후에
	if (NewState) BoxCollider->SetSimulatePhysics(NewState);
}

void ADroppedItem::CancelMeshStreaming()
{
	if (MeshStreamingHandle.IsValid())
	{
		MeshStreamingHandle->CancelHandle();
		MeshStreamingHandle.Reset();
	}
}



void ADroppedItem::SetDroppedItem(UItemBase* NewData)
{
	CancelMeshStreaming();

	if (NewData == nullptr)
	{
		ClearDroppedItemData();
		return;
	}

	DroppedItem = NewData;

	if (DroppedItem->GetItemMesh().IsValid())
	{
		OnReadyDropItem();
	}
	else
	{
		TWeakObjectPtr<ADroppedItem> WeakThis(this);

		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		MeshStreamingHandle = Streamable.RequestAsyncLoad(DroppedItem->GetItemMesh().ToSoftObjectPath(), FStreamableDelegate::CreateLambda([WeakThis, NewData]{
			if (!WeakThis.IsValid() || WeakThis->GetDroppedItem() != NewData) return;
			
			WeakThis->OnReadyDropItem();
			WeakThis->MeshStreamingHandle.Reset();
		}));
	}
}



void ADroppedItem::ClearDroppedItemData()
{
	CancelMeshStreaming();

	DroppedItem = nullptr;

	ItemMeshComponent->SetStaticMesh(nullptr);

	GetWorld()->GetSubsystem<UItemWorldSubsystem>()->BackToPool(*this);
}




void ADroppedItem::SetActorHiddenInGame(bool bNewHidden)
{
	if (IsHidden() == bNewHidden) return;

	Super::SetActorHiddenInGame(bNewHidden);

	SetActorEnableCollision(!bNewHidden);
	SetDICollisionActivate(!bNewHidden);
	SetActorTickEnabled(!bNewHidden);
	
}

bool ADroppedItem::IsValidDropItem() const
{
	return DroppedItem != nullptr;
}

void ADroppedItem::SetMagnetizedTarget(APawn* NewTarget)
{
	if (NewTarget == nullptr)
	{
		MagnetizedTarget.Reset();
	}
	else
	{
		MagnetizedTarget = NewTarget;
	}
}

void ADroppedItem::OnReadyDropItem()
{
	if(!IsValidDropItem()){ return; }

	ItemMeshComponent->SetStaticMesh(DroppedItem->GetItemMesh().Get());

	SetActorHiddenInGame(false);

}

void ADroppedItem::BounceItem()
{
	PFLOG(Warning, TEXT("Call Bounce"));

	if (!MagnetizedTarget.IsValid())
	{
		PFLOG(Error, TEXT("Can't Find Valid MagnetizedTarget."));
		return;
	}

	SetDICollisionActivate(false);

	BounceStartLocation = GetActorLocation();

	UItemManagerSubsystem* ManagerInstance = UItemManagerSubsystem::GetManagerInstance();
	FVector Offset = GetActorLocation() - MagnetizedTarget->GetActorLocation();
	Offset.Z = 0;

	if (Offset.IsNearlyZero())	{ Offset = MagnetizedTarget->GetActorForwardVector(); }
	else						{ Offset.Normalize(); }

	Offset *= BouncePower;
	Offset.Z = BounceHeight;

	BounceGoalLocation = GetActorLocation() + Offset;

	BounceTimeline.PlayFromStart();
}

void ADroppedItem::BounceTimelineFunction(float Output)
{
	FVector NewLocation = FMath::Lerp(BounceStartLocation, BounceGoalLocation, Output);
	SetActorLocation(NewLocation);
}

void ADroppedItem::MagnetizedEvent()
{
	PFLOG(Warning, TEXT("Call Magnetized"));
	MagnetizedTimeline.PlayFromStart();
}

void ADroppedItem::MagnetizedTimelineFunction(float Output)
{
	if (MagnetizedTarget.IsValid())
	{
		FVector NewLocation = FMath::Lerp(BounceGoalLocation, MagnetizedTarget->GetActorLocation(), Output);
		SetActorLocation(NewLocation);
	}
	else
	{
		MagnetizedCancel();
	}
}

void ADroppedItem::MagnetizedCancel()
{
	BounceTimeline.Stop();
	MagnetizedTimeline.Stop();
	MagnetizedTarget.Reset();
	SetDICollisionActivate(true);
}

void ADroppedItem::MagnetizedComplete()
{
	SetMagnetizedTarget(nullptr);

	if (IsValidDropItem())
	{
		SetDICollisionActivate(true);
	}
	else
	{
		
	}

	BounceTimeline.Stop();
	MagnetizedTimeline.Stop();

	AActionPFPlayerController* PlayerController = MagnetizedTarget->GetController<AActionPFPlayerController>();
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->PickUpItem(this);
}

void ADroppedItem::SetItemCount(int NewCount)
{
	DroppedItem->SetCount(NewCount);
}
