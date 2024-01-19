// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WorldCustomEventUncooked : ModuleRules
{
	public WorldCustomEventUncooked(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bLegacyPublicIncludePaths = false;
		ShadowVariableWarningLevel = WarningLevel.Error;

		PrivateIncludePaths.AddRange(new string[]
		{
            "WorldCustomEventUncooked/public",
		});

		PrivateIncludePaths.AddRange(new string[]
		{
            "WorldCustomEventUncooked/private",
			"WorldCustomEventRuntime/public"
		});

		PublicDependencyModuleNames.AddRange(new string[] {  });

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"WorldCustomEventRuntime",
                "BlueprintGraph",
                "Core", 
				"CoreUObject", 
				"Engine", 
				"KismetCompiler"
				// ... add private dependencies that you statically link with here ...
			}
			);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
