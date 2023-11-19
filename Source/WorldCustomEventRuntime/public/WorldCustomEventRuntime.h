// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(WorldCustomEventRuntime, Log, All);

#define CUSTOMLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define RADLOG(VER, FMT, ...) UE_LOG(WorldCustomEventRuntime, VER, TEXT("%s %s"), *CUSTOMLOG_CALLINFO, *FString::Printf(FMT, ##__VA_ARGS__) )
#define LOG_INFO(FMT, ...) UE_LOG(WorldCustomEventRuntime, Display, TEXT("%s %s"), *CUSTOMLOG_CALLINFO, *FString::Printf(FMT, ##__VA_ARGS__) )
#define LOG_WARNING(FMT, ...) UE_LOG(WorldCustomEventRuntime, Warning, TEXT("%s %s"), *CUSTOMLOG_CALLINFO, *FString::Printf(FMT, ##__VA_ARGS__) )
#define LOG_ERROR(FMT, ...) UE_LOG(WorldCustomEventRuntime, Error, TEXT("%s %s"), *CUSTOMLOG_CALLINFO, *FString::Printf(FMT, ##__VA_ARGS__) )

class IWorldCustomEventRuntime : public IModuleInterface
{
public:

	static IWorldCustomEventRuntime& Get()
	{
		return FModuleManager::LoadModuleChecked<IWorldCustomEventRuntime>("WorldCustomEventRuntime");
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("WorldCustomEventRuntime");
	}
};

class FWorldCustomEventRuntime : public IWorldCustomEventRuntime
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};






