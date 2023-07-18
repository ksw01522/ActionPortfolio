// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Boss/BossAIController.h"
#include "Character/Enemy/Boss/CharacterBoss.h"
#include "ActionPortfolio.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Character/Widget_HPBar_Basic.h"

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
}

void ABossAIController::OnBossDamaged(float DamageAmount, AActor* DamageInstigator)
{
	GetBlackboardComponent()->SetValueAsBool(IsDamagedKey, true);
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ACharacterBoss* Boss = Cast<ACharacterBoss>(InPawn)) {
		Boss->OnDamagedDel.AddDynamic(this, &ABossAIController::OnBossDamaged);
	}
	else {
		PFLOG(Error, TEXT("BossAIController Possess Other Pawn Not Boss."));
	}

}

void ABossAIController::OnUnPossess()
{
	ACharacterBoss* Boss = GetPawn<ACharacterBoss>();
	if (IsValid(Boss))
	{
		Boss->OnDamagedDel.RemoveAll(this);
	}
	if (HPBarWidget != nullptr) {
		HPBarWidget->RemoveFromParent();
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
	if (HPBarClass.GetDefaultObject() != nullptr && IsValid(PlayerController)) {
		HPBarWidget = CreateWidget<UWidget_HPBar_Basic>(PlayerController, HPBarClass);
		if (HPBarWidget != nullptr) {
			HPBarWidget->RegistCharacter(GetPawn<AActionPortfolioCharacter>());
			HPBarWidget->AddToViewport();
		}

	}

}