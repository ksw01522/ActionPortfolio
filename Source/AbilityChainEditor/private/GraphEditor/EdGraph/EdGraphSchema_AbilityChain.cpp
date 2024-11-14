// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphEditor/EdGraph/EdGraphSchema_AbilityChain.h"

#define LOCTEXT_NAMESPACE "AbilityChainGraph"

FText UEdGraphSchema_AbilityChain::GetNodeCategoryName() const
{
    return LOCTEXT("NodeCategory", "어빌리티 체인");
}

#undef LOCTEXT_NAMESPACE