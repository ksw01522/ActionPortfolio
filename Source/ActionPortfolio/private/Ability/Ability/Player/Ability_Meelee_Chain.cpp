// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Player/Ability_Meelee_Chain.h"
#include "Ability/Tasks/AbilityTask_WaitEventInputPress.h"
#include "Ability/ActionPFAbilitySystemComponent.h"

void UAbility_Meelee_Chain::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility_CPP(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	int NodesNum = ChainedAbilityNodes.Num();

	for (int Idx = 0; Idx < NodesNum; Idx++)
	{
		UAbilityTask_WaitEventInputPress* TempEvent = UAbilityTask_WaitEventInputPress::WaitEventInputPress(this, NAME_None, ChainedAbilityNodes[Idx].InputKey, false);

		TempEvent->OnPressDel.AddUObject(this, &UAbility_Meelee_Chain::OnEventInput);

		TempEvent->ReadyForActivation();
	}
}

void UAbility_Meelee_Chain::OnEventInput(int32 InputKey)
{
	FAbilityChainNode* FindedNode = FindChainNode(InputKey);

	if (FindedNode)
	{
		GetAbilitySystemComponentFromActorInfo()->TryActivateAbilityByClass(FindedNode->AbilityClass);
	}
}

FAbilityChainNode* UAbility_Meelee_Chain::FindChainNode(int32 InputKey) const
{
	int NodesNum = ChainedAbilityNodes.Num();

	for (int Idx = 0; Idx < NodesNum; Idx++)
	{
		if (ChainedAbilityNodes[Idx].InputKey == InputKey)
		{
			return const_cast<FAbilityChainNode*>(&ChainedAbilityNodes[Idx]);
		}
	}

	return nullptr;
}

#if WITH_EDITOR
EDataValidationResult UAbility_Meelee_Chain::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult SuperResult = Super::IsDataValid(ValidationErrors);
	EDataValidationResult Result = EDataValidationResult::Valid;

	int Num = ChainedAbilityNodes.Num();
	TMap<int, TArray<int>> IDNodeMap;
	
	for (int Idx = 0; Idx < Num; Idx++)
	{
		if (ChainedAbilityNodes[Idx].AbilityClass.GetDefaultObject() == nullptr)
		{
			ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("ChainedAbilityNodes Idx : {%d}'s Class Is Null"), Idx)));
			Result = EDataValidationResult::Invalid;
		}

		IDNodeMap.FindOrAdd(ChainedAbilityNodes[Idx].InputKey).Add(Idx);
	}

	for (auto& IDNode : IDNodeMap)
	{
		if(2 <= IDNode.Value.Num())
		{
			FString IndexArray = FString::FromInt(IDNode.Value[0]);

			for(int Idx = 1; Idx < IDNode.Value.Num(); Idx++)
			{
				IndexArray.Append(", " + FString::FromInt(IDNode.Value[1]));
			}

			ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("ChainedAbilityNodes Exist Multiple ID {%d} : { %s }"), IDNode.Key, *IndexArray)));
			Result = EDataValidationResult::Invalid;
		}
	}

	return SuperResult == EDataValidationResult::Valid ? Result : SuperResult;
}
void UAbility_Meelee_Chain::PostCDOCompiled(const FPostCDOCompiledContext& Context)
{
	ChainedAbilityNodes.Sort( [] (const FAbilityChainNode& A, const FAbilityChainNode & B) -> bool {return A.InputKey < B.InputKey;});
}
#endif