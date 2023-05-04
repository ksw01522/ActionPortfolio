// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionPortfolio : ModuleRules
{
	public ActionPortfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "DialogueRuntime", "SkillTreeRuntime" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });
	}
}
