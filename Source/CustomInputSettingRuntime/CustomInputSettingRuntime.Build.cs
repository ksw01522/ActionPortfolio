// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CustomInputSettingRuntime : ModuleRules
{
	public CustomInputSettingRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bLegacyPublicIncludePaths = false;
		ShadowVariableWarningLevel = WarningLevel.Error;

        PublicIncludePaths.AddRange(new string[]
        {
            "CustomInputSettingRuntime/public",
        });

        PrivateIncludePaths.AddRange(new string[]
		{
			"CustomInputSettingRuntime/public",
			"CustomInputSettingRuntime/private",
        });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

	
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
            });

	}
}
