// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityChainEdge.h"


UAbilityChainNode* UAbilityChainEdge::GetFromNode() const
{
	checkf(From.IsValid(), TEXT("%s Can't find From Node"), *(FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")")));
	return From.Get();
}

UAbilityChainNode* UAbilityChainEdge::GetToNode() const
{
	checkf(To.IsValid(), TEXT("%s Can't find To Node"), *(FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")")));
	return To.Get();
}
