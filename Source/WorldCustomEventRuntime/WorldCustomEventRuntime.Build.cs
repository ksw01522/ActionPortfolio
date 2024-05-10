// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WorldCustomEventRuntime : ModuleRules
{
	public WorldCustomEventRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bLegacyPublicIncludePaths = true;
		ShadowVariableWarningLevel = WarningLevel.Error;

        PublicIncludePaths.AddRange(new string[]
        {
			"WorldCustomEventRuntime/public",
        });

        PrivateIncludePaths.AddRange(new string[]
		{
			"WorldCustomEventRuntime/public",
			"WorldCustomEventRuntime/private",
        });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});

		/*
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"UMG",
                "RenderCore"
            });
		*/
	}
}
