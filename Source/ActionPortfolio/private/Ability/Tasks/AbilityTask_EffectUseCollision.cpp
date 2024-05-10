// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Tasks/AbilityTask_EffectUseCollision.h"
#include "Components/CapsuleComponent.h"
#include "Ability/ActionPFAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Ability/ActionPFGameplayAbility.h"
#include "Ability/CustomAbilityHelperInterface.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "ActionPortfolio.h"

UAbilityTask_EffectUseCollision::UAbilityTask_EffectUseCollision(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}


UAbilityTask_EffectUseCollision* UAbilityTask_EffectUseCollision::EffectUseCollision(UGameplayAbility* OwningAbility,FName TaskInstanceName,TArray<TSubclassOf<UGameplayEffect>> EffectClasses,TArray<UGameplayEffect*> Effects)
{
	UAbilityTask_EffectUseCollision* ReturnTask = NewAbilityTask<UAbilityTask_EffectUseCollision>(OwningAbility, TaskInstanceName);

	ReturnTask->AddGameplayEffects(EffectClasses);
	ReturnTask->AddGameplayEffects(Effects);

	return ReturnTask;
}

void UAbilityTask_EffectUseCollision::Activate()
{
	EffectShape->SetGenerateOverlapEvents(true);

#if WITH_EDITOR
	EffectShape->SetHiddenInGame(false);
#endif

	if(0 < LifeTime) bRemoveAfterLifeTime = true;
}



void UAbilityTask_EffectUseCollision::OnDestroy(bool AbilityEnded)
{
	OnTaskEnd.ExecuteIfBound(AbilityEnded);

	if (EffectShape != nullptr)
	{
		EffectShape->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		EffectShape->DestroyComponent();
	}

	Super::OnDestroy(AbilityEnded);
}


void UAbilityTask_EffectUseCollision::TickTask(float DeltaTime)
{
	if (bRemoveAfterLifeTime)
	{
		LifeTime -= DeltaTime;
		if (LifeTime <= 0)
		{
			EndTask();
		}
	}
}

void UAbilityTask_EffectUseCollision::MakeCapsuleCollision(float Radius, float HalfHeight, const FName& InCollisionProfileName, const FName& ShapeName)
{
	EffectShape = NewObject<UCapsuleComponent>(Ability->GetAvatarActorFromActorInfo(), ShapeName);
	static_cast<UCapsuleComponent*>(EffectShape)->InitCapsuleSize(Radius, HalfHeight);
	EffectShape->SetCollisionProfileName(InCollisionProfileName, false);
	EffectShape->OnComponentBeginOverlap.AddDynamic(this, &UAbilityTask_EffectUseCollision::OnEffectShapeBeginOverlap);
	EffectShape->RegisterComponent();
	EffectShape->CreationMethod = EComponentCreationMethod::Instance;
	EffectShape->SetEnableGravity(false);
	EffectShape->SetGenerateOverlapEvents(false);
}

void UAbilityTask_EffectUseCollision::MakeBoxCollision(FVector InBoxExtent, const FName& InCollisionProfileName, const FName& ShapeName)
{
	EffectShape = NewObject<UBoxComponent>(Ability->GetAvatarActorFromActorInfo(), ShapeName);
	static_cast<UBoxComponent*>(EffectShape)->InitBoxExtent(InBoxExtent);
	EffectShape->SetCollisionProfileName(InCollisionProfileName, false);
	EffectShape->OnComponentBeginOverlap.AddDynamic(this, &UAbilityTask_EffectUseCollision::OnEffectShapeBeginOverlap);
	EffectShape->RegisterComponent();
	EffectShape->CreationMethod = EComponentCreationMethod::Instance;
	EffectShape->SetEnableGravity(false);
	EffectShape->SetGenerateOverlapEvents(false);

}

