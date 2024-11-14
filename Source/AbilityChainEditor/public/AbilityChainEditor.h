// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <IAssetTools.h>
#include <EdGraphUtilities.h>
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(AbilityChainEditor, Log, All);

#define CUSTOMLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define LOG_INFO(FMT, ...) UE_LOG(AbilityChainEditor, Display, TEXT("%s %s"), *CUSTOMLOG_CALLINFO, *FString::Printf(FMT, ##__VA_ARGS__) )
#define LOG_WARNING(FMT, ...) UE_LOG(AbilityChainEditor, Warning, TEXT("%s %s"), *CUSTOMLOG_CALLINFO, *FString::Printf(FMT, ##__VA_ARGS__) )
#define LOG_ERROR(FMT, ...) UE_LOG(AbilityChainEditor, Error, TEXT("%s %s"), *CUSTOMLOG_CALLINFO, *FString::Printf(FMT, ##__VA_ARGS__) )

/**
 * The public interface to this module
 */
class IAbilityChainEditor : public IModuleInterface
{

public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static IAbilityChainEditor& Get()
	{
		return FModuleManager::LoadModuleChecked< IAbilityChainEditor >("AbilityChainEditor");
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("AbilityChainEditor");
	}
};





class FAbilityChainEditor : public IAbilityChainEditor
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


private:
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

	void RegisterCategory();

	void RegisterNodeFactory();
	void UnRegisterNodeFactory();

	void RegisterBlueprintEvents();

	void HandleNewAbilityChainEventBlueprintCreated(UBlueprint* Blueprint);


private:
	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;

	//TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory_AbilityChain;
	
	EAssetTypeCategories::Type AbilityChainAssetCategoryBit;
};

