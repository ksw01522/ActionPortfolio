// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AbilityChainRuntime : ModuleRules
{
	public AbilityChainRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bLegacyPublicIncludePaths = true;
		ShadowVariableWarningLevel = WarningLevel.Error;

        PublicIncludePaths.AddRange(new string[]
        {
			"AbilityChainRuntime/public",
        });

        PrivateIncludePaths.AddRange(new string[]
		{
			"AbilityChainRuntime/private",
        });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "GameplayAbilities",
            });
	}
}
