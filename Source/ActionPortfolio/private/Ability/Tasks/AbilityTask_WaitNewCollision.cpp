// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Tasks/AbilityTask_WaitNewCollision.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Ability/CustomAbilityHelperInterface.h"

UAbilityTask_WaitNewCollision::UAbilityTask_WaitNewCollision(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

void UAbilityTask_WaitNewCollision::TickTask(float DeltaTime)
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

void UAbilityTask_WaitNewCollision::MakeCapsuleCollision(float Radius, float HalfHeight, const FName& InCollisionProfileName)
{
	CollisionShape = NewObject<UCapsuleComponent>(Ability->GetAvatarActorFromActorInfo(), InstanceName);
	CollisionShape->SetGenerateOverlapEvents(false);
	static_cast<UCapsuleComponent*>(CollisionShape)->InitCapsuleSize(Radius, HalfHeight);
	CollisionShape->SetCollisionProfileName(InCollisionProfileName, false);
	CollisionEnabled = CollisionShape->GetCollisionEnabled();
	CollisionShape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionShape->RegisterComponent();
	CollisionShape->CreationMethod = EComponentCreationMethod::Instance;
	CollisionShape->SetEnableGravity(false);
	CollisionShape->OnComponentBeginOverlap.AddDynamic(this, &UAbilityTask_WaitNewCollision::OnTaskBeginOverlap);

}

void UAbilityTask_WaitNewCollision::MakeBoxCollision(FVector InBoxExtent, const FName& InCollisionProfileName)
{
	CollisionShape = NewObject<UBoxComponent>(Ability->GetAvatarActorFromActorInfo(), InstanceName);
	CollisionShape->SetGenerateOverlapEvents(false);
	static_cast<UBoxComponent*>(CollisionShape)->InitBoxExtent(InBoxExtent);
	CollisionShape->SetCollisionProfileName(InCollisionProfileName, false);
	CollisionEnabled = CollisionShape->GetCollisionEnabled();
	CollisionShape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionShape->RegisterComponent();
	CollisionShape->CreationMethod = EComponentCreationMethod::Instance;
	
	CollisionShape->OnComponentBeginOverlap.AddDynamic(this, &UAbilityTask_WaitNewCollision::OnTaskBeginOverlap);

}

void UAbilityTask_WaitNewCollision::MakeSphereCollision(float Radius, const FName& InCollisionProfileName)
{
	CollisionShape = NewObject<USphereComponent>(Ability->GetAvatarActorFromActorInfo(), InstanceName);
	CollisionShape->SetGenerateOverlapEvents(false);
	static_cast<USphereComponent*>(CollisionShape)->InitSphereRadius(Radius);
	CollisionShape->SetCollisionProfileName(InCollisionProfileName, false);
	CollisionEnabled = CollisionShape->GetCollisionEnabled();
	CollisionShape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionShape->RegisterComponent();
	CollisionShape->CreationMethod = EComponentCreationMethod::Instance;
	CollisionShape->SetEnableGravity(false);
	CollisionShape->OnComponentBeginOverlap.AddDynamic(this, &UAbilityTask_WaitNewCollision::OnTaskBeginOverlap);

}

void UAbilityTask_WaitNewCollision::SetShapeTransform(const FTransform& InTransform)
{
	CollisionShape->SetRelativeTransform(InTransform);
}

void UAbilityTask_WaitNewCollision::SetCollisionProfileName(const FName& ProfileName, bool bUpdateOverlap)
{
	CollisionShape->SetCollisionProfileName(ProfileName, bUpdateOverlap);
}

void UAbilityTask_WaitNewCollision::AttachToAvatar(const FAttachmentTransformRules& AttachmentRules, const FName& SocketName)
{
	AActor* Avatar = Ability->GetAvatarActorFromActorInfo();
	USceneComponent* DefaultAttachComponent = Avatar->GetDefaultAttachComponent();
	
	CollisionShape->AttachToComponent(DefaultAttachComponent, AttachmentRules, SocketName);
}

void UAbilityTask_WaitNewCollision::AddIgnoreActor(AActor* InActor)
{
	if(InActor != nullptr) AlreadyOverlapActors.Add(InActor);
}

void UAbilityTask_WaitNewCollision::OnTaskBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AlreadyOverlapActors.Contains(OtherActor)) return;

	if (OtherActor)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(SweepResult);

		FGameplayAbilityTargetDataHandle Handle;
		Handle.Data.Add(TSharedPtr<FGameplayAbilityTargetData>(TargetData));
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnOverlapDel.Broadcast(Handle);
		}

		if(bActEventOnce) { EndTask(); }
		else {
			AlreadyOverlapActors.Add(OtherActor);
		}
	}
}

void UAbilityTask_WaitNewCollision::Activate()
{
	CollisionShape->SetGenerateOverlapEvents(true);
	CollisionShape->SetCollisionEnabled(CollisionEnabled);
#if WITH_EDITOR
	CollisionShape->SetHiddenInGame(false);
#endif

	if (0 < LifeTime) bRemoveAfterLifeTime = true;
}

void UAbilityTask_WaitNewCollision::OnDestroy(bool AbilityEnded)
{
	if (IsValid(CollisionShape))
	{
		CollisionShape->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		CollisionShape->DestroyComponent();
	}

	Super::OnDestroy(AbilityEnded);
}

UAbilityTask_WaitNewCollision* UAbilityTask_WaitNewCollision::WaitNewCollision(UGameplayAbility* OwningAbility, FName TaskInstanceName, bool bActEventOnce, UPrimitiveComponent* CustomCollision)
{
	UAbilityTask_WaitNewCollision* ReturnTask = NewAbilityTask<UAbilityTask_WaitNewCollision>(OwningAbility, TaskInstanceName);
	ReturnTask->bActEventOnce = bActEventOnce;
	if (CustomCollision)
	{
		ReturnTask->SetCustomCollision(CustomCollision);
	}

	return ReturnTask;
}