void UAbilityTask_EffectUseCollision::MakeSphereCollision(float Radius, const FName& InCollisionProfileName, const FName& ShapeName)
{
	EffectShape = NewObject<USphereComponent>(Ability->GetAvatarActorFromActorInfo(), ShapeName);
	static_cast<USphereComponent*>(EffectShape)->InitSphereRadius(Radius);
	EffectShape->SetCollisionProfileName(InCollisionProfileName, false);
	EffectShape->OnComponentBeginOverlap.AddDynamic(this, &UAbilityTask_EffectUseCollision::OnEffectShapeBeginOverlap);
	EffectShape->RegisterComponent();
	EffectShape->CreationMethod = EComponentCreationMethod::Instance;
	EffectShape->SetEnableGravity(false);
	EffectShape->SetGenerateOverlapEvents(false);

}

void UAbilityTask_EffectUseCollision::AddGameplayEffect(TSubclassOf<UGameplayEffect> InEffectClass)
{
	FGameplayEffectContextHandle EffectContextHandle = Ability->MakeEffectContext(GetAbilitySpecHandle(), Ability->GetCurrentActorInfo());
	EffectSpecHandles.Add(AbilitySystemComponent->MakeOutgoingSpec(InEffectClass, Ability->GetAbilityLevel(), EffectContextHandle));

}

void UAbilityTask_EffectUseCollision::AddGameplayEffect(UGameplayEffect* InEffect)
{
	FGameplayEffectContextHandle EffectContextHandle = Ability->MakeEffectContext(GetAbilitySpecHandle(), Ability->GetCurrentActorInfo());
	EffectSpecHandles.Add(FGameplayEffectSpecHandle(new FGameplayEffectSpec(InEffect, EffectContextHandle, Ability->GetAbilityLevel())));
}

void UAbilityTask_EffectUseCollision::AddGameplayEffects(TArray<TSubclassOf<UGameplayEffect>> InEffectClasses)
{
	FGameplayEffectContextHandle EffectContextHandle = Ability->MakeEffectContext(GetAbilitySpecHandle(), Ability->GetCurrentActorInfo());

	for (auto& EffectClass : InEffectClasses)
	{
		EffectSpecHandles.Add(AbilitySystemComponent->MakeOutgoingSpec(EffectClass, Ability->GetAbilityLevel(), EffectContextHandle));
	}
}

void UAbilityTask_EffectUseCollision::AddGameplayEffects(TArray<UGameplayEffect*> InEffects)
{
	FGameplayEffectContextHandle EffectContextHandle = Ability->MakeEffectContext(GetAbilitySpecHandle(), Ability->GetCurrentActorInfo());
	for (auto& InEffect : InEffects)
	{
		EffectSpecHandles.Add(FGameplayEffectSpecHandle(new FGameplayEffectSpec(InEffect, EffectContextHandle, Ability->GetAbilityLevel())));
	}

}


void UAbilityTask_EffectUseCollision::AttachToAvatar(const FAttachmentTransformRules& AttachmentRules, const FName& SocketName)
{
	Cast<ICustomAbilityHelperInterface>(Ability->GetAvatarActorFromActorInfo())->AttachComponentToCharacter(EffectShape, AttachmentRules, SocketName);
}


void UAbilityTask_EffectUseCollision::SetCollisionProfileName(const FName& ProfileName, bool bUpdateOverlap)
{
	EffectShape->SetCollisionProfileName(ProfileName, bUpdateOverlap);
}

void UAbilityTask_EffectUseCollision::SetShapeTransform(const FTransform& InTransform)
{
	EffectShape->SetRelativeTransform(InTransform);
}

void UAbilityTask_EffectUseCollision::OnEffectShapeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
#if WITH_EDITOR
	PFLOG(Warning, TEXT("On EffectShape BeginOverlap"));
#endif

	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OtherActor);
	if(ASI == nullptr) return;

	UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	if(AlreadyEffectedSystems.Contains(ASC)) return;

	AlreadyEffectedSystems.Add(ASC);
	for (auto& EffectSpecHandle : EffectSpecHandles)
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, ASC);
	}

	if(OnEffectShapeBeginOverlapEvent.IsBound()) OnEffectShapeBeginOverlapEvent.Broadcast(InstanceName, OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult );
}