// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GAEffectBoxActor.h"
#include "Components/BoxComponent.h"
#include "Character/ActionPortfolioCharacter.h"
#include "Components/ArrowComponent.h"
#include "ActionPortfolio.h"

AGAEffectBoxActor::AGAEffectBoxActor()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("EfffectBox");
	RootComponent = BoxComponent;

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SetupAttachment(RootComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
		ArrowComponent->SetSimulatePhysics(false);
	}
#endif // WITH_EDITORONLY_DATA

}

void AGAEffectBoxActor::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AGAEffectBoxActor::BoxBeginOverlap);
}

void AGAEffectBoxActor::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OverlappedActors.Contains(OtherActor)) return;
	
	OverlappedActors.AddUnique(OtherActor);
	IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (AbilityInterface != nullptr) {
		EffectToTarget(AbilityInterface->GetAbilitySystemComponent());
	}
}
