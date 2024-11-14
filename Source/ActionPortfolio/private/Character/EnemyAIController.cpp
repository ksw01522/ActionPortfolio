// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EnemyAIController.h"
#include "ActionPortfolio.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISenseConfig_Team.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Character/ActionPortfolioCharacter.h"

const FName AEnemyAIController::FocusedHostileTargetKey(TEXT("FocusedHostileTarget"));

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructPerceptionSystem();

	SetGenericTeamId(2);
}

void AEnemyAIController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (GetGenericTeamId() != NewTeamID)
	{
		Super::SetGenericTeamId(NewTeamID);
		if (IGenericTeamAgentInterface* PawnGTAI = Cast<IGenericTeamAgentInterface>(GetPawn()))
		{
			PawnGTAI->SetGenericTeamId(NewTeamID);
		}
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();


}

void AEnemyAIController::ConstructPerceptionSystem()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	SetPerceptionComponent(*AIPerception);

	//�þ�
	Sight_Config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	Sight_Config->SightRadius = 1400.f;
	Sight_Config->LoseSightRadius = Sight_Config->SightRadius + 400.f;
	Sight_Config->PeripheralVisionAngleDegrees = 180.f;
	Sight_Config->SetMaxAge(3.f);
	Sight_Config->AutoSuccessRangeFromLastSeenLocation = 200.f;
	Sight_Config->DetectionByAffiliation.bDetectEnemies = true;
	Sight_Config->DetectionByAffiliation.bDetectFriendlies = false;
	Sight_Config->DetectionByAffiliation.bDetectNeutrals = false;

	GetPerceptionComponent()->SetDominantSense(*Sight_Config->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*Sight_Config);
}

void AEnemyAIController::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{
	if (!IsValid(actor)) return;

	ETeamAttitude::Type Team = GetTeamAttitudeTowards(*actor);

	if (stimulus.WasSuccessfullySensed())						//On
	{
		if (Team == ETeamAttitude::Friendly)
		{

		}
		else if (Team == ETeamAttitude::Hostile)
		{
			OnHostileTargetDetected(actor);
		}
		else         //Neutral
		{

		}

	}
	else														//OffSight Target
	{
		if (Team == ETeamAttitude::Friendly)
		{

		}
		else if (Team == ETeamAttitude::Hostile)
		{
			RemoveDetectedHostileTarget(actor);
		}
		else         //Neutral
		{

		}
	}
	
}

void AEnemyAIController::RemoveDetectedHostileTarget(AActor* Target)
{
	DetectedHostileTargets.Remove(Target);

	RemoveNotValidHostileTargets();

	AActor* FocusedTarget = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusedHostileTargetKey));
	if (FocusedTarget == Target)
	{
		FocusOtherTarget();
	}
}


void AEnemyAIController::OnHostileTargetDetected(AActor* NewTarget)
{
	if(!IsValid(NewTarget)) return;

#if WITH_EDITOR
	//PFLOG(Warning, TEXT("Add Hostile Target Name : %s"), *NewTarget->GetFName().ToString());
#endif

	DetectedHostileTargets.AddUnique(NewTarget);

	if (AActor* FocusedTarget = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusedHostileTargetKey)))
	{
		
	}
	else
	{
#if WITH_EDITOR
		//PFLOG(Warning, TEXT("Focus Hostile Target Name : %s"), *NewTarget->GetFName().ToString());
#endif

		GetBlackboardComponent()->SetValueAsObject(FocusedHostileTargetKey, NewTarget);
	}	
}



void AEnemyAIController::FocusOtherTarget()
{
	if (DetectedHostileTargets.IsEmpty()) {
		GetBlackboardComponent()->SetValueAsObject(FocusedHostileTargetKey, nullptr);
		return;
	}

	int RandIdx = FMath::Rand() % DetectedHostileTargets.Num();
	if (!DetectedHostileTargets[RandIdx].IsValid())
	{
		RemoveNotValidHostileTargets();
		RandIdx = FMath::Rand() % DetectedHostileTargets.Num();
	}
	if (DetectedHostileTargets.IsValidIndex(RandIdx))
	{
		GetBlackboardComponent()->SetValueAsObject(FocusedHostileTargetKey, DetectedHostileTargets[RandIdx].Get());
		
		#if WITH_EDITOR
		/*if (DetectedHostileTargets[RandIdx].IsValid()) {
			PFLOG(Warning,TEXT("Focus Hostile Target : %s."), *DetectedHostileTargets[RandIdx]->GetFName().ToString());
		}*/
		#endif
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* TempBlackbaordComponent = Blackboard;
	if (UseBlackboard(BB_AI, TempBlackbaordComponent))
	{
		if (!RunBehaviorTree(BT_AI))
		{
			PFLOG(Warning, TEXT("Can't Run Enemy AI"));
			return;
		}

		UAIPerceptionSystem::GetCurrent(GetWorld())->UpdateListener(*AIPerception);
	}
	else
	{
		PFLOG(Warning, TEXT("Can't Use BlackBoard"));
		return;
	}
}

void AEnemyAIController::OnUnPossess()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}

	Super::OnUnPossess();
}

void AEnemyAIController::RemoveNotValidHostileTargets()
{
	for (int Idx = 0; Idx < DetectedHostileTargets.Num(); Idx++)
	{
		if (!DetectedHostileTargets[Idx].IsValid())
		{
			DetectedHostileTargets.RemoveAt(Idx);
			Idx--;
		}
	}

}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActionPortfolioCharacter* PossesChar = GetPawn<AActionPortfolioCharacter>();
	if (IsValid(PossesChar))
	{
		GetBlackboardComponent()->SetValueAsBool("CanBasicAct", PossesChar->CanBasicAct());
	}

}

void AEnemyAIController::SetFocusTargetForced(AActor* NewTarget) 
{
	GetBlackboardComponent()->SetValueAsObject(FocusedHostileTargetKey, NewTarget);
}

AActionPortfolioCharacter* AEnemyAIController::GetCurrentFocusedTarget() const
{
	return Cast<AActionPortfolioCharacter>(GetBlackboardComponent()->GetValueAsObject(FocusedHostileTargetKey));
}
