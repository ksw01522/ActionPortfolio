// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EnhancedInputSubsystemInterface.h"
#include "CustomInputHelper.generated.h"
/**
 * 
 */

class APlayerController;


UCLASS()
class CUSTOMINPUTSETTINGRUNTIME_API UCustomInputHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//커스텀 인풋을 사용할 컨트롤러 추가
	UFUNCTION(BlueprintCallable, Category = "CustomInputHelper")
	static void RegistryCustomMappingUser(APlayerController* Target);

	//단일 인풋매핑 추가
	UFUNCTION(BlueprintCallable, Category = "CustomInputHelper", meta = (AutoCreateRefTerm = "Options"))
	static void AddInputMapping(APlayerController* PlayerController, class UInputMappingContext* Mapping, int Priority = 0, const FModifyContextOptions& Options = FModifyContextOptions());

	//단일 인풋매핑 제거
	UFUNCTION(BlueprintCallable, Category = "CustomInputHelper", meta = (AutoCreateRefTerm = "Options"))
	static void RemoveInputMapping(APlayerController* PlayerController, class UInputMappingContext* Mapping, const FModifyContextOptions& Options = FModifyContextOptions());

	//다중 인숲매핑 추가
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "CustomMappable", meta = (AutoCreateRefTerm = "Options"))
	virtual void AddPlayerMappableConfig(APlayerController* PlayerController, const UPlayerMappableInputConfig* Config, const FModifyContextOptions& Options = FModifyContextOptions());

	/** Removes all the input mapping contexts inside of this mappable config. */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "CustomMappable", meta = (AutoCreateRefTerm = "Options"))
	virtual void RemovePlayerMappableConfig(APlayerController* PlayerController, const UPlayerMappableInputConfig* Config, const FModifyContextOptions& Options = FModifyContextOptions());
};