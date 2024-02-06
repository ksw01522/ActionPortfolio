// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionPortfolio : ModuleRules
{
	public ActionPortfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "DialogueRuntime", "GameplayTasks", "GameplayTags", "WorldCustomEventRuntime", "CustomInputSettingRuntime" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG", "RenderCore" });
	}
}
