// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Boss/BossAIController.h"
#include "Character/Enemy/Boss/CharacterBoss.h"
#include "ActionPortfolio.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"


const FName ABossAIController::IsDamagedKey(TEXT("IsDamaged"));

ABossAIController::ABossAIController()
{
	Sight_Config->SightRadius = 0;
	Sight_Config->LoseSightRadius = 0;
	Sight_Config->PeripheralVisionAngleDegrees = 0;
	Sight_Config->SetMaxAge(0);
	Sight_Config->AutoSuccessRangeFromLastSeenLocation = 0;
	Sight_Config->DetectionByAffiliation.bDetectEnemies = false;
	Sight_Config->DetectionByAffiliation.bDetectFriendlies = false;
	Sight_Config->DetectionByAffiliation.bDetectNeutrals = false;

	PrimaryActorTick.bCanEverTick = true;
}

void ABossAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DistToTargetFromXY = -1;

	if (AActionPortfolioCharacter* FocusedHostileTarget = Cast<AActionPortfolioCharacter>(GetBlackboardComponent()->GetValueAsObject(FName("FocusedHostileTarget"))))
	{
		FVector AvatarLocation = GetPawn()->GetActorLocation();
		FVector TargetLocation = FocusedHostileTarget->GetActorLocation();

		FVector AvatarToTarget = TargetLocation - AvatarLocation;
		
		DistToTargetFromXY = AvatarToTarget.X * AvatarToTarget.X + AvatarToTarget.Y * AvatarToTarget.Y;
	}

	GetBlackboardComponent()->SetValueAsFloat(FName("DistanceToTargetFromXY"), DistToTargetFromXY);

	if (ACharacterBoss* Boss = GetPawn<ACharacterBoss>()) {
		float HPPercent = Boss->GetHealth() / Boss->GetMaxHealth();
		GetBlackboardComponent()->SetValueAsFloat(FName("HPPercent"), HPPercent);
	}
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);



}

void ABossAIController::OnUnPossess()
{
	ACharacterBoss* Boss = GetPawn<ACharacterBoss>();
	if (IsValid(Boss))
	{
		Boss->OnDamagedDel.RemoveAll(this);
	}


	Super::OnUnPossess();
}

void ABossAIController::SetIsDamaged(bool NewState)
{
	GetBlackboardComponent()->SetValueAsBool(IsDamagedKey, NewState);
}

bool ABossAIController::IsDamaged() const
{
	return GetBlackboardComponent()->GetValueAsBool(IsDamagedKey);
}

void ABossAIController::FocusPlayer()
{
	SetFocusTargetForced(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	/*
	if (HPBarClass.GetDefaultObject() != nullptr && IsValid(PlayerController)) {
		HPBarWidget = CreateWidget<UWidget_HPBar_Basic>(PlayerController, HPBarClass);
		if (HPBarWidget != nullptr) {
			HPBarWidget->RegistCharacter(GetPawn<AActionPortfolioCharacter>());
			HPBarWidget->AddToViewport();
		}

	}
	*/
}
