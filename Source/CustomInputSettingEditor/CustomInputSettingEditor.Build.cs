// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CustomInputSettingEditor : ModuleRules
{
	public CustomInputSettingEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bLegacyPublicIncludePaths = true;
		ShadowVariableWarningLevel = WarningLevel.Error;

        PublicIncludePaths.AddRange(new string[]
        {
            "CustomInputSettingEditor/public"
        });

        PrivateIncludePaths.AddRange(new string[]
		{
			"CustomInputSettingRuntime/public"
        });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "CustomInputSettingRuntime"});

	
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
            });

	}
}
