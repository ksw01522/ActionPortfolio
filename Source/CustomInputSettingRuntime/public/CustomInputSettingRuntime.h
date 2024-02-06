// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(CustomInputSettingRuntime, Log, All);

#define CUSTOMLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define LOG_INFO(FMT, ...) UE_LOG(CustomInputSettingRuntime, Display, TEXT("%s %s"), *CUSTOMLOG_CALLINFO, *FString::Printf(FMT, ##__VA_ARGS__) )
#define LOG_WARNING(FMT, ...) UE_LOG(CustomInputSettingRuntime, Warning, TEXT("%s %s"), *CUSTOMLOG_CALLINFO, *FString::Printf(FMT, ##__VA_ARGS__) )
#define LOG_ERROR(FMT, ...) UE_LOG(CustomInputSettingRuntime, Error, TEXT("%s %s"), *CUSTOMLOG_CALLINFO, *FString::Printf(FMT, ##__VA_ARGS__) )

class ICustomInputSettingRuntime : public IModuleInterface
{
public:

	static ICustomInputSettingRuntime& Get()
	{
		return FModuleManager::LoadModuleChecked<ICustomInputSettingRuntime>("CustomInputSettingRuntime");
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("CustomInputSettingRuntime");
	}
};

class FCustomInputSettingRuntime : public ICustomInputSettingRuntime
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};






